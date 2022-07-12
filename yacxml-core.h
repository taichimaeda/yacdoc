#ifndef YACXML_CORE_H
#define YACXML_CORE_H

#include "hashmap.h"

typedef YacDocHashMap YacXMLChildMap;
typedef YacDocHashMap YacXMLAttributeMap;

typedef struct {
    char *name;
    char *text;
    YacXMLChildMap *children;
    YacXMLAttributeMap *attributes;
} YacXMLElement;

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

#endif
