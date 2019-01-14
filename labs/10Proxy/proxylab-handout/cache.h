#include "csapp.h"
#include "stdbool.h"

typedef struct {
    bool valid;
    unsigned long LRU_count;
    char key[2490];
    char data[102400];
} cache_entry;

void init_cache(cache_entry *cache[]);

cache_entry *get_by_uri(cache_entry *cache[], char *uri);

void set_by_uri(cache_entry *cache[], char *uri, char *data);
