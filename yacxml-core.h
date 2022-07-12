#ifndef YACXML_CORE_H
#define YACXML_CORE_H

#include "hashmap.h"

typedef YacDocHashMap YacXMLChildMap;
typedef YacDocHashMap YacXMLAttributeMap;

typedef YacDocHashMapIterator YacXMLChildMapIterator;
typedef YacDocHashMapIterator YacXMLAttributeMapIterator;
typedef YacDocHashMapItem YacXMLChildMapItem;
typedef YacDocHashMapItem YacXMLAttributeMapItem;

typedef struct {
    char *name;
    char *text;
    YacXMLChildMap *children;
    YacXMLAttributeMap *attributes;
} YacXMLElement;

YacXMLAttributeMapIterator *yacxml_attribute_map_iterator_new(YacXMLAttributeMap *attrs);
YacXMLChildMapIterator *yacxml_child_map_iterator_new(YacXMLChildMap *chs);
YacXMLAttributeMapItem *yacxml_attribute_map_iterator_next(YacXMLAttributeMapIterator *it);
YacXMLChildMapItem *yacxml_child_map_iterator_next(YacXMLChildMapIterator *it);

YacXMLElement *yacxml_element_new();
void yacxml_element_free(YacXMLElement *elem);
char *yacxml_element_get_name(YacXMLElement *elem);
void yacxml_element_set_name(YacXMLElement *elem, const char *name);
char *yacxml_element_get_text(YacXMLElement *elem);
void yacxml_element_set_text(YacXMLElement *elem, const char *text);
char *yacxml_element_get_attribute(YacXMLElement *elem, const char *key);
void yacxml_element_add_attribute(YacXMLElement *elem, const char *key, char *value);
YacXMLElement *yacxml_element_get_child(YacXMLElement *elem, const char *key);
void yacxml_element_add_child(YacXMLElement *elem, const char *key, YacXMLElement *value);

YacXMLElement *yacxml_parse(const char *filepath);
void yacxml_serialize(YacXMLElement *elem, const char *filepath);

#endif
