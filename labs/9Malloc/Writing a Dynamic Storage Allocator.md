# Writing a Dynamic Storage Allocator

## 说明

- textbook中提供了一套简单的动态内存分配器实现，定义了链表格式，实现了大部分操作，但是find_fit和place没有提供。

- 对于动态内存分配器的优化，textbook中给出了几种思路，包括显示链表、适配分离、伙伴系统等。


## 简单隐式空闲链表

### 隐式链表格式与宏操作
- 链表包含一个头部和一个脚部，这两部分完全相同。之所以放置两部分，是因为在遍历链表或者获取前后链表项时，脚部的信息为这种操作提供了可能。

- 头部信息是8位对齐的，最后一位被设计为是否已被分配的标志位。

- 为了方便获取块大小、是否已被分配等头部信息，将这些通用的指针操作封装成了宏，避免出错。

```c
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x, y) ((x) > (y)? (x) : (y))

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
```

### find_fit
- textbook中没有给出find_fit的实现，这里采用首次适配。
```c
static void *find_fit(size_t asize) {
  void *current_head = heap_listp;
  while (GET_SIZE(HDRP(current_head)) != 0) {
    if (GET_SIZE(HDRP(current_head)) >= asize && !GET_ALLOC(HDRP(current_head))) return current_head;
    current_head = NEXT_BLKP(current_head);
  }
  return NULL;
}

```

### place
- place可以对内存块split，但是对小内存块的split很可能造成外部碎片。
```c
static void place(void *bp, size_t asize) {
  size_t size = GET_SIZE(HDRP(bp));
  if (size >= (asize + 2 * DSIZE)) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(size - asize, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size - asize, 0));
  } else {
    PUT(HDRP(bp), PACK(size, 1));
    PUT(FTRP(bp), PACK(size, 1));
  }
}
```

### 结果
```bash
Results for libc malloc:
trace  valid  util     ops      secs  Kops
 0       yes    0%    5694  0.000872  6531
 1       yes    0%    5848  0.000697  8391
 2       yes    0%    6648  0.001077  6172
 3       yes    0%    5380  0.001238  4345
 4       yes    0%   14400  0.000222 64953
 5       yes    0%    4800  0.001516  3167
 6       yes    0%    4800  0.001844  2603
 7       yes    0%   12000  0.000708 16949
 8       yes    0%   24000  0.000651 36872
 9       yes    0%   14401  0.000359 40137
10       yes    0%   14401  0.000168 85874
Total           0%  112372  0.009351 12017

Results for mm malloc:
trace  valid  util     ops      secs  Kops
 0       yes   99%    5694  0.006692   851
 1       yes   99%    5848  0.006121   955
 2       yes   99%    6648  0.010305   645
 3       yes  100%    5380  0.007419   725
 4       yes   66%   14400  0.000134107704
 5       yes   91%    4800  0.006518   736
 6       yes   92%    4800  0.006229   771
 7       yes   55%   12000  0.132141    91
 8       yes   51%   24000  0.237033   101
 9       yes   27%   14401  0.057591   250
10       yes   34%   14401  0.002065  6973
Total          74%  112372  0.472247   238

Perf index = 44 (util) + 16 (thru) = 60/100
```

## 分离适配

- 对简单隐式空闲链表的一个改进是，使用分离适配。

- 在分离适配策略中，分配器维护多个大小类空闲链表。处理一个分配请求时，首先对适当的大小类空闲链表做首次适配，如果未找到则在下一个大小类空闲链表中搜索。

- 采用分离适配后，主要的难点在于对于空闲链表的维护。

### 大小类定义及宏的扩展

- 空闲链表头部指针存储在一个大小为14的指针数组中。

- 大小类的定义采用textbook推荐的做法，即按照2的冥来组织不同大小的内存块。

- 分离适配中的空闲链表原则上可以被组织成某种类型的显式或隐式链表，这里采用单向显式链表的实现。在原payload部分的第1-4字节存放下一个链表项的指针。

- 一个巧妙的设计是，将内存块的头部信息的第二位作为是否有继承者的标志位。在一个内存块加入到空闲链表后，要检查原空闲链表是否为空。在移除时，要检查后续是否还有继承者。

```c
#define GET_SUCC_BLKP(bp) ((void *)*(unsigned int *)(bp))
#define SET_SUCC_BLKP(bp, succ) (*(unsigned int *)(bp) = (unsigned int)(succ))
#define HAS_SUCC_BLKP(bp) (GET(bp) >> 1 & 0x1)
```

### find_fit
- 这里的find_fit仍然是首次适配。
```c
static void *find_fit(size_t asize) {
  int index = ilog2(asize);
  if (index > 13) index = 13;
  while (index <= 13) {
    void *current = free_list[index];
    int count = 5;
    while (current != NULL && GET_SIZE(HDRP(current)) != 0 && !GET_ALLOC(HDRP(current)) && count > 0) {
      if (GET_SIZE(HDRP(current)) >= asize) return current;
      current = HAS_SUCC_BLKP(HDRP(current)) ? GET_SUCC_BLKP(current) : NULL;
      count--;
    }
    index++;
  }
  return NULL;
}
```

### free_list_insert
- free_list_insert用来将内存块加入到对应的空闲链表头部。

- free_list_insert将在free结束时、place结束时被调用。place有可能分割内存块，此时要将剩余的内存块加入空闲链表。

```c
static void free_list_insert(void *bp) {
  size_t size = GET(HDRP(bp));
  int index = ilog2(size);
  if (index > 13) index = 13;
  void *head = free_list[index];
  free_list[index] = bp;
  SET_SUCC_BLKP(bp, head);
  if (head != NULL) PUT(HDRP(bp), GET(HDRP(bp)) | (0x2));
}
```

### free_list_remove
- free_list_remove用来将内存块从空闲链表中移除。

- free_list_remove应该在coalesce、find_fit以后被调用。find_fit后该内存块即将被使用；coalesce时，该内存块合并后即将重新加入空闲链表，因此原来的内存块应当被移除。

- 这里要小心处理头部信息的设置。内存块的头部信息的第二位作为是否有继承者的标志位。在一个内存块加入到空闲链表后，要检查原空闲链表是否为空。在移除时，要检查后续是否还有继承者。

```c
static void free_list_remove(void *bp) {
  size_t size = GET(HDRP(bp));
  int index = ilog2(size);
  if (index > 13) index = 13;
  void *current = free_list[index];
  void *prev = current;
  while (current != NULL && GET_SIZE(HDRP(current)) != 0 && !GET_ALLOC(HDRP(current))) {
    void *succ = HAS_SUCC_BLKP(HDRP(current)) ? GET_SUCC_BLKP(current) : NULL;
    if (current == bp) {
      if (prev == current) {
        free_list[index] = succ;
      } else {
        if (succ == NULL) PUT(HDRP(prev), GET(HDRP(prev)) & (~0x2));
        else {
          SET_SUCC_BLKP(prev, succ);
          PUT(HDRP(prev), GET(HDRP(prev)) | 0x2);
        }
      }
      break;
    }
    prev = current;
    current = succ;
  }
}
```

### 结果
```bash
Results for libc malloc:
trace  valid  util     ops      secs  Kops
 0       yes    0%    5694  0.000867  6570
 1       yes    0%    5848  0.000686  8522
 2       yes    0%    6648  0.001122  5923
 3       yes    0%    5380  0.001043  5156
 4       yes    0%   14400  0.000219 65723
 5       yes    0%    4800  0.001596  3007
 6       yes    0%    4800  0.001669  2876
 7       yes    0%   12000  0.000795 15085
 8       yes    0%   24000  0.001002 23954
 9       yes    0%   14401  0.000441 32648
10       yes    0%   14401  0.000190 75636
Total           0%  112372  0.009632 11667

Results for mm malloc:
trace  valid  util     ops      secs  Kops
 0       yes   98%    5694  0.000228 24963
 1       yes   94%    5848  0.000222 26366
 2       yes   98%    6648  0.000290 22932
 3       yes   99%    5380  0.000232 23210
 4       yes   66%   14400  0.000542 26563
 5       yes   80%    4800  0.001823  2634
 6       yes   77%    4800  0.001359  3532
 7       yes   55%   12000  0.008105  1481
 8       yes   51%   24000  0.029201   822
 9       yes   30%   14401  0.059474   242
10       yes   45%   14401  0.002583  5576
Total          72%  112372  0.104058  1080

Perf index = 43 (util) + 40 (thru) = 83/100
```

- 可以看到对比于简单隐式链表的实现有了比较大的提升，但还算不上让人满意。

- 关于结果中的吞吐量，mm实现大约是libc的十分之一，特别是随着操作数变大，kops显著减小。原因也是显而易见的，因为mm实现采用了首次适配，在遍历空闲链表时的性能较差，从而导致吞吐量随着操作数变大而减小。

- 关于结果中的空间利用率，mm实现大约是libc的1.39倍，甚至比简单隐式链表的实现还要差。一方面可能是大小类的数量太小，导致内部碎片过多；另一方面因为采用了首次适配，因此外部碎片也会比较多。