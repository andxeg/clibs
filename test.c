#include <stdio.h>
#include <stdlib.h>
#include "lists.h"
#include "iofile.h"
#include "file_schema.h"
#include "dynamic_arrays.h"
#include "good_field_types.h"
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

    TEMPLATE(print, int)(&array_int);
    TEMPLATE(destroy, int)(&array_int);
    return 0;
}

int test_array_string_st() {
    TEMPLATE(DYN_ARRAY, string_st) array_string;
    if (TEMPLATE(create, string_st)(1, &array_string)) {
        fprintf(stderr, "error in creation char* array\n");
        return 1;
    }

    TEMPLATE(append, string_st)(&array_string, "first\0");
    TEMPLATE(append, string_st)(&array_string, "second\0");
    TEMPLATE(append, string_st)(&array_string, "third\0");

    TEMPLATE(print, string_st)(&array_string);
    TEMPLATE(destroy, string_st)(&array_string);
    return 0;
}

int get_array_string(TEMPLATE(DYN_ARRAY, string_st)* result) {
    printf("get_array_string\n");
    TEMPLATE(DYN_ARRAY, string_st) array_string;
    if (TEMPLATE(create, string_st)(4, &array_string)) {
        fprintf(stderr, "error in creation char* array\n");
        return 1;
    }

    TEMPLATE(append, string_st)(&array_string, "first\0");
    TEMPLATE(append, string_st)(&array_string, "second\0");
    TEMPLATE(append, string_st)(&array_string, "third\0");
    char* t = "fourth\0";
    TEMPLATE(append, string_st)(&array_string, t);
    TEMPLATE(print, string_st)(&array_string);
    *result = array_string;
    return 0;
}

int test_array_string() {
    TEMPLATE(DYN_ARRAY, string_st) array_string;
    get_array_string(&array_string);
    TEMPLATE(append, string_st)(&array_string, "fifth\0");
    TEMPLATE(append, string_st)(&array_string, "sixth\0");
    TEMPLATE(print, string_st)(&array_string);
    TEMPLATE(destroy, string_st)(&array_string);
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

    TEMPLATE(print, good_field)(&array_good_field);
    TEMPLATE(destroy, good_field)(&array_good_field);
    return 0;
}

int test_array_char() {
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(10, &array_char);
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(append, char)(&array_char, 'a');
    TEMPLATE(append, char)(&array_char, 'b');
    TEMPLATE(append, char)(&array_char, 'c'); 
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(destroy, char)(&array_char);
    return 0;
}

int test_array_void_st() {
    TEMPLATE(DYN_ARRAY, vop_st) array_void;
    if (TEMPLATE(create, vop_st)(1, &array_void)) {
        fprintf(stderr, "error in creation void* array\n");
        return 1;
    }
    
    int a = 1;
    TEMPLATE(append, vop_st)(&array_void, (void *)&a);
    int b = 2;
    TEMPLATE(append, vop_st)(&array_void, (void *)&b);
    
    char *s = "third\0";
    TEMPLATE(append, vop_st)(&array_void, (void *)s);
    
    int types[3] = {INT, INT, STRING};
    for (int i = 0; i < 3; i++)
        TEMPLATE(append, int)(array_void.types, types[i]);
    TEMPLATE(print, vop_st)(&array_void);
    TEMPLATE(destroy, vop_st)(&array_void);    
    return 0;
}

int get_array_void(TEMPLATE(DYN_ARRAY, vop)* result) {
    TEMPLATE(DYN_ARRAY, vop) array_void;
    if (TEMPLATE(create, vop)(10, &array_void)) {
        fprintf(stderr, "error in creation void* array\n");
        return 1;
    }
    
    int* a = (int *) malloc(sizeof(int));
    int* b = (int *) malloc(sizeof(int));
    char* s1 = "third";
    char* s = (char *) malloc(strlen(s1) + 1);
    s = strcpy(s, s1);
    *a = 1;
    *b = 2;
    TEMPLATE(append, vop)(&array_void, (void *)a);
    TEMPLATE(append, vop)(&array_void, (void *)b);
    TEMPLATE(append, vop)(&array_void, (void *)s);
    
    int types[3] = {INT, INT, STRING};
    for (int i = 0; i < 3; i++)
        TEMPLATE(append, int)(array_void.types, types[i]);
    TEMPLATE(print, vop)(&array_void);
    *result = array_void;
    return 0;
}

int test_array_void() {
    TEMPLATE(DYN_ARRAY, vop) array_void;
    get_array_void(&array_void);
    int* a = (int *) malloc(sizeof(int));
    *a = 1;
    TEMPLATE(append, vop)(&array_void, (void *)a);
    TEMPLATE(append, int)(array_void.types, INT);
    TEMPLATE(print, vop)(&array_void);
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
    TEMPLATE(print, int)(array_int);
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
    TEMPLATE(print, int)(a1);
    TEMPLATE(print, int)(a2);
    if (TEMPLATE(array_equal, int)(a1, a2)) {
        printf("arrays are equal\n");
    } else
        return 1;
    
    // arrays with different lengths
    TEMPLATE(append, int)(a1, 13);
    TEMPLATE(print, int)(a1); 
    TEMPLATE(print, int)(a2);
    if (TEMPLATE(array_equal, int)(a1, a2)) {
        return 1;
    } else
        printf("arrays are not equal\n");

    // arrays are not equal
    TEMPLATE(append, int)(a2, 14);
    TEMPLATE(print, int)(a1); 
    TEMPLATE(print, int)(a2);
    if (TEMPLATE(array_equal, int)(a1, a2)) {
        return 1;
    } else
        printf("arrays are not equal\n");
    
    TEMPLATE(destroy2, int)(a1);
    TEMPLATE(destroy2, int)(a2);
    return 0;
}

int test_array_comparison_complex() {
    TEMPLATE(DYN_ARRAY, vop_st) *a1 = TEMPLATE(create2, vop_st)(10);
    TEMPLATE(DYN_ARRAY, vop_st) *a2 = TEMPLATE(create2, vop_st)(10);
    int v1 = 1;
    int v2 = 2;
    char *v3 = "first\0";
    char *v4 = "second\0";
    TEMPLATE(append, vop_st)(a1, (void *)&v1);
    TEMPLATE(append, vop_st)(a1, (void *)v3);
    TEMPLATE(append, vop_st)(a2, (void *)&v1);
    TEMPLATE(append, vop_st)(a2, (void *)v3);
    TEMPLATE(append, vop_st)(a1, (void *)&v1);
    TEMPLATE(append, vop_st)(a2, (void *)&v1);
    
    int types[4] = {INT, STRING, INT, INT};
    for (int i = 0; i < 3; i++) {
        TEMPLATE(append, int)(a2->types, types[i]);
        TEMPLATE(append, int)(a1->types, types[i]);
    }
    // arrays are equal 
    TEMPLATE(print, vop_st)(a1);
    TEMPLATE(print, vop_st)(a2);
    if (TEMPLATE(array_equal, vop_st)(a1, a2)) {
        printf("arrays are equal\n");
    } else
        return 1;

    // arrays with different lengths
    TEMPLATE(append, vop_st)(a1, (void *)&v2);
    TEMPLATE(append, int)(a1->types, INT);
    TEMPLATE(print, vop_st)(a1);
    TEMPLATE(print, vop_st)(a2);
    if (TEMPLATE(array_equal, vop_st)(a1, a2)) {
        return 1;
    } else
        printf("arrays are not equal\n");

    // arrays are not equal
    TEMPLATE(append, vop_st)(a2, (void *)v4);
    for (int i = 0; i < 4; i++)
        TEMPLATE(append, int)(a2->types, STRING);
    TEMPLATE(print, vop_st)(a1); 
    TEMPLATE(print, vop_st)(a2);
    if (TEMPLATE(array_equal, vop_st)(a1, a2)) {
        return 1;
    } else
        printf("arrays are not equal\n");
    
    TEMPLATE(destroy2, vop_st)(a1);
    TEMPLATE(destroy2, vop_st)(a2);
    return 0;
}

int test_file_read() {
    FILE_SCHEMA* schema = create_file_schema();
    char* filename = "./test_data/goods.txt\0";

    if (read_file_with_goods(filename, schema)) {
        destroy_file_schema(schema);
        return 1;
    }

    print_file_schema(schema); 
    destroy_file_schema(schema);   
    return 0;
}

int test_shrink_to_fit() {
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(10, &array_char);
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(append, char)(&array_char, 'a');
    TEMPLATE(append, char)(&array_char, 'b');
    TEMPLATE(append, char)(&array_char, 'c');
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(shrink_to_fit, char)(&array_char);
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(destroy, char)(&array_char);
    return 0;
}

int test_get_raw_data() {
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(10, &array_char);
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(append, char)(&array_char, 'a');
    TEMPLATE(append, char)(&array_char, 'b');
    TEMPLATE(append, char)(&array_char, 'c');
    TEMPLATE(append, char)(&array_char, '\0');
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(shrink_to_fit, char)(&array_char);
    TEMPLATE(print, char)(&array_char);

    char* raw_data = array_char.data;
    printf("raw data -> %s\n", raw_data);
    free(raw_data);
    return 0;
}

int test_get_types() {
    TEMPLATE(DYN_ARRAY, vop) array_void;
    get_array_void(&array_void);
    int* a = (int *) malloc(sizeof(int));
    int* b = (int *) malloc(sizeof(int));
    int* c = (int *) malloc(sizeof(int));
    int* d = (int *) malloc(sizeof(int));
    int* e = (int *) malloc(sizeof(int));
    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    TEMPLATE(append, vop)(&array_void, (void *)a);
    TEMPLATE(append, vop)(&array_void, (void *)b);
    TEMPLATE(append, vop)(&array_void, (void *)c);
    TEMPLATE(append, vop)(&array_void, (void *)d);
    TEMPLATE(append, vop)(&array_void, (void *)e);
    for (int i = 0; i <5; i++) {
        TEMPLATE(append, int)(array_void.types, INT);
    }
    TEMPLATE(print, vop)(&array_void);
   
    TEMPLATE(DYN_ARRAY, int)* types = TEMPLATE(get_array_types, vop)(&array_void);
    int length = array_void.length;
    for (int i = 0; i < length; i++) {
        int type;
        TEMPLATE(get, int)(types, i, &type);
        printf("%d ", type);
    }
    putchar('\n');
    TEMPLATE(destroy, vop)(&array_void);
    return 0;
}

int test_recreate_array() {
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(10, &array_char);
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(append, char)(&array_char, 'a');
    TEMPLATE(append, char)(&array_char, 'b');
    TEMPLATE(append, char)(&array_char, 'c');
    TEMPLATE(append, char)(&array_char, '\0');
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(shrink_to_fit, char)(&array_char);
    TEMPLATE(print, char)(&array_char);
    
    TEMPLATE(recreate, char)(&array_char, 10);
    TEMPLATE(append, char)(&array_char, 'c');
    TEMPLATE(print, char)(&array_char);
    TEMPLATE(destroy, char)(&array_char);
    return 0;
}

int test_array_string_raw_data() {
    TEMPLATE(DYN_ARRAY, string) array_string;
    TEMPLATE(create, string)(10, &array_string);
    /*
    char* s1 = "first";
    char* s2 = "second";
    char* s3 = "third";
    char* a = (char *) malloc(strlen(s1) + 1);
    char* b = (char *) malloc(strlen(s2) + 1);
    char* c = (char *) malloc(strlen(s3) + 1);
    a = strcpy(a, s1);
    b = strcpy(b, s2);
    c = strcpy(c, s3);
    TEMPLATE(append, string)(&array_string, a);
    TEMPLATE(append, string)(&array_string, b);
    TEMPLATE(append, string)(&array_string, c);
    */
    
    TEMPLATE(DYN_ARRAY, char) a;
    TEMPLATE(create, char)(10, &a);
    TEMPLATE(append, char)(&a, 'o');
    TEMPLATE(append, char)(&a, 'n');
    TEMPLATE(append, char)(&a, 'e');
    TEMPLATE(append, char)(&a, '\0');
    TEMPLATE(shrink_to_fit, char)(&a);

    TEMPLATE(DYN_ARRAY, char) b;
    TEMPLATE(create, char)(10, &b);
    TEMPLATE(append, char)(&b, 't');
    TEMPLATE(append, char)(&b, 'w');
    TEMPLATE(append, char)(&b, 'o');
    TEMPLATE(append, char)(&b, '\0');
    TEMPLATE(shrink_to_fit, char)(&b);

    TEMPLATE(print, char)(&a);
    TEMPLATE(print, char)(&b);
    char* raw_data_a = TEMPLATE(get_raw_data, char)(&a);
    char* raw_data_b = TEMPLATE(get_raw_data, char)(&b);

    TEMPLATE(append, string)(&array_string, raw_data_a);
    TEMPLATE(append, string)(&array_string, raw_data_b); 
    TEMPLATE(print, string)(&array_string);
    TEMPLATE(destroy, string)(&array_string);
    return 0;
}

typedef int (*func)();

typedef struct {
    char* name;
    func test;
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[16] = {
        {"Test array with integer values", test_array_int},
        {"Test array with char* (static)", test_array_string_st},
        {"Test array with char* (dynamic)", test_array_string},
        {"Test array with good fields", test_array_good_field},
        {"Test array with char", test_array_char},
        {"Test array with void* (static)", test_array_void_st},
        {"Test array with void* (dynamic)", test_array_void},
        {"Test another version of array creation", test_create2},
        {"Test simple array comparison", test_array_comparison_simple},
        {"Test complex array comparison", test_array_comparison_complex},
        {"Test file reading", test_file_read},
        {"Test shrink to fit", test_shrink_to_fit},
        {"Test get raw data", test_get_raw_data},
        {"Test get types", test_get_types},
        {"Test recreate array", test_recreate_array},
        {"Test string dyn array with element raw data from dyn array char", test_array_string_raw_data}
    };
    
    int all_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed_tests = 0;
         
    for (int i = 0; i < all_tests; i++) {
        TEST_CASE ts = test_cases[i];
        printf("launch: %s\n", ts.name);
        if (ts.test()) {
            printf("There is an error in test: %s\n", ts.name);
        } else {
            ++passed_tests;
            printf("%s was successfully passed\n", ts.name);
        }
        putchar('\n');
    }

    printf("ALL: %d, PASSED: %d, FAILED: %d\n",
           all_tests, passed_tests, all_tests - passed_tests);
    return 0;
}

