#ifndef YACXML_CORE_H
#define YACXML_CORE_H

#include "hashmap.h"

typedef YacDocHashMapItem YacXMLAttribute;
typedef YacDocHashMap YacXMLAttributes;

typedef struct {
    char *name;
    char *content;
    YacXMLAttributes *attrs;
} YacXMLElement;

#endif
