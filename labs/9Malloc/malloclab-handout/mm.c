/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
        /* Team name */
        "ateam",
        /* First member's full name */
        "Harry Bovik",
        /* First member's email address */
        "bovik@cs.cmu.edu",
        /* Second member's full name (leave blank if none) */
        "",
        /* Second member's email address (leave blank if none) */
        ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

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

#define GET_SUCC_BLKP(bp) ((void *)*(unsigned int *)(bp))
#define SET_SUCC_BLKP(bp, succ) (*(unsigned int *)(bp) = (unsigned int)(succ))
#define HAS_SUCC_BLKP(bp) (GET(bp) >> 1 & 0x1)

void *heap_listp;
void *free_list[14];

static void *extend_heap(size_t words);

static void *coalesce(void *bp);

static void *find_fit(size_t asize);

static void place(void *bp, size_t asize);

static void free_list_insert(void *bp);

static void free_list_remove(void *bp);

static void print_free_list();

int ilog2(int x) {
  int tmp = x >> 16;
  int flg1 = (~!tmp << 31) >> 31;
  x = (flg1 & tmp) | (~flg1 & x);
  tmp = x >> 8;
  int flg2 = (~!tmp << 31) >> 31;
  x = (flg2 & tmp) | (~flg2 & x);
  tmp = x >> 4;
  int flg3 = (~!tmp << 31) >> 31;
  x = (flg3 & tmp) | (~flg3 & x);
  tmp = x >> 2;
  int flg4 = (~!tmp << 31) >> 31;
  x = (flg4 & tmp) | (~flg4 & x);
  tmp = x >> 1;
  int flg5 = (~!tmp << 31) >> 31;
  x = (flg5 & tmp) | (~flg5 & x);
  int result = ((0x1 & flg5) + (0x2 & flg4) + (0x4 & flg3) + (0x8 & flg2) + (0x10 & flg1));
  return result + 1;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) -1)
    return -1;
  PUT(heap_listp, 0);
  PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
  heap_listp += (2 * WSIZE);

  if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
    return -1;
  return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  //print_free_list();
  size_t asize;
  size_t extendsize;
  void *bp;

  if (size == 0)
    return NULL;

  if (size <= DSIZE)
    asize = 2 * DSIZE;
  else
    asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

  if ((bp = find_fit(asize)) != NULL) {
    free_list_remove(bp);
    place(bp, asize);
    return bp;
  }

  extendsize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    return NULL;
  place(bp, asize);
  return bp;
}

/*
 * mm_free - Freeing a block.
 */
void mm_free(void *ptr) {
  size_t size = GET_SIZE(HDRP(ptr));

  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  void *bp = coalesce(ptr);
  free_list_insert(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copy_size;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copy_size = GET_SIZE(HDRP(oldptr));
  if (size < copy_size)
    copy_size = size;
  memcpy(newptr, oldptr, copy_size);
  mm_free(oldptr);
  return newptr;
}

static void *extend_heap(size_t words) {
  void *bp;
  size_t size;

  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
  if ((long) (bp = mem_sbrk(size)) == -1) return NULL;

  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
  return coalesce(bp);
}

static void *coalesce(void *bp) {
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (!prev_alloc) {
    free_list_remove(PREV_BLKP(bp));
  }
  if (!next_alloc) {
    free_list_remove(NEXT_BLKP(bp));
  }

  if (prev_alloc && next_alloc) {
    return bp;
  } else if (prev_alloc && !next_alloc) {
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  } else if (!prev_alloc && next_alloc) {
    size += GET_SIZE(FTRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  } else {
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }
  return bp;
}

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

static void place(void *bp, size_t asize) {
  size_t size = GET_SIZE(HDRP(bp));
  if (size >= (asize + 2 * DSIZE)) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(size - asize, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size - asize, 0));
    free_list_insert(NEXT_BLKP(bp));
  } else {
    PUT(HDRP(bp), PACK(size, 1));
    PUT(FTRP(bp), PACK(size, 1));
  }
}

static void free_list_insert(void *bp) {
  size_t size = GET(HDRP(bp));
  int index = ilog2(size);
  if (index > 13) index = 13;
  void *head = free_list[index];
  free_list[index] = bp;
  SET_SUCC_BLKP(bp, head);
  if (head != NULL) PUT(HDRP(bp), GET(HDRP(bp)) | (0x2));
}

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

static void print_free_list() {
  for (int i = 0; i < 14; i++) {
    void *curr = free_list[i];
    fprintf(stderr, "free_list pow(2,%d) size classes:", i);
    while (curr != NULL) {
      fprintf(stderr, "%x,%d,%x,%d; ", &GET(curr), GET(HDRP(curr)), GET(curr), HAS_SUCC_BLKP(HDRP(curr)));
      curr = HAS_SUCC_BLKP(HDRP(curr)) ? GET_SUCC_BLKP(curr) : NULL;
    }
    fprintf(stderr, "\n");
  }
}
