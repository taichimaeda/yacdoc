#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

#define YACDOC_HASHMAP_INITIAL_CAPACITY (64)

YacDocHashMap *yacdoc_hashmap_new() {
    YacDocHashMap *map = malloc(sizeof(YacDocHashMap));
    assert(map != NULL);
    map->capacity = YACDOC_HASHMAP_INITIAL_CAPACITY;
    map->size = 0;
    map->items = calloc(map->capacity, sizeof(YacDocHashMapItem *));
    assert(map->items != NULL);
    return map;
}

void yacdoc_hashmap_free(YacDocHashMap *map, YacDocHashMapValueFreeFunc free_func) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i] == NULL) continue;
        free(map->items[i]->key);
        free_func(map->items[i]->value);
        free(map->items[i]);
    }
    free(map->items);
    free(map);
}

static int yacdoc_positive_mod(int value, int mod) {
    return (value % mod + mod) % mod;
}

static int yacdoc_djb2_hash(const char *str) {
    int hash = 5381;
    char *ch_ptr = (char *) str;
    while (*ch_ptr != '\0') {
        hash = ((hash << 5) + hash) + *ch_ptr;
        ch_ptr++;
    }
    return hash;
}

static void yacdoc_hashmap_resize(YacDocHashMap *map) {
    YacDocHashMapItem **old_items = map->items;
    map->capacity *= 2;
    map->items = calloc(map->capacity, sizeof(YacDocHashMap *));
    assert(map->items != NULL);
    for (int i = 0; i < map->size; i++) {
        yacdoc_hashmap_add(map, old_items[i]->key, old_items[i]->value);
    }
}

bool yacdoc_hashmap_add(YacDocHashMap *map, const char *key, void *value) {
    if (map->size == map->capacity) {
        yacdoc_hashmap_resize(map);
    }
    int hash = yacdoc_djb2_hash(key);
    int index = yacdoc_positive_mod(hash, map->capacity);
    while (map->items[index] != NULL) {
        if (!strcmp(map->items[index]->key, key)) {
            return false;
        }
        index++;
        index %= map->capacity;
    }
    YacDocHashMapItem *item = malloc(sizeof(YacDocHashMapItem));
    assert(item != NULL);
    item->key = malloc(strlen(key) + 1);
    assert(item->key != NULL);
    strcpy(item->key, key);
    item->value = value;
    map->items[index] = item;
    map->size++;
    return true;
}

void *yacdoc_hashmap_get(YacDocHashMap *map, const char *key) {
    int hash = yacdoc_djb2_hash(key);
    int index = yacdoc_positive_mod(hash, map->capacity);
    int count = 0;
    while (map->items[index] != NULL && count < map->capacity) {
        if (!strcmp(map->items[index]->key, key)) {
            return map->items[index]->value;
        }
        index++;
        index %= map->capacity;
        count++;
    }
    return NULL;
}

YacDocHashMapIterator *yacdoc_hashmap_iterator_new(YacDocHashMap *map) {
    YacDocHashMapIterator *it = malloc(sizeof(YacDocHashMapIterator));
    assert(it != NULL);
    it->count = 0;
    it->last = -1;
    it->map = map;
    return it;
}

void yacdoc_hashmap_iterator_free(YacDocHashMapIterator *it) {
    free(it);
}

YacDocHashMapItem *yacdoc_hashmap_iterator_next(YacDocHashMapIterator *it) {
    YacDocHashMapItem *item;
    for (int i = it->last + 1; i < it->map->capacity; i++) {
        if ((item = it->map->items[i]) != NULL) {
            it->count++;
            it->last = i;
            return item;
        }
    }
    return NULL;
}

int yacdoc_hashmap_iterator_count(YacDocHashMapIterator *it) {
    return it->count;
}
