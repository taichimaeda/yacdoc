#ifndef YACJSON_CORE_H
#define YACJSON_CORE_H

#include <stdbool.h>

#include "arraylist.h"
#include "hashmap.h"

typedef YacDocHashMap YacJSONObject;
typedef YacDocArrayList YacJSONArray;

typedef YacDocHashMapIterator YacJSONObjectIterator;
typedef YacDocArrayListIterator YacJSONArrayIterator;
typedef YacDocHashMapItem YacJSONObjectItem;
typedef YacDocArrayListItem YacJSONArrayItem;

typedef struct {
    enum {
        YACJSON_BOOLEAN,
        YACJSON_INTEGER,
        YACJSON_DECIMAL,
        YACJSON_STRING,
    } type;
    union {
        bool boolean;
        int integer;
        double decimal;
        char *string;
    } data;
} YacJSONPrimitive;

typedef struct {
    enum {
        YACJSON_OBJECT,
        YACJSON_ARRAY,
        YACJSON_PRIMITIVE,
    } type;
    union {
        YacJSONObject *object;
        YacJSONArray *array;
        YacJSONPrimitive *primitive;
    } data;
} YacJSONValue;

YacJSONObject *yacjson_object_new();
YacJSONArray *yacjson_array_new();
void yacjson_object_free(YacJSONObject *object);
void yacjson_array_free(YacJSONArray *array);

YacJSONObjectIterator *yacjson_object_iterator_new(YacJSONObject *object);
YacJSONArrayIterator *yacjson_array_iterator_new(YacJSONArray *array);
void yacjson_object_iterator_free(YacJSONObjectIterator *it);
void yacjson_array_iterator_free(YacJSONArrayIterator *it);
YacJSONObjectItem *yacjson_object_iterator_next(YacJSONObjectIterator *it);
YacJSONArrayItem *yacjson_array_iterator_next(YacJSONArrayIterator *it);
char *yacjson_object_item_key(YacJSONObjectItem *item);
YacJSONValue *yacjson_object_item_value(YacJSONObjectItem *item);
YacJSONValue *yacjson_array_item_value(YacJSONArrayItem *item);
int yacjson_object_iterator_count(YacJSONObjectIterator *it);
int yacjson_array_iterator_count(YacJSONArrayIterator *it);

bool yacjson_value_is_object(YacJSONValue *value);
bool yacjson_value_is_array(YacJSONValue *value);
bool yacjson_value_is_primitive(YacJSONValue *value);
bool yacjson_value_is_boolean(YacJSONValue *value);
bool yacjson_value_is_integer(YacJSONValue *value);
bool yacjson_value_is_decimal(YacJSONValue *value);
bool yacjson_value_is_string(YacJSONValue *value);
bool yacjson_primitive_is_boolean(YacJSONPrimitive *primitive);
bool yacjson_primitive_is_integer(YacJSONPrimitive *primitive);
bool yacjson_primitive_is_decimal(YacJSONPrimitive *primitive);
bool yacjson_primitive_is_string(YacJSONPrimitive *primitive);

YacJSONObject *yacjson_value_to_object(YacJSONValue *value);
YacJSONArray *yacjson_value_to_array(YacJSONValue *value);
YacJSONPrimitive *yacjson_value_to_primitive(YacJSONValue *value);
bool yacjson_value_to_boolean(YacJSONValue *value);
int yacjson_value_to_integer(YacJSONValue *value);
double yacjson_value_to_decimal(YacJSONValue *value);
char *yacjson_value_to_string(YacJSONValue *value);
bool yacjson_primitive_to_boolean(YacJSONPrimitive *primitive);
int yacjson_primitive_to_integer(YacJSONPrimitive *primitive);
double yacjson_primitive_to_decimal(YacJSONPrimitive *primitive);
char *yacjson_primitive_to_string(YacJSONPrimitive *primitive);

int yacjson_object_size(YacJSONObject *object);
int yacjson_array_size(YacJSONArray *array);

void yacjson_object_add(YacJSONObject *object, char *key, YacJSONValue *value);
void yacjson_array_add(YacJSONArray *array, YacJSONValue *value);
void yacjson_object_add_object(YacJSONObject *object, char *key, YacJSONObject *value_object);
void yacjson_object_add_array(YacJSONObject *object, char *key, YacJSONArray *value_array);
void yacjson_object_add_primitive(YacJSONObject *object, char *key, YacJSONPrimitive *value_primitive);
void yacjson_object_add_boolean(YacJSONObject *object, char *key, bool value_boolean);
void yacjson_object_add_integer(YacJSONObject *object, char *key, int value_integer);
void yacjson_object_add_decimal(YacJSONObject *object, char *key, double value_decimal);
void yacjson_object_add_string(YacJSONObject *object, char *key, char *value_string);
void yacjson_array_add_object(YacJSONArray *array, YacJSONObject *value_object);
void yacjson_array_add_array(YacJSONArray *array, YacJSONArray *value_array);
void yacjson_array_add_primitive(YacJSONArray *array, YacJSONPrimitive *value_primitive);
void yacjson_array_add_boolean(YacJSONArray *array, bool value_boolean);
void yacjson_array_add_integer(YacJSONArray *array, int value_integer);
void yacjson_array_add_decimal(YacJSONArray *array, double value_decimal);
void yacjson_array_add_string(YacJSONArray *array, char *value_string);

YacJSONValue *yacjson_object_get(YacJSONObject *object, const char *key);
YacJSONValue *yacjson_array_get(YacJSONArray *array, int index);
YacJSONObject *yacjson_object_get_object(YacJSONObject *object, const char *key);
YacJSONArray *yacjson_object_get_array(YacJSONObject *object, const char *key);
YacJSONPrimitive *yacjson_object_get_primitive(YacJSONObject *object, const char *key);
bool yacjson_object_get_boolean(YacJSONObject *object, const char *key);
int yacjson_object_get_integer(YacJSONObject *object, const char *key);
double yacjson_object_get_decimal(YacJSONObject *object, const char *key);
char *yacjson_object_get_string(YacJSONObject *object, const char *key);
YacJSONObject *yacjson_array_get_object(YacJSONArray *array, int index);
YacJSONArray *yacjson_array_get_array(YacJSONArray *array, int index);
YacJSONPrimitive *yacjson_array_get_primitive(YacJSONArray *array, int index);
bool yacjson_array_get_boolean(YacJSONArray *array, int index);
int yacjson_array_get_integer(YacJSONArray *array, int index);
double yacjson_array_get_decimal(YacJSONArray *array, int index);
char *yacjson_array_get_string(YacJSONArray *array, int index);

YacJSONValue *yacjson_parse(const char *filepath);
void yacjson_serialize(YacJSONValue *value, const char *filepath);

#endif
