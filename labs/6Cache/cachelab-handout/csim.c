#include "cachelab.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"

typedef struct {
    bool valid;
    long key;
    unsigned long LRU_count;
    char b[0];
} cache_entry;

bool v_mode = false;
int S = 0;
int s = 0;
int E = 0;
int B = 0;
int b = 0;
int t = 0;
FILE *trace_file;
int hits;
int misses;
int evictions;

int tolower(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c + 'a' - 'A';
  } else {
    return c;
  }
}

long string_to_long(char s[], int i) {
  long result = 0;
  if (s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
    i += 2;
  }
  for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); i++) {
    if (tolower(s[i] > '9')) {
      result = 16 * result + (10 + tolower(s[i]) - 'a');
    } else {
      result = 16 * result + (tolower(s[i]) - '0');
    }
  }
  return result;
}

int check_hit(cache_entry *cache_set_start, long address) {
  int result = -1;
  for (int i = 0; i < E; i++) {
    long mark = address >> (s + b);
    cache_entry *cache = cache_set_start + i;
    if (cache->valid == true) {
      if (mark == cache->key) {
        cache->LRU_count = (cache->LRU_count >> 1) + (1L << 63);
        if (v_mode) {
          printf("hit ");
        }
        result = i;
      } else {
        cache->LRU_count = cache->LRU_count >> 1;
      }
    }
  }
  return result;
}

int get_free_index(cache_entry *cache_set_start) {
  for (int i = 0; i < E; i++) {
    if ((cache_set_start + i)->valid == false) {
      return i;
    }
  }
  return -1;
}

int eviction(cache_entry *cache_set_start) {
  int index = 0;
  int least_count = (1 << 31) >> 31;
  for (int i = 0; i < E; i++) {
    cache_entry *cache = cache_set_start + index;
    if (cache->LRU_count < least_count) {
      least_count = cache->LRU_count;
      index = i;
    }
  }
  (cache_set_start + index)->valid = false;
  return index;
}

void parse_trace_file(cache_entry *cache[]) {
  //printf("starting to parse trace file %p\n",trace_file);
  char line[100];
  while (!feof(trace_file)) {
    if (fgets(line, 100, trace_file) == NULL) break;
    if (line[0] == 'I') continue;
    for (int i = 0; i < 100; i++) {
      if (line[i] == '\n') {
        line[i] = ' ';
        break;
      }
    }
    if (v_mode) {
      printf("\n%s", line + 1);
    }
    long address = string_to_long(line, 3);
    //printf("the address is %ld, the size is %d\n",address,size);
    cache_entry *cache_set_start = cache[((address >> b) % S) * E];
    if (line[1] == 'L') {
      if (check_hit(cache_set_start, address) != -1) {
        hits++;
      } else {
        misses++;
        if (v_mode) {
          printf("miss ");
        }
        int index = get_free_index(cache_set_start);
        cache_entry *cache;
        if (index == -1) {
          evictions++;
          if (v_mode) {
            printf("eviction ");
          }
          cache = cache_set_start + eviction(cache_set_start);
        } else {
          cache = cache_set_start + index;
        }
        cache->valid = true;
        cache->key = address >> (s + b);
        cache->LRU_count = 0;
      }
    } else if (line[1] == 'M') {
      if (check_hit(cache_set_start, address) != -1) {
        hits++;
      } else {
        misses++;
        if (v_mode) {
          printf("miss ");
        }
        int index = get_free_index(cache_set_start);
        cache_entry *cache;
        if (index == -1) {
          evictions++;
          if (v_mode) {
            printf("eviction ");
          }
          cache = cache_set_start + eviction(cache_set_start);
        } else {
          cache = cache_set_start + index;
        }
        cache->valid = true;
        cache->key = address >> (s + b);
        cache->LRU_count = 0;
      }
      hits++;
      if (v_mode) {
        printf("hit ");
      }
    } else if (line[1] == 'S') {
      if (check_hit(cache_set_start, address) != -1) {
        hits++;
      } else {
        misses++;
        if (v_mode) {
          printf("miss ");
        }
        int index = get_free_index(cache_set_start);
        cache_entry *cache;
        if (index == -1) {
          evictions++;
          if (v_mode) {
            printf("eviction ");
          }
          cache = cache_set_start + eviction(cache_set_start);
        } else {
          cache = cache_set_start + index;
        }
        cache->valid = true;
        cache->key = address >> (s + b);
        cache->LRU_count = 0;
      }
    }
  }
  if (v_mode) {
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  char *file_name;
  while (argc-- > 1) {
    if (strcmp("-v", *argv) == 0) {
      v_mode = true;
    } else if (strcmp("-s", *argv) == 0) {
      argv++;
      argc--;
      s = atoi(*argv);
      S = pow(2, s);
    } else if (strcmp("-E", *argv) == 0) {
      argv++;
      argc--;
      E = atoi(*argv);
    } else if (strcmp("-b", *argv) == 0) {
      argv++;
      argc--;
      b = atoi(*argv);
      B = pow(2, b);
    } else if (strcmp("-t", *argv) == 0) {
      argv++;
      argc--;
      file_name = *argv;
    }
    argv++;
  }
  if ((S == 0) || (E == 0) || (b == 0) || (file_name == NULL)) {
    printf("missing required command line argument\n");
    return 0;
  }
  printf("S is %d, E is %d, b is %d, file is %s\n", S, E, b, file_name);
  t = 64 - s - b;
  cache_entry *cache[S * E];
  for (int i = 0; i < S * E; i++) {
    cache[i] = malloc(sizeof(cache_entry) + B);
    //printf("the address is %p\n",cache[i*S + j]);
    memset(cache[i], 0, sizeof(*cache[i]));
  }
  trace_file = fopen(file_name, "r");
  //printf("the size of cache_entry is %ld\n",sizeof(cache_entry));
  parse_trace_file(cache);
  for (int i = 0; i < S * E; i++) {
    if (cache[i] != NULL) {
      //free(cache[i]);
    }
  }
  printSummary(hits, misses, evictions);
  return 0;
}
