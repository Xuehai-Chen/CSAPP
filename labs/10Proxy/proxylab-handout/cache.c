#include "cache.h"
#include <stdio.h>

void init_cache(cache_entry *cache[]) {
  for (int i = 0; i < 10; i++) {
    cache[i] = Malloc(sizeof(cache_entry));
    memset(cache[i], 0, sizeof(cache_entry));
  }
}

cache_entry *get_by_uri(cache_entry *cache[], char *uri) {
  cache_entry *result = NULL;
  for (int i = 0; i < 10; i++) {
    cache_entry *current = cache[i];
    if (strcmp(current->key, uri) == 0) {
      current->LRU_count = (current->LRU_count >> 1) + (1L << 63);
      result = current;
    } else {
      current->LRU_count = current->LRU_count >> 1;
    }
  }
  return result;
}

int get_free(cache_entry *cache[]) {
  int result = -1;
  for (int i = 0; i < 10; i++) {
    cache_entry *current = cache[i];
    if (current->valid == false) {
      return i;
    }
  }
  return result;
}

int eviction(cache_entry *cache[]) {
  int index = 0;
  unsigned long least_count = 0xFFFFFFFFFFFFFFFF;
  for (int i = 0; i < 10; i++) {
    cache_entry *current = cache[i];
    if (current->LRU_count < least_count) {
      index = i;
      least_count = current->LRU_count;
    }
  }
  return index;
}

void set_by_uri(cache_entry *cache[], char *uri, char *data) {
  int index = get_free(cache);
  if (index < 0) {
    index = eviction(cache);
  }
  cache_entry *current = cache[index];
  strncpy(current->key, uri, strlen(uri));
  current->valid = true;
  current->LRU_count = (1L << 63);
  memcpy((void *) current->data, (void *) data, 102400);
}


