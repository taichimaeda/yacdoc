#include <stdlib.h>
#include <assert.h>

#include "arraylist.h"

#define YACJSON_ARRAYLIST_INITIAL_CAPACITY (64)

YacJSONArrayList *yacjson_arraylist_new() {
    YacJSONArrayList *list = malloc(sizeof(YacJSONArrayList));
    assert(list != NULL);
    list->capacity = YACJSON_ARRAYLIST_INITIAL_CAPACITY;
    list->size = 0;
    list->items = calloc(list->capacity, sizeof(YacJSONArrayListItem *));
    assert(list->items != NULL);
    return list;
}

void yacjson_arraylist_free(YacJSONArrayList *list, YacJSONArrayListValueFreeFunc free_func) {
    for (int i = 0; i < list->capacity; i++) {
        if (list->items[i] == NULL) continue;
        free_func(list->items[i]->value);
        free(list->items[i]);
    }
    free(list);
}

static void yacjson_list_resize(YacJSONArrayList *list) {
    list->capacity *= 2;
    list->items = realloc(list->items, list->capacity * sizeof(YacJSONArrayListItem *));
    assert(list->items != NULL);
}

void yacjson_arraylist_add(YacJSONArrayList *list, void *value) {
    if (list->size == list->capacity) {
        yacjson_list_resize(list);
    }
    YacJSONArrayListItem *item = malloc(sizeof(YacJSONArrayListItem));
    assert(item != NULL);
    item->value = value;
    list->items[list->size] = item;
    list->size++;
}

void *yacjson_arraylist_get(YacJSONArrayList *list, int index) {
    if (list->items[index] != NULL) {
        return list->items[index]->value;    
    }
    return NULL;
}

YacJSONArrayListIterator *yacjson_arraylist_iterator_new(YacJSONArrayList *list) {
    YacJSONArrayListIterator *it = malloc(sizeof(YacJSONArrayListIterator));
    assert(it != NULL);
    it->count = 0;
    it->last = -1;
    it->list = list;
    return it;
}

void yacjson_arraylist_iterator_free(YacJSONArrayListIterator *it) {
    free(it);
}

YacJSONArrayListItem *yacjson_arraylist_iterator_next(YacJSONArrayListIterator *it) {
    if (it->last == it->list->size - 1) {
        return NULL;
    }
    it->count++;
    it->last++;
    return it->list->items[it->last];
}

int yacjson_arraylist_iterator_count(YacJSONArrayListIterator *it) {
    return it->count;
}
