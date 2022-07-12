#include <stdio.h>
#include <stdlib.h>

#include "yacjson-core.h"
#include "yacxml-core.h"

int main(void) {
    // YacJSONValue *value = yacjson_parse("./tests/test.json");
    // YacJSONObject *object = yacjson_value_to_object(value);
    // puts(yacjson_array_get_string(yacjson_object_get_array(object, "array"), 3));
    // puts(yacjson_object_get_string(yacjson_object_get_object(object, "Blake"), "age"));
    // yacjson_serialize(value, "./tests/tests_out.json");
    // yacjson_value_free(value);
    YacXMLElement *elem = yacxml_parse("./tests/test.xml");
    YacXMLElement *child = yacxml_element_get_child(yacxml_element_get_child(elem, "consist"), "off");
    puts(yacxml_element_get_attribute(child, "asleep"));
    puts(yacxml_element_get_text(child));
    yacxml_serialize(elem, "./tests/test_out.xml");
    yacxml_element_free(elem);
    return EXIT_SUCCESS;
}
