#include <stdio.h>
#include "dynamic_arrays.h"
#include "good_field_types.h"
#include "file_schema.h"

int test_array_int() {
    TEMPLATE(DYN_ARRAY, int) array_int;
    if (TEMPLATE(create, int)(10, &array_int)) {
        fprintf(stderr, "error in creation int array\n");
        return 1;
    }
    
    TEMPLATE(append, int)(&array_int, 1);
    TEMPLATE(append, int)(&array_int, 2);
    TEMPLATE(append, int)(&array_int, 3);
    TEMPLATE(append, int)(&array_int, 4);
    TEMPLATE(append, int)(&array_int, 5);

    TEMPLATE(print, int)(&array_int);
    return 0;
}

int test_array_string() {
    TEMPLATE(DYN_ARRAY, string) array_string;
    if (TEMPLATE(create, string)(1, &array_string)) {
        fprintf(stderr, "error in creation char* array\n");
        return 1;
    }

    TEMPLATE(append, string)(&array_string, "first\0");
    TEMPLATE(append, string)(&array_string, "second\0");
    TEMPLATE(append, string)(&array_string, "third\0");

    TEMPLATE(print, string)(&array_string);
    return 0;
}

int test_array_good_field() {
    TEMPLATE(DYN_ARRAY, good_field) array_good_field;
    if (TEMPLATE(create, good_field)(1, &array_good_field)) {
        fprintf(stderr, "error in creation array of good fields\n");
        return 1;
    }
    
    TEMPLATE(append, good_field)(&array_good_field, BOOL);
    TEMPLATE(append, good_field)(&array_good_field, NUMBER);
    TEMPLATE(append, good_field)(&array_good_field, STRING);

    TEMPLATE(print, good_field) (&array_good_field);
    return 0;
}

typedef int (*func)();

typedef struct {
    char* name;
    func test;
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[3] = {
        {"Test integer array", test_array_int},
        {"Test array with char*", test_array_string},
        {"Test array with good fields", test_array_good_field}
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        TEST_CASE ts = test_cases[i];
        printf("launch: %s\n", ts.name);
        if (ts.test())
            fprintf(stderr, "error in test: %s\n", ts.name);
        else
            printf("%s was successfully passed\n", ts.name);
        putchar('\n');
    }
    return 0;
}

