#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "hashmap.h"

#define YACJSON_HASHMAP_INITIAL_CAPACITY (64)

YacJSONHashMap *yacjson_hashmap_new() {
    YacJSONHashMap *map = malloc(sizeof(YacJSONHashMap));
    assert(map != NULL);
    map->capacity = YACJSON_HASHMAP_INITIAL_CAPACITY;
    map->size = 0;
    map->items = calloc(map->capacity, sizeof(YacJSONHashMapItem *));
    assert(map->items != NULL);
    return map;
}

void yacjson_hashmap_free(YacJSONHashMap *map, YacJSONHashMapValueFreeFunc free_func) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i] == NULL) continue;
        free(map->items[i]->key);
        free_func(map->items[i]->value);
        free(map->items[i]);
    }
    free(map->items);
    free(map);
}

static int yacjson_positive_mod(int value, int mod) {
    return (value % mod + mod) % mod;
}

static int yacjson_djb2_hash(const char *str) {
    int hash = 5381;
    char *ch_ptr = (char *) str;
    while (*ch_ptr != '\0') {
        hash = ((hash << 5) + hash) + *ch_ptr;
        ch_ptr++;
    }
    return hash;
}

static void yacjson_hashmap_resize(YacJSONHashMap *map) {
    YacJSONHashMapItem **old_items = map->items;
    map->capacity *= 2;
    map->items = calloc(map->capacity, sizeof(YacJSONHashMap *));
    assert(map->items != NULL);
    for (int i = 0; i < map->size; i++) {
        yacjson_hashmap_add(map, old_items[i]->key, old_items[i]->value);
    }
}

bool yacjson_hashmap_add(YacJSONHashMap *map, const char* key, void *value) {
    if (map->size == map->capacity) {
        yacjson_hashmap_resize(map);
    }
    int hash = yacjson_djb2_hash(key);
    int index = yacjson_positive_mod(hash, map->capacity);
    while (map->items[index] != NULL) {
        if (!strcmp(map->items[index]->key, key)) {
            return false;
        }
        index++;
        index %= map->capacity;
    }
    YacJSONHashMapItem *item = malloc(sizeof(YacJSONHashMapItem));
    assert(item != NULL);
    item->key = malloc((strlen(key) + 1) * sizeof(char));
    assert(item->key != NULL);
    strcpy(item->key, key);
    item->value = value;
    map->items[index] = item;
    map->size++;
    return true;
}

void *yacjson_hashmap_get(YacJSONHashMap *map, const char* key) {
    int hash = yacjson_djb2_hash(key);
    int index = yacjson_positive_mod(hash, map->capacity);
    int count = 0;
    while (map->items[index] != NULL && count < map->capacity) {
        if (!strcmp(map->items[index]->key ,key)) {
            return map->items[index]->value;
        }
        index++;
        index %= map->capacity;
        count++;
    }
    return NULL;
}

YacJSONHashMapIterator *yacjson_hashmap_iterator_new(YacJSONHashMap *map) {
    YacJSONHashMapIterator *it = malloc(sizeof(YacJSONHashMapIterator));
    assert(it != NULL);
    it->count = 0;
    it->last = -1;
    it->map = map;
    return it;
}

void yacjson_hashmap_iterator_free(YacJSONHashMapIterator *it) {
    free(it);
}

YacJSONHashMapItem *yacjson_hashmap_iterator_next(YacJSONHashMapIterator *it) {
    YacJSONHashMapItem *item;
    for (int i = it->last + 1; i < it->map->capacity; i++) {
        if ((item = it->map->items[i]) != NULL) {
            it->count++;
            it->last = i;
            return item;
        }
    }
    return NULL;
}

int yacjson_hashmap_iterator_count(YacJSONHashMapIterator *it) {
    return it->count;
}
