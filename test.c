#include <stdio.h>
#include "dynamic_arrays.h"
#include "good_field_types.h"
#include "file_schema.h"
#include "dynamic_array_types.h"

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

    TEMPLATE(print, int)(&array_int, NULL);
    TEMPLATE(destroy, int)(&array_int);
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

    TEMPLATE(print, string)(&array_string, NULL);
    TEMPLATE(destroy, string)(&array_string);
    return 0;
}

int test_array_good_field() {
    TEMPLATE(DYN_ARRAY, good_field) array_good_field;
    if (TEMPLATE(create, good_field)(1, &array_good_field)) {
        fprintf(stderr, "error in creation array of good fields\n");
        return 1;
    }
    
    TEMPLATE(append, good_field)(&array_good_field, BOOL_GOOD);
    TEMPLATE(append, good_field)(&array_good_field, NUMBER_GOOD);
    TEMPLATE(append, good_field)(&array_good_field, STRING_GOOD);

    TEMPLATE(print, good_field)(&array_good_field, NULL);
    TEMPLATE(destroy, good_field)(&array_good_field);
    return 0;
}

int test_array_void() {
    TEMPLATE(DYN_ARRAY, vop) array_void;
    if (TEMPLATE(create, vop)(1, &array_void)) {
        fprintf(stderr, "error in creation void* array\n");
        return 1;
    }
    
    int a;
    a = 1;
    TEMPLATE(append, vop)(&array_void, (void *)&a);
    a = 2;
    TEMPLATE(append, vop)(&array_void, (void *)&a);
    
    char *s = "third\0";
    TEMPLATE(append, vop)(&array_void, (void *)s);
    
    int types[3] = {INT, INT, STRING};
    TEMPLATE(print, vop)(&array_void, types);
    TEMPLATE(destroy, vop)(&array_void);    
    return 0;
}

int test_create2() {
    TEMPLATE(DYN_ARRAY, int)* array_int = TEMPLATE(create2, int)(10);
    if (array_int == NULL) {
        fprintf(stderr, "cannot create array int\n");
        return 1;
    }
    TEMPLATE(append, int)(array_int, 10);
    TEMPLATE(append, int)(array_int, 12);
    TEMPLATE(print, int)(array_int, NULL);
    TEMPLATE(destroy2, int)(array_int);
    return 0;
}

int test_array_comparison_simple() {
    TEMPLATE(DYN_ARRAY, int) *a1 = TEMPLATE(create2, int)(10);
    TEMPLATE(DYN_ARRAY, int) *a2 = TEMPLATE(create2, int)(10);
    TEMPLATE(append, int)(a1, 10);
    TEMPLATE(append, int)(a1, 11);
    TEMPLATE(append, int)(a1, 12);

    TEMPLATE(append, int)(a2, 10);
    TEMPLATE(append, int)(a2, 11);
    TEMPLATE(append, int)(a2, 12);

    // arrays are equal 
    TEMPLATE(print, int)(a1, NULL);
    TEMPLATE(print, int)(a2, NULL);
    if (TEMPLATE(array_equal, int)(a1, a2, NULL, NULL)) {
        printf("arrays are equal\n");
    } else
        return 1;
    
    // arrays with different lengths
    TEMPLATE(append, int)(a1, 13);
    TEMPLATE(print, int)(a1, NULL); 
    TEMPLATE(print, int)(a2, NULL);
    if (TEMPLATE(array_equal, int)(a1, a2, NULL, NULL)) {
        return 1;
    } else
        printf("arrays are not equal\n");

    // arrays are not equal
    TEMPLATE(append, int)(a2, 14);
    TEMPLATE(print, int)(a1, NULL); 
    TEMPLATE(print, int)(a2, NULL);
    if (TEMPLATE(array_equal, int)(a1, a2, NULL, NULL)) {
        return 1;
    } else
        printf("arrays are not equal\n");
    
    TEMPLATE(destroy2, int)(a1);
    TEMPLATE(destroy2, int)(a2);
    return 0;
}

int test_array_comparison_complex() {
    TEMPLATE(DYN_ARRAY, vop) *a1 = TEMPLATE(create2, vop)(10);
    TEMPLATE(DYN_ARRAY, vop) *a2 = TEMPLATE(create2, vop)(10);
    int v1 = 1;
    int v2 = 2;
    char *v3 = "first\0";
    char *v4 = "second\0";
    TEMPLATE(append, vop)(a1, (void *)&v1);
    TEMPLATE(append, vop)(a1, (void *)v3);
    TEMPLATE(append, vop)(a2, (void *)&v1);
    TEMPLATE(append, vop)(a2, (void *)v3);
    TEMPLATE(append, vop)(a1, (void *)&v1);
    TEMPLATE(append, vop)(a2, (void *)&v1);
    
    int types[4] = {INT, STRING, INT, INT};
    // arrays are equal 
    TEMPLATE(print, vop)(a1, types);
    TEMPLATE(print, vop)(a2, types);
    if (TEMPLATE(array_equal, vop)(a1, a2, types, types)) {
        printf("arrays are equal\n");
    } else
        return 1;

    // arrays with different lengths
    TEMPLATE(append, vop)(a1, (void *)&v2);
    TEMPLATE(print, vop)(a1, types);
    TEMPLATE(print, vop)(a2, types);
    if (TEMPLATE(array_equal, vop)(a1, a2, types, types)) {
        return 1;
    } else
        printf("arrays are not equal\n");

    // arrays are not equal
    int types2[4] = {INT, STRING, INT, STRING};
    TEMPLATE(append, vop)(a2, (void *)v4);
    TEMPLATE(print, vop)(a1, types); 
    TEMPLATE(print, vop)(a2, types2);
    if (TEMPLATE(array_equal, vop)(a1, a2, types, types2)) {
        return 1;
    } else
        printf("arrays are not equal\n");
    
    TEMPLATE(destroy2, vop)(a1);
    TEMPLATE(destroy2, vop)(a2);
    return 0;
}

typedef int (*func)();

typedef struct {
    char* name;
    func test;
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[7] = {
        {"Test array with integer values", test_array_int},
        {"Test array with char*", test_array_string},
        {"Test array with good fields", test_array_good_field},
        {"Test array with void*", test_array_void},
        {"Test another version of array creation", test_create2},
        {"Test simple array comparison", test_array_comparison_simple},
        {"Test complex array comparison", test_array_comparison_complex}
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

