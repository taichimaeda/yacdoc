#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "yacxml-core.h"

#define YACXML_MAX_BUFFER_LEN (256)

static char *yacxml_parse_name_from_file(FILE *file) {
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    char *name = NULL;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == ' ') {
            buffer[pos] = '\0';
            name = malloc(strlen(buffer) + 1);
            assert(name != NULL);
            strcpy(name, buffer);
            pos = 0;
            break;
        }
        buffer[pos++] = (char) ch;
    }
    return name;
}

static YacXMLAttributeMap *yacxml_parse_attribute_map_from_file(FILE *file) {
    YacXMLAttributeMap *attrs = yacdoc_hashmap_new();
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    char key[YACXML_MAX_BUFFER_LEN];
    bool is_quoted = false;
    while ((ch = fgetc(file)) != EOF) {
        if ((ch == ' ' || ch == '\n' || ch == '\t') && !is_quoted) continue;
        if (ch == '>' && !is_quoted) break;
        if (ch == '=' && !is_quoted) {
            buffer[pos] = '\0'; pos = 0;
            strcpy(key, buffer);
            continue;
        }
        if (ch == '"') {
            if (is_quoted) {
                buffer[pos] = '\0'; pos = 0;
                yacdoc_hashmap_add(attrs, key, strtok(buffer, "\""));
            }
            is_quoted = !is_quoted;
            continue;
        }
        buffer[pos++] = (char) ch;
    }
    return attrs;
}

static YacXMLElement *yacxml_parse_from_file(FILE *file) {
    YacXMLElement *elem = malloc(sizeof(YacXMLElement));
    assert(elem != NULL);
    elem->name = yacxml_parse_name_from_file(file);
    elem->attrs = yacxml_parse_attribute_map_from_file(file);
    int ch, pos = 0;
    static char buffer[YACXML_MAX_BUFFER_LEN];
    bool is_commented = false;
    while ((ch = fgetc(file)) != EOF) {
        buffer[pos] = '\0';
        if (is_commented) continue;
        if (!strcmp(buffer + pos - 4, "<!--")) is_commented = true;
        if (!strcmp(buffer + pos - 3, "-->")) is_commented = false;
        if (ch == '\t') continue;
        if (ch == '\n') continue;
        if (ch == '<') {
            yacxml_parse_from_file(file);
        }
        buffer[pos++] = (char) ch;
    }
    return elem;
}

YacXMLElement *yacxml_parse(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    // get first '<'
    fgetc(file);
    YacXMLElement *elem = yacxml_parse_from_file(file);
    fclose(file);
    return elem;
}
