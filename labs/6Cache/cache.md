## Part A:Writing a Cache Simulator

### 参数说明
组相连高速缓存的三个主要参数：
- S：组数
- E：每组的行数
- B：2^b,块大小

### 设计思路
- 整个高速缓存的结构可以看做是一个指针数组，每个数组项对应于一个高速缓存行的变长结构体的指针。
- 高速缓存行的结构设计：
1. 由于块大小由参数确定，因此该结构体应该是一个变长结构体。
2. 高速缓存的替换算法是LRU，因此结构体内应包含LRU计数位，可以直接用long型变量来表示。
```C
typedef struct {
	bool valid;
	unsigned long key;
	unsigned long LRU_count;
	char b[0];
}cache_entry;
```
### TRACE_FILE的解析
- trace file 是由linux程序valgrind生成的，用来跟踪程序执行过程中的访存行为。
```shell
linux> valgrind --log-fd=1 --tool=lackey -v --trace-mem=yes ls -l
```

-  trace file 的字段解释：
```shell
[space]operation address,size
其中，operation包含I 指令加载，L 数据加载，S 数据写入，M 数据修改；
只有I指令前没有空格；
```

- L指令：先看是否命中，如果不命中，判断组中是否有空闲的缓存块，如果没有则根据LRU选择牺牲一个缓存项，然后加载数据。

- S指令：先看是否命中，如果不命中，判断组中是否有空闲的缓存块，如果没有则根据LRU选择牺牲一个缓存项，然后写入数据，代码实现上和L指令完全相同。

- M指令：先看是否命中，如果不命中则加载，然后将新值写入，类似L和S的融合。

### LRU替换算法的实现
- 此处采用软件模拟的方式，为每一个缓存项维护一个LRU_count域，当所在组被访问时，组中所有缓存项的LRU_count右移一位，同时命中的缓存项加0x8000000000000000。

- 如果某组的缓存项已满且需要置换时，选择LRU_count值最小的缓存项置换。

### 结果
```shell
$./test-csim 
                        Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
     3 (1,1,1)       9       8       6       9       8       6  traces/yi2.trace
     3 (4,2,4)       4       5       2       4       5       2  traces/yi.trace
     3 (2,1,4)       2       3       1       2       3       1  traces/dave.trace
     3 (2,1,3)     167      71      67     167      71      67  traces/trans.trace
     3 (2,2,3)     201      37      29     201      37      29  traces/trans.trace
     3 (2,4,3)     212      26      10     212      26      10  traces/trans.trace
     3 (5,1,5)     231       7       0     231       7       0  traces/trans.trace
     6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/long.trace
    27
TEST_CSIM_RESULTS=27
```

### 细节注意
- LRU_count 和 key 域都应该声明成无符号长整型，这是因为C语言的移位操作会根据有无符号选择逻辑移位还是符号移位，这里很有可能影响计算结果。

- free和malloc要成对使用，malloc后应当及时free，即便这个simulator里不做free不会造成问题，但是不作free很有可能造成内存溢出。

- 官方文档建议采用getopt.h解析命令行参数，初步实现时没有注意到这一点，采用了原始的字符串比较，后面有时间可以adopt这个建议。

&nbsp;


## Part B: Optimizing Matrix Transpose

### 说明
- 这个部分主要是根据缓存命中的特性对矩阵转置函数做优化， B=transpose of A。

- 主要用到的优化手段在chapter5,chapter6当中有提及，包括code motion, loop unrolling, blocking等。

- cache lab是performace lab的替代，早先CMU是用performance lab作为assignment，特别是优化这个部分可以参考7Performance lab。

### 分析及实现
- 首先，这个优化是针对一个特定的高速缓存大小(s = 5, E = 1, b = 5)。

- E=1说明这是一个直接映射的高速缓存。

- b=5说明块大小为32字节，矩阵元素类型为int，因此一个块可以存放8个元素。

- s=5说明组数为32，相邻32个块会被映射到各自不同的组里。

- 对于M=32,N=32，A矩阵一行对应4组，B矩阵按列访问，每个元素间隔4组，32组最大可以容纳8组B矩阵块，因此blocking size =8。

- 对于M=64,N=64，A矩阵一行对应8组，B矩阵按列访问，每个元素间隔8组，32组最大可以容纳4组B矩阵块，因此blocking size =4。

- 由以上分析不难得到M=61,N=67时，blocking size = 4。

- 代码实现：
```C
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, tmp;
  int bsize = 32 / ((M % 8) != 0 ? ((M / 8) + 1) : (M / 8));
  for (i = 0; i < N; i += bsize) {
    for (i = 0; i < N; i += bsize) {
      for (j = 0; j < M; j += bsize) {
        int iinext = i + bsize;
        int jjnext = j + bsize;
        for (int ii = i; ii < iinext && ii < N; ii++) {
          for (int jj = j; jj < jjnext && jj < M; jj++) {
            tmp = A[ii][jj];
            B[jj][ii] = tmp;
          }
        }
      }
    }
  }
}
```
### 结果
```shell
$ ./test-trans -M 32 -N 32

Function 0 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 0 (Transpose submission): hits:1710, misses:343, evictions:311

Function 1 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 1 (Simple row-wise scan transpose): hits:870, misses:1183, evictions:1151

Summary for official submission (func 0): correctness=1 misses=343

TEST_TRANS_RESULTS=1:343



$ ./test-trans -M 64 -N 64

Function 0 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 0 (Transpose submission): hits:6306, misses:1891, evictions:1859

Function 1 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 1 (Simple row-wise scan transpose): hits:3474, misses:4723, evictions:4691

Summary for official submission (func 0): correctness=1 misses=1891

TEST_TRANS_RESULTS=1:1891



$ ./test-trans -M 61 -N 67

Function 0 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 0 (Transpose submission): hits:5754, misses:2425, evictions:2393

Function 1 (2 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 1 (Simple row-wise scan transpose): hits:3756, misses:4423, evictions:4391

Summary for official submission (func 0): correctness=1 misses=2425

TEST_TRANS_RESULTS=1:2425
```

- 官方的评分细则是：
1. 32 × 32: 8 points if m < 300, 0 points if m > 600
2. 64 × 64: 8 points if m < 1, 300, 0 points if m > 2, 000
3. 61 × 67: 10 points if m < 2, 000, 0 points if m > 3, 000

- 显然，这里还有很大的优化空间，缓存命中如何进一步优化值得思考。