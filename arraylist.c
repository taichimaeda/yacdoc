#include <stdlib.h>
#include <assert.h>

#include "arraylist.h"

#define YACDOC_ARRAYLIST_INITIAL_CAPACITY (64)

YacDocArrayList *yacdoc_arraylist_new() {
    YacDocArrayList *list = malloc(sizeof(YacDocArrayList));
    assert(list != NULL);
    list->capacity = YACDOC_ARRAYLIST_INITIAL_CAPACITY;
    list->size = 0;
    list->items = calloc(list->capacity, sizeof(YacDocArrayListItem *));
    assert(list->items != NULL);
    return list;
}

void yacdoc_arraylist_free(YacDocArrayList *list, YacDocArrayListValueFreeFunc free_func) {
    for (int i = 0; i < list->capacity; i++) {
        if (list->items[i] == NULL) continue;
        free_func(list->items[i]->value);
        free(list->items[i]);
    }
    free(list);
}

static void yacdoc_list_resize(YacDocArrayList *list) {
    list->capacity *= 2;
    list->items = realloc(list->items, list->capacity * sizeof(YacDocArrayListItem *));
    assert(list->items != NULL);
}

void yacdoc_arraylist_add(YacDocArrayList *list, void *value) {
    if (list->size == list->capacity) {
        yacdoc_list_resize(list);
    }
    YacDocArrayListItem *item = malloc(sizeof(YacDocArrayListItem));
    assert(item != NULL);
    item->value = value;
    list->items[list->size] = item;
    list->size++;
}

void *yacdoc_arraylist_get(YacDocArrayList *list, int index) {
    if (list->items[index] != NULL) {
        return list->items[index]->value;    
    }
    return NULL;
}

YacDocArrayListIterator *yacdoc_arraylist_iterator_new(YacDocArrayList *list) {
    YacDocArrayListIterator *it = malloc(sizeof(YacDocArrayListIterator));
    assert(it != NULL);
    it->count = 0;
    it->last = -1;
    it->list = list;
    return it;
}

void yacdoc_arraylist_iterator_free(YacDocArrayListIterator *it) {
    free(it);
}

YacDocArrayListItem *yacdoc_arraylist_iterator_next(YacDocArrayListIterator *it) {
    if (it->last == it->list->size - 1) {
        return NULL;
    }
    it->count++;
    it->last++;
    return it->list->items[it->last];
}

int yacdoc_arraylist_iterator_count(YacDocArrayListIterator *it) {
    return it->count;
}
