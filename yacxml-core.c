#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yacxml-core.h"

#define YACXML_MAX_BUFFER_LEN (256)

static YacXMLAttributeMap *yacxml_attribute_map_new() {
    return yacdoc_hashmap_new();
}

static YacXMLChildMap *yacxml_child_map_new() {
    return yacdoc_hashmap_new();
}

static void yacxml_attribute_map_add(YacXMLAttributeMap *attrs, const char *key, char *value) {
    yacdoc_hashmap_add(attrs, key, (void *) value);
}

static void yacxml_child_map_add(YacXMLChildMap *chs, const char *key, YacXMLElement *value) {
    yacdoc_hashmap_add(chs, key, (void *) value);
}

static char *yacxml_attribute_map_get(YacXMLAttributeMap *attrs, const char *key) {
    return yacdoc_hashmap_get(attrs, key);
}

static YacXMLElement *yacxml_child_map_get(YacXMLChildMap *chs, const char *key) {
    return yacdoc_hashmap_get(chs, key);
}

YacXMLAttributeMapIterator *yacxml_attribute_map_iterator_new(YacXMLAttributeMap *attrs) {
    return yacdoc_hashmap_iterator_new(attrs);
}

YacXMLChildMapIterator *yacxml_child_map_iterator_new(YacXMLChildMap *chs) {
    return yacdoc_hashmap_iterator_new(chs);
}

YacXMLAttributeMapItem *yacxml_attribute_map_iterator_next(YacXMLAttributeMapIterator *it) {
    return yacdoc_hashmap_iterator_next(it);
}

YacXMLChildMapItem *yacxml_child_map_iterator_next(YacXMLChildMapIterator *it) {
    return yacdoc_hashmap_iterator_next(it);
}

YacXMLElement *yacxml_element_new() {
    YacXMLElement *elem = malloc(sizeof(YacXMLElement));
    assert(elem != NULL);
    elem->name = malloc(1);
    assert(elem->name != NULL);
    strcpy(elem->name, "");
    elem->text = malloc(1);
    assert(elem->text != NULL);
    strcpy(elem->text, "");
    elem->attributes = yacxml_attribute_map_new();
    elem->children = yacxml_child_map_new();
    return elem;
}

static void yacxml_element_free_void(void *elem) {
    free(((YacXMLElement *) elem)->name);
    free(((YacXMLElement *) elem)->text);
    yacdoc_hashmap_free(((YacXMLElement *) elem)->attributes, free);
    yacdoc_hashmap_free(((YacXMLElement *) elem)->children, yacxml_element_free_void);
    free(elem);
}

void yacxml_element_free(YacXMLElement *elem) {
    yacxml_element_free_void((void *) elem);
}

char *yacxml_element_get_name(YacXMLElement *elem) {
    return elem->name;
}

void yacxml_element_set_name(YacXMLElement *elem, const char *name) {
    free(elem->name);
    elem->name = malloc(strlen(name) + 1);
    assert(elem->name != NULL);
    strcpy(elem->name, name);
}

char *yacxml_element_get_text(YacXMLElement *elem) {
    return elem->text;
}

void yacxml_element_set_text(YacXMLElement *elem, const char *text) {
    free(elem->text);
    elem->text = malloc(strlen(text) + 1);
    assert(elem->text != NULL);
    strcpy(elem->text, text);
}

char *yacxml_element_get_attribute(YacXMLElement *elem, const char *key) {
    return yacxml_attribute_map_get(elem->attributes, key);
}

void yacxml_element_add_attribute(YacXMLElement *elem, const char *key, char *value) {
    yacxml_attribute_map_add(elem->attributes, key, value);
}

YacXMLElement *yacxml_element_get_child(YacXMLElement *elem, const char *key) {
    return yacxml_child_map_get(elem->children, key);
}

void yacxml_element_add_child(YacXMLElement *elem, const char *key, YacXMLElement *value) {
    yacxml_child_map_add(elem->children, key, value);
}

static char *yacxml_get_opening_tag_name(FILE *file) {
    char *name = NULL;
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    while ((ch = fgetc(file)) != EOF) {
        switch (ch) {
            case '/':
            case '>':
                ungetc(ch, file);
            case ' ':
                buffer[pos] = '\0';
                name = malloc(strlen(buffer) + 1);
                assert(name != NULL);
                strcpy(name, buffer);
                return name;
        }
        buffer[pos++] = (char) ch;
    }
    return NULL;
}

static YacXMLAttributeMap *yacxml_get_tag_attributes(FILE *file) {
    YacXMLAttributeMap *attrs = yacxml_attribute_map_new();
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    char key[YACXML_MAX_BUFFER_LEN];
    char *value;
    bool is_quoted = false;
    while ((ch = fgetc(file)) != EOF) {
        if (!is_quoted) {
            switch (ch) {
                case ' ':
                case '\n':
                case '\t':
                    continue;
                case '/':
                case '>':
                    ungetc(ch, file);
                    return attrs;
                case '=':
                    buffer[pos] = '\0';
                    pos = 0;
                    strcpy(key, buffer);
                    continue;
                case '"':
                    is_quoted = true;
                    continue;
            }
        } else {
            switch (ch) {
                case '"':
                    buffer[pos] = '\0';
                    pos = 0;
                    value = malloc(strlen(buffer) + 1);
                    assert(value != NULL);
                    strcpy(value, buffer);
                    yacxml_attribute_map_add(attrs, key, value);
                    is_quoted = false;
                    continue;
            }
        }
        buffer[pos++] = (char) ch;
    }
    return NULL;
}

static int fpeekc(FILE *file) {
    int ch = fgetc(file);
    ungetc(ch, file);
    return ch;
}

static bool fpeeks_match(FILE *file, char *target) {
    static char buffer[YACXML_MAX_BUFFER_LEN];
    bool res = true;
    for (int i = 0; target[i] != '\0'; i++) {
        buffer[i] = (char) fgetc(file);
        if (buffer[i] != target[i]) res = false;
    }
    for (int j = strlen(target) - 1; j >= 0; j--) {
        ungetc(buffer[j], file);
    }
    return res;
}

static bool yacxml_match_closing_tag_name(FILE *file, char *tag_name) {
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    while ((ch = fgetc(file)) != EOF) {
        switch (ch) {
            case ' ':
            case '/':
                continue;
            case '>':
                buffer[pos] = '\0';
                return !strcmp(tag_name, buffer);
        }
        buffer[pos++] = (char) ch;
    }
    return false;
}

static YacXMLElement *yacxml_parse_from_file(FILE *file) {
    YacXMLElement *elem = malloc(sizeof(YacXMLElement));
    assert(elem != NULL);
    elem->name = yacxml_get_opening_tag_name(file);
    elem->attributes = yacxml_get_tag_attributes(file);
    elem->children = yacxml_child_map_new();
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    bool is_commented = false;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '<' && fpeeks_match(file, "!--")) {
            is_commented = true;
            continue;
        }
        if (ch == '-' && fpeeks_match(file, "->")) {
            is_commented = false;
            continue;
        }
        if (is_commented) continue;
        switch (ch) {
            case ' ':
            case '\n':
            case '\t':
                continue;
            case '>':
                continue;
            case '/':
                if (fpeekc(file) == '>') return elem;
            case '<':
                if (fpeekc(file) == '/') {
                    buffer[pos] = '\0';
                    elem->text = malloc(strlen(buffer) + 1);
                    assert(elem->text != NULL);
                    strcpy(elem->text, buffer);
                    assert(yacxml_match_closing_tag_name(file, elem->name));
                    return elem;
                }
                if (isalpha(fpeekc(file))) {
                    YacXMLElement *child = yacxml_parse_from_file(file);
                    yacxml_child_map_add(elem->children, child->name, child);
                    continue;
                }
                continue;
        }
        buffer[pos++] = (char) ch;
    }
    return NULL;
}

YacXMLElement *yacxml_parse(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    // TODO: Handle XML prolog
    assert(fgetc(file) == '<');
    YacXMLElement *elem = yacxml_parse_from_file(file);
    fclose(file);
    return elem;
}

static void yacxml_serialize_to_file(YacXMLElement *elem, FILE *file, int depth) {
    for (int i = 0; i < depth - 1; i++) fputc('\t', file);
    fputc('<', file);
    fputs(elem->name, file);
    YacXMLAttributeMapItem *attr_item;
    YacXMLAttributeMapIterator *attr_it = yacxml_attribute_map_iterator_new(elem->attributes);
    while ((attr_item = yacxml_attribute_map_iterator_next(attr_it)) != NULL) {
        fputc(' ', file);
        fputs(attr_item->key, file);
        fputs("=\"", file);
        fputs(attr_item->value, file);
        fputc('\"', file);
    }
    if (!strcmp(elem->text, "") && elem->children->size == 0) {
        fputs("/>\n", file);
        return;
    }
    fputs(">\n", file);
    if (strcmp(elem->text, "")) {
        for (int i = 0; i < depth; i++) fputc('\t', file);
        fputs(elem->text, file);
        fputc('\n', file);
    }
    YacXMLChildMapItem *ch_item;
    YacXMLChildMapIterator *ch_it = yacxml_child_map_iterator_new(elem->children);
    while ((ch_item = yacxml_child_map_iterator_next(ch_it)) != NULL) {
        yacxml_serialize_to_file(ch_item->value, file, depth + 1);
    }
    for (int i = 0; i < depth - 1; i++) fputc('\t', file);
    fputs("</", file);
    fputs(elem->name, file);
    fputs(">\n", file);
}

void yacxml_serialize(YacXMLElement *elem, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    yacxml_serialize_to_file(elem, file, 1);
    fclose(file);
}
