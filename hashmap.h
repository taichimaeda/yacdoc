#ifndef YACJSON_HASHMAP_H
#define YACJSON_HASHMAP_H

typedef struct {
    char *key;
    void *value;
} YacJSONHashMapItem;

typedef struct {
    int capacity;
    int size;
    YacJSONHashMapItem **items;
} YacJSONHashMap;

typedef struct {
    int count;
    int last;
    YacJSONHashMap *map;
} YacJSONHashMapIterator;

typedef void (* YacJSONHashMapValueFreeFunc)(void *);

YacJSONHashMap *yacjson_hashmap_new();
void yacjson_hashmap_free(YacJSONHashMap *map, YacJSONHashMapValueFreeFunc free_func);
bool yacjson_hashmap_add(YacJSONHashMap *map, const char* key, void *value);
void *yacjson_hashmap_get(YacJSONHashMap *map, const char* key);
YacJSONHashMapIterator *yacjson_hashmap_iterator_new(YacJSONHashMap *map);
void yacjson_hashmap_iterator_free(YacJSONHashMapIterator *it);
YacJSONHashMapItem *yacjson_hashmap_iterator_next(YacJSONHashMapIterator *it);
int yacjson_hashmap_iterator_count(YacJSONHashMapIterator *it);

#endif
