#ifndef YACDOC_ARRAYLIST_H
#define YACDOC_ARRAYLIST_H

#include <stdbool.h>

typedef struct {
    void *value;
} YacDocArrayListItem;

typedef struct {
    int capacity;
    int size;
    YacDocArrayListItem **items;
} YacDocArrayList;

typedef struct {
    int count;
    int last;
    YacDocArrayList *list;
} YacDocArrayListIterator;

typedef void (* YacDocArrayListValueFreeFunc)(void *);

YacDocArrayList *yacdoc_arraylist_new();
void yacdoc_arraylist_free(YacDocArrayList *list, YacDocArrayListValueFreeFunc free_func);
void yacdoc_arraylist_add(YacDocArrayList *list, void *value);
void *yacdoc_arraylist_get(YacDocArrayList *list, int index);
YacDocArrayListIterator *yacdoc_arraylist_iterator_new(YacDocArrayList *list);
void yacdoc_arraylist_iterator_free(YacDocArrayListIterator *it);
YacDocArrayListItem *yacdoc_arraylist_iterator_next(YacDocArrayListIterator *it);
int yacdoc_arraylist_iterator_count(YacDocArrayListIterator *it);

#endif
