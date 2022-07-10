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

typedef enum {
    YACJSON_OBJECT,
    YACJSON_ARRAY,
    YACJSON_BOOLEAN,
    YACJSON_INTEGER,
    YACJSON_DECIMAL,
    YACJSON_STRING,
} YacJSONValueType;

typedef struct {
    YacJSONValueType type;
    union {
        YacJSONObject *object;
        YacJSONArray *array;
        bool boolean;
        long integer;
        double decimal;
        char *string;
    } data;
} YacJSONValue;

YacJSONObject *yacjson_object_new();
YacJSONArray *yacjson_array_new();
void yacjson_value_free(YacJSONValue *value);

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
bool yacjson_value_is_boolean(YacJSONValue *value);
bool yacjson_value_is_integer(YacJSONValue *value);
bool yacjson_value_is_decimal(YacJSONValue *value);
bool yacjson_value_is_string(YacJSONValue *value);

YacJSONObject *yacjson_value_to_object(YacJSONValue *value);
YacJSONArray *yacjson_value_to_array(YacJSONValue *value);
bool yacjson_value_to_boolean(YacJSONValue *value);
long yacjson_value_to_integer(YacJSONValue *value);
double yacjson_value_to_decimal(YacJSONValue *value);
char *yacjson_value_to_string(YacJSONValue *value);

YacJSONValue *yacjson_value_from_object(YacJSONObject *object);
YacJSONValue *yacjson_value_from_array(YacJSONArray *array);
YacJSONValue *yacjson_value_from_boolean(bool boolean);
YacJSONValue *yacjson_value_from_integer(long integer);
YacJSONValue *yacjson_value_from_decimal(double decimal);
YacJSONValue *yacjson_value_from_string(char *string);

int yacjson_object_size(YacJSONObject *object);
int yacjson_array_size(YacJSONArray *array);

void yacjson_object_add(YacJSONObject *object, char *key, YacJSONValue *value);
void yacjson_array_add(YacJSONArray *array, YacJSONValue *value);
void yacjson_object_add_object(YacJSONObject *object, char *key, YacJSONObject *value_object);
void yacjson_object_add_array(YacJSONObject *object, char *key, YacJSONArray *value_array);
void yacjson_object_add_boolean(YacJSONObject *object, char *key, bool value_boolean);
void yacjson_object_add_integer(YacJSONObject *object, char *key, long value_integer);
void yacjson_object_add_decimal(YacJSONObject *object, char *key, double value_decimal);
void yacjson_object_add_string(YacJSONObject *object, char *key, char *value_string);
void yacjson_array_add_object(YacJSONArray *array, YacJSONObject *value_object);
void yacjson_array_add_array(YacJSONArray *array, YacJSONArray *value_array);
void yacjson_array_add_boolean(YacJSONArray *array, bool value_boolean);
void yacjson_array_add_integer(YacJSONArray *array, long value_integer);
void yacjson_array_add_decimal(YacJSONArray *array, double value_decimal);
void yacjson_array_add_string(YacJSONArray *array, char *value_string);

YacJSONValue *yacjson_object_get(YacJSONObject *object, const char *key);
YacJSONValue *yacjson_array_get(YacJSONArray *array, int index);
YacJSONObject *yacjson_object_get_object(YacJSONObject *object, const char *key);
YacJSONArray *yacjson_object_get_array(YacJSONObject *object, const char *key);
bool yacjson_object_get_boolean(YacJSONObject *object, const char *key);
long yacjson_object_get_integer(YacJSONObject *object, const char *key);
double yacjson_object_get_decimal(YacJSONObject *object, const char *key);
char *yacjson_object_get_string(YacJSONObject *object, const char *key);
YacJSONObject *yacjson_array_get_object(YacJSONArray *array, int index);
YacJSONArray *yacjson_array_get_array(YacJSONArray *array, int index);
bool yacjson_array_get_boolean(YacJSONArray *array, int index);
long yacjson_array_get_integer(YacJSONArray *array, int index);
double yacjson_array_get_decimal(YacJSONArray *array, int index);
char *yacjson_array_get_string(YacJSONArray *array, int index);

YacJSONValue *yacjson_parse(const char *filepath);
void yacjson_serialize(YacJSONValue *value, const char *filepath);

#endif
