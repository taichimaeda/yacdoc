#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "yacjson-core.h"

#define YACJSON_MAX_BUFFER_LEN (256)

YacJSONObject *yacjson_object_new() {
    return yacdoc_hashmap_new();
}

YacJSONArray *yacjson_array_new() {
    return yacdoc_arraylist_new();
}

static void yacjson_value_free_void(void *value) {
    if (yacjson_value_is_string(value)) {
        free(yacjson_value_to_string(value));
    } else if (yacjson_value_is_object(value)) {
        yacdoc_hashmap_free(yacjson_value_to_object(value), yacjson_value_free_void);
    } else if (yacjson_value_is_array(value)) {
        yacdoc_arraylist_free(yacjson_value_to_array(value), yacjson_value_free_void);
    }
    free(value);
}

void yacjson_value_free(YacJSONValue *value) {
    yacjson_value_free_void((void *) value);
}

YacJSONObjectIterator *yacjson_object_iterator_new(YacJSONObject *object) {
    return yacdoc_hashmap_iterator_new(object);
}

YacJSONArrayIterator *yacjson_array_iterator_new(YacJSONArray *array) {
    return yacdoc_arraylist_iterator_new(array);
}

void yacjson_object_iterator_free(YacJSONObjectIterator *it) {
    yacdoc_hashmap_iterator_free(it);
}

void yacjson_array_iterator_free(YacJSONArrayIterator *it) {
    yacdoc_arraylist_iterator_free(it);
}

YacJSONObjectItem *yacjson_object_iterator_next(YacJSONObjectIterator *it) {
    return yacdoc_hashmap_iterator_next(it);
}

YacJSONArrayItem *yacjson_array_iterator_next(YacJSONArrayIterator *it) {
    return yacdoc_arraylist_iterator_next(it);
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
    return yacdoc_hashmap_iterator_count(it);
}

int yacjson_array_iterator_count(YacJSONArrayIterator *it) {
    return yacdoc_arraylist_iterator_count(it);
}

bool yacjson_value_is_object(YacJSONValue *value) {
    return value->type == YACJSON_OBJECT;
}

bool yacjson_value_is_array(YacJSONValue *value) {
    return value->type == YACJSON_ARRAY;
}

bool yacjson_value_is_boolean(YacJSONValue *value) {
    return value->type == YACJSON_BOOLEAN;
}

bool yacjson_value_is_integer(YacJSONValue *value) {
    return value->type == YACJSON_INTEGER;
}

bool yacjson_value_is_decimal(YacJSONValue *value) {
    return value->type == YACJSON_DECIMAL;
}

bool yacjson_value_is_string(YacJSONValue *value) {
    return value->type == YACJSON_STRING;
}

YacJSONObject *yacjson_value_to_object(YacJSONValue *value) {
    return value->data.object;
}

YacJSONArray *yacjson_value_to_array(YacJSONValue *value) {
    return value->data.array;
}

bool yacjson_value_to_boolean(YacJSONValue *value) {
    return value->data.boolean;
}

long yacjson_value_to_integer(YacJSONValue *value) {
    return value->data.integer;
}

double yacjson_value_to_decimal(YacJSONValue *value) {
    return value->data.decimal;
}

char *yacjson_value_to_string(YacJSONValue *value) {
    return value->data.string;
}

static YacJSONValue *yacjson_value_from_object(YacJSONObject *object) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_OBJECT;
    value->data.object = object;
    return value;
}

static YacJSONValue *yacjson_value_from_array(YacJSONArray *array) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_ARRAY;
    value->data.array = array;
    return value;
}

static YacJSONValue *yacjson_value_from_boolean(bool boolean) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_BOOLEAN;
    value->data.boolean = boolean;
    return value;
}

static YacJSONValue *yacjson_value_from_integer(long integer) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_INTEGER;
    value->data.integer = integer;
    return value;
}

static YacJSONValue *yacjson_value_from_decimal(double decimal) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_DECIMAL;
    value->data.decimal = decimal;
    return value;
}

static YacJSONValue *yacjson_value_from_string(char *string) {
    YacJSONValue *value = malloc(sizeof(YacJSONValue));
    assert(value != NULL);
    value->type = YACJSON_STRING;
    value->data.string = malloc((strlen(string) + 1) * sizeof(char));
    assert(value->data.string != NULL);
    strcpy(value->data.string, string);
    return value;
}

int yacjson_object_size(YacJSONObject *object) {
    return object->size;
}

int yacjson_array_size(YacJSONArray *array) {
    return array->size;
}

void yacjson_object_add(YacJSONObject *object, char *key, YacJSONValue *value) {
    yacdoc_hashmap_add(object, key, (void *) value);
}

void yacjson_array_add(YacJSONArray *array, YacJSONValue *value) {
    yacdoc_arraylist_add(array, (void *) value);
}

void yacjson_object_add_object(YacJSONObject *object, char *key, YacJSONObject *value_object) {
    yacjson_object_add(object, key, yacjson_value_from_object(value_object));
}

void yacjson_object_add_array(YacJSONObject *object, char *key, YacJSONArray *value_array) {
    yacjson_object_add(object, key, yacjson_value_from_array(value_array));
}

void yacjson_object_add_boolean(YacJSONObject *object, char *key, bool value_boolean) {
    yacjson_object_add(object, key, yacjson_value_from_boolean(value_boolean));
}

void yacjson_object_add_integer(YacJSONObject *object, char *key, long value_integer) {
    yacjson_object_add(object, key, yacjson_value_from_integer(value_integer));
}

void yacjson_object_add_decimal(YacJSONObject *object, char *key, double value_decimal) {
    yacjson_object_add(object, key, yacjson_value_from_decimal(value_decimal));
}

void yacjson_object_add_string(YacJSONObject *object, char *key, char *value_string) {
    yacjson_object_add(object, key, yacjson_value_from_string(value_string));
}

void yacjson_array_add_object(YacJSONArray *array, YacJSONObject *value_object) {
    yacjson_array_add(array, yacjson_value_from_object(value_object));
}

void yacjson_array_add_array(YacJSONArray *array, YacJSONArray *value_array) {
    yacjson_array_add(array, yacjson_value_from_array(value_array));
}

void yacjson_array_add_boolean(YacJSONArray *array, bool value_boolean) {
    yacjson_array_add(array, yacjson_value_from_boolean(value_boolean));
}

void yacjson_array_add_integer(YacJSONArray *array, long value_integer) {
    yacjson_array_add(array, yacjson_value_from_integer(value_integer));
}

void yacjson_array_add_decimal(YacJSONArray *array, double value_decimal) {
    yacjson_array_add(array, yacjson_value_from_decimal(value_decimal));
}

void yacjson_array_add_string(YacJSONArray *array, char *value_string) {
    yacjson_array_add(array, yacjson_value_from_string(value_string));
}

YacJSONValue *yacjson_object_get(YacJSONObject *object, const char *key) {
    return yacdoc_hashmap_get(object, key);
}

YacJSONValue *yacjson_array_get(YacJSONArray *array, int index) {
    return yacdoc_arraylist_get(array, index);
}

YacJSONObject *yacjson_object_get_object(YacJSONObject *object, const char *key) {
    return yacjson_value_to_object(yacjson_object_get(object, key));
}

YacJSONArray *yacjson_object_get_array(YacJSONObject *object, const char *key) {
    return yacjson_value_to_array(yacjson_object_get(object, key));
}

bool yacjson_object_get_boolean(YacJSONObject *object, const char *key) {
    return yacjson_value_to_boolean(yacjson_object_get(object, key));
}

long yacjson_object_get_integer(YacJSONObject *object, const char *key) {
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

bool yacjson_array_get_boolean(YacJSONArray *array, int index) {
    return yacjson_value_to_boolean(yacjson_array_get(array, index));
}

long yacjson_array_get_integer(YacJSONArray *array, int index) {
    return yacjson_value_to_integer(yacjson_array_get(array, index));
}

double yacjson_array_get_decimal(YacJSONArray *array, int index) {
    return yacjson_value_to_decimal(yacjson_array_get(array, index));
}

char *yacjson_array_get_string(YacJSONArray *array, int index) {
    return yacjson_value_to_string(yacjson_array_get(array, index));
}

static YacJSONValue *yacjson_parse_primitive_from_string(char *value_string) {
    if (!strcmp(value_string, "true")) return yacjson_value_from_boolean(true);
    if (!strcmp(value_string, "false")) return yacjson_value_from_boolean(false);
    char *end_ptr;
    long value_integer = strtol(value_string, &end_ptr, 10);
    if (*end_ptr == '\0') return yacjson_value_from_integer(value_integer);
    double value_decimal = strtod(value_string, &end_ptr);
    if (*end_ptr == '\0') return yacjson_value_from_decimal(value_decimal);
    return yacjson_value_from_string(strtok(value_string, "\""));
}

// TODO: Extract logic into get_key and get_value functions

static YacJSONValue *yacjson_parse_object_from_file(FILE *file);
static YacJSONValue *yacjson_parse_array_from_file(FILE *file);

static YacJSONValue *yacjson_parse_object_from_file(FILE *file) {
    YacJSONObject *object = yacjson_object_new();
    int pos = 0;
    int curr = 0;
    int prev = 0;
    char buffer[YACJSON_MAX_BUFFER_LEN];
    char key[YACJSON_MAX_BUFFER_LEN];
    bool in_string = false;
    bool in_comment = false;
    bool is_key_added = false;
    bool is_value_added = false;
    while ((curr = fgetc(file)) != EOF) {
        if (in_comment) continue;
        if (curr == '"') in_string = !in_string || prev == '\\';
        if (curr == '/' && prev == '/' && !in_string) in_comment = true;
        if (curr == '\n') in_comment = false;
        if (curr == '\t' || curr == '\n') continue;
        if (curr == ' ' && !in_string) continue; 
        if (curr == ':' && !in_string && !is_key_added) {
            buffer[pos] = '\0';
            strcpy(key, strtok(buffer, "\""));
            pos = 0; is_key_added = true;
            continue;
        }
        if (curr == '{' && !in_string && is_key_added){
            yacjson_object_add(object, key, yacjson_parse_object_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == '[' && !in_string && is_key_added) {
            yacjson_object_add(object, key, yacjson_parse_array_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == ',' && !in_string && is_key_added) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_object_add(object, key, yacjson_parse_primitive_from_string(buffer));
            }
            pos = 0; is_key_added = false; is_value_added = false;
            continue;
        }
        if (curr == '}' && !in_string && is_key_added) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_object_add(object, key, yacjson_parse_primitive_from_string(buffer));
            }
            break;
        }
        buffer[pos++] = (char) curr;
        prev = curr;
    }
    return yacjson_value_from_object(object);
}

static YacJSONValue *yacjson_parse_array_from_file(FILE *file) {
    YacJSONArray *array = yacjson_array_new();
    int pos = 0;
    int curr = 0;
    int prev = 0;
    char buffer[YACJSON_MAX_BUFFER_LEN];
    bool in_string = false;
    bool in_comment = false;
    bool is_value_added = false;
    while ((curr = fgetc(file)) != EOF) {
        if (in_comment) continue;
        if (curr == '"') in_string = !in_string || prev == '\\';
        if (curr == '/' && prev == '/' && !in_string) in_comment = true;
        if (curr == '\n') in_comment = false;
        if (curr == '\t' || curr == '\n') continue;
        if (curr == ' ' && !in_string) continue; 
        if (curr == '{' && !in_string) {
            yacjson_array_add(array, yacjson_parse_object_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == '[' && !in_string) {
            yacjson_array_add(array, yacjson_parse_array_from_file(file));
            pos = 0; is_value_added = true;
            continue;
        }
        if (curr == ',' && !in_string) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_array_add(array, yacjson_parse_primitive_from_string(buffer));
            }
            pos = 0; is_value_added = false;
            continue;
        }
        if (curr == ']' && !in_string) {
            if (!is_value_added) {
                buffer[pos] = '\0';
                yacjson_array_add(array, yacjson_parse_primitive_from_string(buffer));
            }
            break;
        }
        buffer[pos++] = (char) curr;
        prev = curr;
    }
    return yacjson_value_from_array(array);
}

YacJSONValue *yacjson_parse(const char *filepath) {
    YacJSONValue *value = NULL;
    FILE *file = fopen(filepath, "r");
    assert(file != NULL);
    int prev, curr;
    bool in_comment = false;
    while ((curr = fgetc(file)) != EOF) {
        if (in_comment) continue;
        if (prev == '/' && curr == '/') in_comment = true;
        if (curr == '\n') in_comment = false;
        // Top level is object
        if (curr == '{') value = yacjson_parse_object_from_file(file);
        // Top level is array
        if (curr == '[') value = yacjson_parse_array_from_file(file);
        prev = curr;
    }
    return value;
}

static void yacjson_serialize_to_file(YacJSONValue *value, FILE *file, int depth) {
    if (yacjson_value_is_boolean(value)) {
        fprintf(file, "%s", yacjson_value_to_boolean(value) ? "true" : "false");
    } else if (yacjson_value_is_integer(value)) {
        fprintf(file, "%ld", yacjson_value_to_integer(value));
    } else if (yacjson_value_is_decimal(value)) {
        fprintf(file, "%lf", yacjson_value_to_decimal(value));
    } else if (yacjson_value_is_string(value)) {
        fputc('"', file);
        fputs(yacjson_value_to_string(value), file);
        fputc('"', file);
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
