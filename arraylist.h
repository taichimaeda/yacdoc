#ifndef YACJSON_ARRAYLIST_H
#define YACJSON_ARRAYLIST_H

#include <stdbool.h>

typedef struct {
    void *value;
} YacJSONArrayListItem;

typedef struct {
    int capacity;
    int size;
    YacJSONArrayListItem **items;
} YacJSONArrayList;

typedef struct {
    int count;
    int last;
    YacJSONArrayList *list;
} YacJSONArrayListIterator;

typedef void (* YacJSONArrayListValueFreeFunc)(void *);

YacJSONArrayList *yacjson_arraylist_new();
void yacjson_arraylist_free(YacJSONArrayList *list, YacJSONArrayListValueFreeFunc free_func);
void yacjson_arraylist_add(YacJSONArrayList *list, void *value);
void *yacjson_arraylist_get(YacJSONArrayList *list, int index);
YacJSONArrayListIterator *yacjson_arraylist_iterator_new(YacJSONArrayList *list);
void yacjson_arraylist_iterator_free(YacJSONArrayListIterator *it);
YacJSONArrayListItem *yacjson_arraylist_iterator_next(YacJSONArrayListIterator *it);
int yacjson_arraylist_iterator_count(YacJSONArrayListIterator *it);

#endif
