#ifndef YACXML_CORE_H
#define YACXML_CORE_H

#include "hashmap.h"

typedef YacDocHashMap YacXMLChildMap;
typedef YacDocHashMap YacXMLAttributeMap;

typedef struct {
    char *name;
    char *text;
    YacXMLChildMap *chs;
    YacXMLAttributeMap *attrs;
} YacXMLElement;

YacXMLElement *yacxml_parse(const char *filepath);

#endif
