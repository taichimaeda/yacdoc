#ifndef YACDOC_HASHMAP_H
#define YACDOC_HASHMAP_H

typedef struct {
    char *key;
    void *value;
} YacDocHashMapItem;

typedef struct {
    int capacity;
    int size;
    YacDocHashMapItem **items;
} YacDocHashMap;

typedef struct {
    int count;
    int last;
    YacDocHashMap *map;
} YacDocHashMapIterator;

typedef void (* YacDocHashMapValueFreeFunc)(void *);

YacDocHashMap *yacdoc_hashmap_new();
void yacdoc_hashmap_free(YacDocHashMap *map, YacDocHashMapValueFreeFunc free_func);
bool yacdoc_hashmap_add(YacDocHashMap *map, const char* key, void *value);
void *yacdoc_hashmap_get(YacDocHashMap *map, const char* key);
YacDocHashMapIterator *yacdoc_hashmap_iterator_new(YacDocHashMap *map);
void yacdoc_hashmap_iterator_free(YacDocHashMapIterator *it);
YacDocHashMapItem *yacdoc_hashmap_iterator_next(YacDocHashMapIterator *it);
int yacdoc_hashmap_iterator_count(YacDocHashMapIterator *it);

#endif
