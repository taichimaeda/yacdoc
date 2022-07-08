#include <stdio.h>
#include <stdlib.h>

#include "yacjson-core.h"

int main(void) {
    YacJSONValue *value = yacjson_parse("./tests/test.json");
    YacJSONObject *object = yacjson_value_to_object(value);
    puts(yacjson_array_get_string(yacjson_object_get_array(object, "array"), 3));
    puts(yacjson_object_get_string(yacjson_object_get_object(object, "Blake"), "age"));
    yacjson_serialize(value, "./tests/tests_out.json");
    yacjson_value_free(value);
    return EXIT_SUCCESS;
}
