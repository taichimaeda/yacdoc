#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "yacjson-core.h"

#define YACJSON_MAX_BUFFER_LEN (256)

YacJSONObject *yacjson_object_new() {
    return yacjson_hashmap_new();
}

YacJSONArray *yacjson_array_new() {
    return yacjson_arraylist_new();
}

// TODO
static void yacjson_value_free(void *value) {
    if (yacjson_value_is_primitive(value)) {
        if (yacjson_value_is_string(value)) {
            free(yacjson_value_to_string(value));
        }
        free(yacjson_value_to_primitive(value));
    } else if (yacjson_value_is_object(value)) {
        yacjson_hashmap_free(yacjson_value_to_object(value), yacjson_value_free);
    } else if (yacjson_value_is_array(value)) {
        yacjson_arraylist_free(yacjson_value_to_array(value), yacjson_value_free);
    }
}

void yacjson_object_free(YacJSONObject *object) {
    yacjson_value_free(object);
}

void yacjson_array_free(YacJSONArray *array) {
    yacjson_value_free(array);
}

YacJSONObjectIterator *yacjson_object_iterator_new(YacJSONObject *object) {
    return yacjson_hashmap_iterator_new(object);
}

YacJSONArrayIterator *yacjson_array_iterator_new(YacJSONArray *array) {
    return yacjson_arraylist_iterator_new(array);
}

void yacjson_object_iterator_free(YacJSONObjectIterator *it) {
    yacjson_hashmap_iterator_free(it);
}

void yacjson_array_iterator_free(YacJSONArrayIterator *it) {
    yacjson_arraylist_iterator_free(it);
}

YacJSONObjectItem *yacjson_object_iterator_next(YacJSONObjectIterator *it) {
    return yacjson_hashmap_iterator_next(it);
}

YacJSONArrayItem *yacjson_array_iterator_next(YacJSONArrayIterator *it) {
    return yacjson_arraylist_iterator_next(it);
}

char *yacjson_object_item_key(YacJSONObjectItem *item) {
    return item->key;
}

YacJSONValue *yacjson_object_item_value(YacJSONObjectItem *item) {
    return item->value;
}

YacJSONValue *yacjson_array_item_value(YacJSONArrayItem *item) {
    return item->value;
}

int yacjson_object_iterator_count(YacJSONObjectIterator *it) {
    return yacjson_hashmap_iterator_count(it);
}

int yacjson_array_iterator_count(YacJSONArrayIterator *it) {
    return yacjson_arraylist_iterator_count(it);
}

bool yacjson_value_is_object(YacJSONValue *value) {
    return value->type == OBJECT;
}

bool yacjson_value_is_array(YacJSONValue *value) {
    return value->type == ARRAY;
}

bool yacjson_value_is_primitive(YacJSONValue *value) {
    return value->type == PRIMITIVE;
}

bool yacjson_value_is_boolean(YacJSONValue *value) {
    return yacjson_value_is_primitive(value) && value->data.primitive->type == BOOLEAN;
}

bool yacjson_value_is_integer(YacJSONValue *value) {
    return yacjson_value_is_primitive(value) && value->data.primitive->type == INTEGER;
}

bool yacjson_value_is_decimal(YacJSONValue *value) {
    return yacjson_value_is_primitive(value) && value->data.primitive->type == DECIMAL;
}

bool yacjson_value_is_string(YacJSONValue *value) {
    return yacjson_value_is_primitive(value) && value->data.primitive->type == STRING;
}

bool yacjson_primitive_is_boolean(YacJSONPrimitive *primitive) {
    return primitive->type == BOOLEAN;
}

bool yacjson_primitive_is_integer(YacJSONPrimitive *primitive) {
    return primitive->type == INTEGER;
}

bool yacjson_primitive_is_decimal(YacJSONPrimitive *primitive) {
    return primitive->type == DECIMAL;
}

bool yacjson_primitive_is_string(YacJSONPrimitive *primitive) {
    return primitive->type == STRING;
}

YacJSONObject *yacjson_value_to_object(YacJSONValue *value) {
    return value->data.object;
}

YacJSONArray *yacjson_value_to_array(YacJSONValue *value) {
    return value->data.array;
}

YacJSONPrimitive *yacjson_value_to_primitive(YacJSONValue *value) {
    return value->data.primitive;
}

bool yacjson_value_to_boolean(YacJSONValue *value) {
    return yacjson_value_to_primitive(value)->data.boolean;
}

int yacjson_value_to_integer(YacJSONValue *value) {
    return yacjson_value_to_primitive(value)->data.integer;
}

double yacjson_value_to_decimal(YacJSONValue *value) {
    return yacjson_value_to_primitive(value)->data.decimal;
}

char *yacjson_value_to_string(YacJSONValue *value) {
    return yacjson_value_to_primitive(value)->data.string;
}

bool yacjson_primitive_to_boolean(YacJSONPrimitive *primitive) {
    return primitive->data.boolean;
}

int yacjson_primitive_to_integer(YacJSONPrimitive *primitive) {
    return primitive->data.integer;
}

double yacjson_primitive_to_decimal(YacJSONPrimitive *primitive) {
    return primitive->data.decimal;
}

char *yacjson_primitive_to_string(YacJSONPrimitive *primitive) {
    return primitive->data.string;
}

static YacJSONValue *yacjson_value_new_from_object(YacJSONObject *object) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    value->type = OBJECT;
    value->data.object = object;
    return value;
}

static YacJSONValue *yacjson_value_new_from_array(YacJSONArray *array) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    value->type = ARRAY;
    value->data.array = array;
    return value;
}

static YacJSONValue *yacjson_value_new_from_primitive(YacJSONPrimitive *primitive) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    value->type = PRIMITIVE;
    value->data.primitive = primitive;
    return value;
}

static YacJSONPrimitive *yacjson_primitive_new_from_boolean(bool boolean) {
    YacJSONPrimitive *primitive = malloc(sizeof(YacJSONPrimitive));
    assert(primitive != NULL);
    primitive->type = BOOLEAN;
    primitive->data.boolean = boolean;
    return primitive;
}

static YacJSONPrimitive *yacjson_primitive_new_from_integer(int integer) {
    YacJSONPrimitive *primitive = malloc(sizeof(YacJSONPrimitive));
    assert(primitive != NULL);
    primitive->type = INTEGER;
    primitive->data.integer = integer;
    return primitive;
}

static YacJSONPrimitive *yacjson_primitive_new_from_decimal(double decimal) {
    YacJSONPrimitive *primitive = malloc(sizeof(YacJSONPrimitive));
    assert(primitive != NULL);
    primitive->type = DECIMAL;
    primitive->data.decimal = decimal;
    return primitive;
}

static YacJSONPrimitive *yacjson_primitive_new_from_string(char *string) {
    YacJSONPrimitive *primitive = malloc(sizeof(YacJSONPrimitive));
    assert(primitive != NULL);
    primitive->type = STRING;
    primitive->data.string = malloc((strlen(string) + 1) * sizeof(char));
    assert(primitive->data.string != NULL);
    strcpy(primitive->data.string, string);
    return primitive;
}

int yacjson_object_size(YacJSONObject *object) {
    return object->size;
}

int yacjson_array_size(YacJSONArray *array) {
    return array->size;
}

void yacjson_object_add(YacJSONObject *object, char *key, YacJSONValue *value) {
    yacjson_hashmap_add(object, key, (void *) value);
}

void yacjson_array_add(YacJSONArray *array, YacJSONValue *value) {
    yacjson_arraylist_add(array, (void *) value);
}

void yacjson_object_add_object(YacJSONObject *object, char *key, YacJSONObject *value_object) {
    yacjson_object_add(object, key, yacjson_value_new_from_object(value_object));
}

void yacjson_object_add_array(YacJSONObject *object, char *key, YacJSONArray *value_array) {
    yacjson_object_add(object, key, yacjson_value_new_from_array(value_array));
}

void yacjson_object_add_primitive(YacJSONObject *object, char *key, YacJSONPrimitive *value_primitive) {
    yacjson_object_add(object, key, yacjson_value_new_from_primitive(value_primitive));
}

void yacjson_object_add_boolean(YacJSONObject *object, char *key, bool value_boolean) {
    yacjson_object_add(object, key, yacjson_value_new_from_primitive(yacjson_primitive_new_from_boolean(value_boolean)));
}

void yacjson_object_add_integer(YacJSONObject *object, char *key, int value_integer) {
    yacjson_object_add(object, key, yacjson_value_new_from_primitive(yacjson_primitive_new_from_integer(value_integer)));
}

void yacjson_object_add_decimal(YacJSONObject *object, char *key, double value_decimal) {
    yacjson_object_add(object, key, yacjson_value_new_from_primitive(yacjson_primitive_new_from_decimal(value_decimal)));
}

void yacjson_object_add_string(YacJSONObject *object, char *key, char *value_string) {
    yacjson_object_add(object, key, yacjson_value_new_from_primitive(yacjson_primitive_new_from_string(value_string)));
}

void yacjson_array_add_object(YacJSONArray *array, YacJSONObject *value_object) {
    yacjson_array_add(array, yacjson_value_new_from_object(value_object));
}

void yacjson_array_add_array(YacJSONArray *array, YacJSONArray *value_array) {
    yacjson_array_add(array, yacjson_value_new_from_array(value_array));
}

void yacjson_array_add_primitive(YacJSONArray *array, YacJSONPrimitive *value_primitive) {
    yacjson_array_add(array, yacjson_value_new_from_primitive(value_primitive));
}

void yacjson_array_add_boolean(YacJSONArray *array, bool value_boolean) {
    yacjson_array_add(array, yacjson_value_new_from_primitive(yacjson_primitive_new_from_boolean(value_boolean)));
}

void yacjson_array_add_integer(YacJSONArray *array, int value_integer) {
    yacjson_array_add(array, yacjson_value_new_from_primitive(yacjson_primitive_new_from_integer(value_integer)));
}

void yacjson_array_add_decimal(YacJSONArray *array, double value_decimal) {
    yacjson_array_add(array, yacjson_value_new_from_primitive(yacjson_primitive_new_from_decimal(value_decimal)));
}

void yacjson_array_add_string(YacJSONArray *array, char *value_string) {
    yacjson_array_add(array, yacjson_value_new_from_primitive(yacjson_primitive_new_from_string(value_string)));
}

YacJSONValue *yacjson_object_get(YacJSONObject *object, const char *key) {
    return yacjson_hashmap_get(object, key);
}

YacJSONValue *yacjson_array_get(YacJSONArray *array, int index) {
    return yacjson_arraylist_get(array, index);
}

YacJSONObject *yacjson_object_get_object(YacJSONObject *object, const char *key) {
    return yacjson_value_to_object(yacjson_object_get(object, key));
}

YacJSONArray *yacjson_object_get_array(YacJSONObject *object, const char *key) {
    return yacjson_value_to_array(yacjson_object_get(object, key));
}

YacJSONPrimitive *yacjson_object_get_primitive(YacJSONObject *object, const char *key) {
    return yacjson_value_to_primitive(yacjson_object_get(object, key));
}

bool yacjson_object_get_boolean(YacJSONObject *object, const char *key) {
    return yacjson_value_to_boolean(yacjson_object_get(object, key));
}

int yacjson_object_get_integer(YacJSONObject *object, const char *key) {
    return yacjson_value_to_integer(yacjson_object_get(object, key));
}

double yacjson_object_get_decimal(YacJSONObject *object, const char *key) {
    return yacjson_value_to_decimal(yacjson_object_get(object, key));
}

char *yacjson_object_get_string(YacJSONObject *object, const char *key) {
    return yacjson_value_to_string(yacjson_object_get(object, key));
}

YacJSONObject *yacjson_array_get_object(YacJSONArray *array, int index) {
    return yacjson_value_to_object(yacjson_array_get(array, index));
}

YacJSONArray *yacjson_array_get_array(YacJSONArray *array, int index) {
    return yacjson_value_to_array(yacjson_array_get(array, index));
}

YacJSONPrimitive *yacjson_array_get_primitive(YacJSONArray *array, int index) {
    return yacjson_value_to_primitive(yacjson_array_get(array, index));
}

bool yacjson_array_get_boolean(YacJSONArray *array, int index) {
    return yacjson_value_to_boolean(yacjson_array_get(array, index));
}

int yacjson_array_get_integer(YacJSONArray *array, int index) {
    return yacjson_value_to_integer(yacjson_array_get(array, index));
}

double yacjson_array_get_decimal(YacJSONArray *array, int index) {
    return yacjson_value_to_decimal(yacjson_array_get(array, index));
}

char *yacjson_array_get_string(YacJSONArray *array, int index) {
    return yacjson_value_to_string(yacjson_array_get(array, index));
}

static YacJSONPrimitive *yacjson_parse_primitive_from_string(char *value_string) {
    double value_decimal;
    int value_integer;
    if (!strcmp(value_string, "true") || !strcmp(value_string, "false")) {
        bool value_boolean = !strcmp(value_string, "true") ? true : false;
        return yacjson_primitive_new_from_boolean(value_boolean);
    } else if (sscanf(value_string, "%d", &value_integer) == 1) {
        return yacjson_primitive_new_from_integer(value_integer);
    } else if (sscanf(value_string, "%lf", &value_decimal) == 1){
        return yacjson_primitive_new_from_decimal(value_decimal);
    } else {
        return yacjson_primitive_new_from_string(strtok(value_string, "\""));
    }
}

static YacJSONObject *yacjson_parse_object_from_file(FILE *file);
static YacJSONArray *yacjson_parse_array_from_file(FILE *file);

static YacJSONObject *yacjson_parse_object_from_file(FILE *file) {
    YacJSONObject *object = yacjson_object_new();
    int pos = 0;
    char prev, curr;
    char buffer[YACJSON_MAX_BUFFER_LEN];
    char key[YACJSON_MAX_BUFFER_LEN];
    bool in_value = false;
    bool in_string = false;
    bool in_comment = false;
    bool is_value_added = false;
    while ((curr = fgetc(file)) != EOF) {
        if (curr == '\t') continue;
        if (curr == '\n') { in_comment = false; continue; }
        if (curr == '/' && prev == '/') { in_comment = true; continue; }
        if (in_comment) continue;
        if (curr == '"') in_string = !in_string || prev == '\\';
        if (curr == ' ' && !in_string) continue; 
        if (curr == ':' && !in_string) {
            buffer[pos] = '\0';
            strcpy(key, strtok(buffer, "\""));
            pos = 0; in_value = true;
            continue;
        }
        if (curr == '{' && !in_string && in_value){
            yacjson_object_add_object(object, key, yacjson_parse_object_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == '[' && !in_string && in_value) {
            yacjson_object_add_array(object, key, yacjson_parse_array_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if ((curr == ',' || curr == '}') && !in_string && in_value) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_object_add_primitive(object, key, yacjson_parse_primitive_from_string(buffer));
                pos = 0; in_value = false;
            }
            if (curr == ',') continue;
            if (curr == '}') break;
        }
        buffer[pos++] = curr;
        prev = curr;
    }
    return object;
}

static YacJSONArray *yacjson_parse_array_from_file(FILE *file) {
    YacJSONArray *array = yacjson_array_new();
    int pos = 0;
    char prev, curr;
    char buffer[YACJSON_MAX_BUFFER_LEN];
    bool in_string = false;
    bool in_comment = false;
    bool is_value_added = false;
    while ((curr = fgetc(file)) != EOF) {
        if (curr == '\t') continue;
        if (curr == '\n') { in_comment = false; continue; }
        if (curr == '/' && prev == '/') { in_comment = true; continue; }
        if (in_comment) continue;
        if (curr == '"') in_string = !in_string || prev == '\\';
        if (curr == ' ' && !in_string) continue; 
        if (curr == '{' && !in_string){
            yacjson_array_add_object(array, yacjson_parse_object_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == '[' && !in_string) {
            yacjson_array_add_array(array, yacjson_parse_array_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if ((curr == ',' || curr == ']') && !in_string) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_array_add_primitive(array, yacjson_parse_primitive_from_string(buffer));
                pos = 0;
            }
            if (curr == ',') continue;
            if (curr == ']') break;
            break;
        }
        buffer[pos++] = curr;
        prev = curr;
    }
    return array;
}

YacJSONValue *yacjson_parse(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    assert(file != NULL);
    char prev, curr;
    bool in_comment = false;
    while ((curr = fgetc(file)) != EOF) {
        if (curr == '\n') in_comment = false;
        if (prev == '/' && curr == '/') in_comment = true;
        if (in_comment) continue;
        if (curr == '{') {
            // Top level is object
            YacJSONObject *object = yacjson_parse_object_from_file(file); fclose(file);
            return yacjson_value_new_from_object(object);
        }
        if (curr == '[') {
            // Top level is array
            YacJSONArray *array = yacjson_parse_array_from_file(file); fclose(file);
            return yacjson_value_new_from_array(array);
        }
        prev = curr;
    }
    return NULL;
}

static void yacjson_serialize_to_file(YacJSONValue *value, FILE *file, int depth) {
    if (yacjson_value_is_primitive(value)) {
        if (yacjson_value_is_boolean(value)) {
            fprintf(file, "%s", yacjson_value_to_boolean(value) ? "true" : "false");
        } else if (yacjson_value_is_integer(value)) {
            fprintf(file, "%d", yacjson_value_to_integer(value));
        } else if (yacjson_value_is_decimal(value)) {
            fprintf(file, "%lf", yacjson_value_to_decimal(value));
        } else if (yacjson_value_is_string(value)) {
            fputc('"', file);
            fputs(yacjson_value_to_string(value), file);
            fputc('"', file);
        }
    } else if (yacjson_value_is_object(value)) {
        fputs("{\n", file);
        YacJSONObjectItem *item;
        YacJSONObject *object = yacjson_value_to_object(value);
        YacJSONObjectIterator *it = yacjson_object_iterator_new(object);
        while ((item = yacjson_object_iterator_next(it)) != NULL) {
            for (int i = 0; i < depth; i++) fputc('\t', file);
            fputc('"', file);
            fputs(item->key, file);
            fputs("\": ", file);
            yacjson_serialize_to_file(item->value, file, depth + 1);
            if (yacjson_object_iterator_count(it) < yacjson_object_size(object)) fputc(',', file);
            fputc('\n', file);
        }
        for (int i = 0; i < depth - 1; i++) fputc('\t', file);
        fputc('}', file);
    } else if (yacjson_value_is_array(value)) {
        fputs("[\n", file);
        YacJSONArrayItem *item;
        YacJSONArray *array = yacjson_value_to_array(value);
        YacJSONArrayIterator *it = yacjson_array_iterator_new(array);
        while ((item = yacjson_array_iterator_next(it)) != NULL) {
            for (int i = 0; i < depth; i++) fputc('\t', file);
            yacjson_serialize_to_file(item->value, file, depth + 1);
            if (yacjson_array_iterator_count(it) < yacjson_array_size(array)) fputc(',', file);
            fputc('\n', file);
        }
        for (int i = 0; i < depth - 1; i++) fputc('\t', file);
        fputc(']', file);
    }
}

void yacjson_serialize(YacJSONValue *value, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    yacjson_serialize_to_file(value, file, 1);
    // End file with a new line char
    fputc('\n', file);
    fclose(file);
}
