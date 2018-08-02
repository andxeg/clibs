#include <stdio.h>
#include <stdlib.h>
#include "lists.h"


int test_creation_int_list() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_empty_list_verification() {
    TEMPLATE(LIST, string_st)* list_string = TEMPLATE(create_list, string_st)();
    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string_st)(list_string));
    TEMPLATE(add_to_list, string_st)(list_string, "first\0");
    TEMPLATE(add_to_list, string_st)(list_string, "second\0");
    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string_st)(list_string));
    TEMPLATE(print_list, string_st)(list_string);
    TEMPLATE(destroy_list, string_st)(list_string);
    return 0;
}

int test_contains_in_list() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    int max_len = 10;
    for (int i = 0; i < max_len; i++) {
        TEMPLATE(add_to_list, int)(list_int, i);
        TEMPLATE(print_list, int)(list_int);
    }
    
    if (TEMPLATE(contains_list, int)(list_int, 0) &&
        TEMPLATE(contains_list, int)(list_int, 5) &&
        TEMPLATE(contains_list, int)(list_int, 9)) {
        printf("list contains 0, 5 and 9 value\n");
    } else
        return 1;
    
    if (TEMPLATE(contains_list, int)(list_int, 10)) {
        return 1;
    } else {
        printf("list doesn't contain value 10\n");
    }
    
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_vec2_list() {
    TEMPLATE(LIST, int_vec2_st)* list_int_vec2 = TEMPLATE(create_list, int_vec2_st)();
    int a[2] = {1, 2};
    int b[2] = {3, 4};
    int c[2] = {5, 6};
    TEMPLATE(add_to_list, int_vec2_st)(list_int_vec2, a);
    TEMPLATE(add_to_list, int_vec2_st)(list_int_vec2, b);
    TEMPLATE(add_to_list, int_vec2_st)(list_int_vec2, c);
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    
    int index = 2;
    int d[2] = {7, 8};
    printf("add [7, 8] to list to %d-th position -> %d\n", index, TEMPLATE(add_elem_by_index, int_vec2_st)(list_int_vec2, index, d));
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    
    printf("empty value -> %d\n", TEMPLATE(is_empty_list, int_vec2_st)(list_int_vec2));

    int e[2] = {1, 2};
    printf("contain value for [1, 2] -> %d\n", TEMPLATE(contains_list, int_vec2_st)(list_int_vec2, e));
    
    printf("remove [1,2] from list -> %d\n", TEMPLATE(remove_from_list, int_vec2_st)(list_int_vec2, e));
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    
    int f[2] = {1, 3};
    printf("remove [1,3] from list -> %d\n", TEMPLATE(remove_from_list, int_vec2_st)(list_int_vec2, f));
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    
    // int *res = (int *) malloc(2 * sizeof(int)); // heap, free after usage
    int res[2]; // static 
    int* pres = &res[0];
    index = 1;
    if (TEMPLATE(get_by_index, int_vec2_st)(list_int_vec2, index, &pres)) {
        printf("cannot get %d-th element\n", index);
    } else {
       printf("get %d-th element: [%d, %d]\n", index, res[0], res[1]);
    }
    
    printf("indexOf([1, 2]) -> %d\n", TEMPLATE(indexOf, int_vec2_st)(list_int_vec2, a));
    printf("indexOf([3, 4]) -> %d\n", TEMPLATE(indexOf, int_vec2_st)(list_int_vec2, b));
    
    int g[2] = {10, 20};
    if (TEMPLATE(add_elem_by_index, int_vec2_st)(list_int_vec2, index, g)) {
        printf("element [%d, %d] cannot be added to %d-th position\n", g[0], g[1], index);
    } else {
        printf("element [%d, %d] was successfully added to %d-th position\n", g[0], g[1], index);
    }
    
    index = TEMPLATE(size_list, int_vec2_st)(list_int_vec2);
    if (TEMPLATE(add_elem_by_index, int_vec2_st)(list_int_vec2, index, g)) {
        printf("element [%d, %d] cannot be added to %d-th position\n", g[0], g[1], index);
    } else {
        printf("element [%d, %d] was successfully added to %d-th position\n", g[0], g[1], index);
    }

    index = TEMPLATE(size_list, int_vec2_st)(list_int_vec2) + 1;
    if (TEMPLATE(add_elem_by_index, int_vec2_st)(list_int_vec2, index, g)) {
        printf("element [%d, %d] cannot be added to %d-th position\n", g[0], g[1], index);
    } else {
        printf("element [%d, %d] was successfully added to %d-th position\n", g[0], g[1], index);
    }
   
    index = 100;
    if (TEMPLATE(remove_elem_by_index, int_vec2_st)(list_int_vec2, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    putchar('\n');
  
    index = 0;
    if (TEMPLATE(remove_elem_by_index, int_vec2_st)(list_int_vec2, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    
    TEMPLATE(print_list, int_vec2_st)(list_int_vec2);
    TEMPLATE(destroy_list, int_vec2_st)(list_int_vec2);
    return 0;
}

int test_removing_from_list() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(print_list, int)(list_int);

    TEMPLATE(remove_from_list, int)(list_int, 3);
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(remove_from_list, int)(list_int, 1);
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(remove_from_list, int)(list_int, 2);
    TEMPLATE(print_list, int)(list_int);

    printf("empty value -> %d\n", TEMPLATE(is_empty_list, int)(list_int));
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_get_elem_by_index() {
    TEMPLATE(LIST, string_st)* list_string = TEMPLATE(create_list, string_st)();
    TEMPLATE(add_to_list, string_st)(list_string, "first\0");
    TEMPLATE(add_to_list, string_st)(list_string, "second\0");
    TEMPLATE(add_to_list, string_st)(list_string, "third\0");
    TEMPLATE(print_list, string_st)(list_string);
    
    char* a;
    int index = 2;
    if (TEMPLATE(get_by_index, string_st)(list_string, index, &a)) {
        printf("cannot find %d-th in list\n", index);
        return 1;
    } else {
        printf("%d-th element -> %s\n", index, a);
    }

    TEMPLATE(remove_from_list, string_st)(list_string, "third\0");
    TEMPLATE(print_list, string_st)(list_string);
    
    index = 0;
    if (TEMPLATE(get_by_index, string_st)(list_string, index, &a)) {
        printf("cannot find %d-th in list\n", index);
        return 1;
    } else {
        printf("%d-th element -> %s\n", index, a);
    }



    TEMPLATE(remove_from_list, string_st)(list_string, "first\0");
    TEMPLATE(print_list, string_st)(list_string);
    TEMPLATE(remove_from_list, string_st)(list_string, "second\0");
    TEMPLATE(print_list, string_st)(list_string);

    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string_st)(list_string));
    TEMPLATE(print_list, string_st)(list_string);
    TEMPLATE(destroy_list, string_st)(list_string);
    return 0;
}

int test_insert_elem_by_index() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(add_to_list, int)(list_int, 10);
    TEMPLATE(add_to_list, int)(list_int, 12);
    TEMPLATE(add_to_list, int)(list_int, 13);
    TEMPLATE(print_list, int)(list_int);    
    

    int list_size = TEMPLATE(size_list, int)(list_int);
    for (int i = 0; i < list_size; i++) {
        TEMPLATE(insert_by_index, int)(list_int, i, i);
    }

    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_indexOf() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(add_to_list, int)(list_int, 10);
    TEMPLATE(add_to_list, int)(list_int, 12);
    TEMPLATE(add_to_list, int)(list_int, 13);
    printf("indexOf(1) -> %d\n", TEMPLATE(indexOf, int)(list_int, 1));
    printf("indexOf(10) -> %d\n", TEMPLATE(indexOf, int)(list_int, 10));
    printf("indexOf(12) -> %d\n", TEMPLATE(indexOf, int)(list_int, 12));
    printf("indexOf(-1) -> %d\n", TEMPLATE(indexOf, int)(list_int, -1));
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
} 

int test_add_element_by_index() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(print_list, int)(list_int);

    int elem = 0;
    int index = 0;
    if (TEMPLATE(add_elem_by_index, int)(list_int, index, elem)) {
        printf("element %d cannot be added to %d-th position\n", elem, index);
    } else {
        printf("element %d was successfully added to %d-th position\n", elem, index);
    }
    
    elem = 20;
    index = 3;
    if (TEMPLATE(add_elem_by_index, int)(list_int, index, elem)) {
        printf("element %d cannot be added to %d-th position\n", elem, index);
    } else {
        printf("element %d was successfully added to %d-th position\n", elem, index);
    }
    
    elem = 20;
    index = 3;
    if (TEMPLATE(add_elem_by_index, int)(list_int, index, elem)) {
        printf("element %d cannot be added to %d-th position\n", elem, index);
    } else {
        printf("element %d was successfully added to %d-th position\n", elem, index);
    }
    
    elem = 30;
    index = TEMPLATE(size_list, int)(list_int);
    if (TEMPLATE(add_elem_by_index, int)(list_int, index, elem)) {
        printf("element %d cannot be added to %d-th position\n", elem, index);
    } else {
        printf("element %d was successfully added to %d-th position\n", elem, index);
    }
    
    elem = 20;
    index = TEMPLATE(size_list, int)(list_int) + 1;
    if (TEMPLATE(add_elem_by_index, int)(list_int, index, elem)) {
        printf("element %d cannot be added to %d-th position\n", elem, index);
    } else {
        printf("element %d was successfully added to %d-th position\n", elem, index);
    }

    
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_remove_element_by_index() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(add_to_list, int)(list_int, 4);
    TEMPLATE(print_list, int)(list_int);
 
    int index = 100;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    putchar('\n');
  
    index = 0;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    putchar('\n');
    
    index = 2;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    putchar('\n');
    
    index = 0;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    putchar('\n');
    
    index = 0;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    putchar('\n');
    
    index = 0;
    if (TEMPLATE(remove_elem_by_index, int)(list_int, index)) {
        printf("cannot remove %d-th element\n", index);
    } else {
        printf("%d-th element was successfully removed\n", index);
    }
    printf("after removing: \n");
    TEMPLATE(print_list, int)(list_int);
    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

int test_int_dyn_array() {
    // create2 not work because two free: first in dyn array destroy2 second in destroy_list (free(node))
    // !!! use create for dyn array creation

    // create first element
    TEMPLATE(LIST, dyn_array_int)* list_dyn_array_int = TEMPLATE(create_list, dyn_array_int)();
    TEMPLATE(DYN_ARRAY, int) array_int;
    TEMPLATE(create, int)(4, &array_int);
    TEMPLATE(append, int)(&array_int, 1);
    TEMPLATE(append, int)(&array_int, 2);
    TEMPLATE(append, int)(&array_int, 3);
    TEMPLATE(append, int)(&array_int, 4);
    TEMPLATE(append, int)(&array_int, 5);
    TEMPLATE(append, int)(&array_int, 6);
    TEMPLATE(append, int)(&array_int, 7);
    TEMPLATE(print, int)(&array_int);
    
    // add first element
    TEMPLATE(add_to_list, dyn_array_int)(list_dyn_array_int, array_int);
    
    int index = 0;
    TEMPLATE(DYN_ARRAY, int) elem;
    TEMPLATE(get_by_index, dyn_array_int)(list_dyn_array_int, index, &elem);
    TEMPLATE(append, int)(&elem, 80);
    TEMPLATE(append, int)(&elem, 90);
    TEMPLATE(insert_by_index, dyn_array_int)(list_dyn_array_int, index, elem);
    
    // create second element
    TEMPLATE(DYN_ARRAY, int) array_int2;
    TEMPLATE(create, int)(4, &array_int2);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    TEMPLATE(append, int)(&array_int2, 1);
    
    // add second element
    TEMPLATE(add_to_list, dyn_array_int)(list_dyn_array_int, array_int2);
    index = 1;
    TEMPLATE(DYN_ARRAY, int) elem2;
    TEMPLATE(get_by_index, dyn_array_int)(list_dyn_array_int, index, &elem2);
    TEMPLATE(append, int)(&elem2, 20);
    TEMPLATE(append, int)(&elem2, 30);
    TEMPLATE(insert_by_index, dyn_array_int)(list_dyn_array_int, index, elem2);

    TEMPLATE(print_list, dyn_array_int)(list_dyn_array_int);
    TEMPLATE(destroy_list, dyn_array_int)(list_dyn_array_int);
    return 0;
}

int test_vop_dyn_array() {
    // create two vop dyn arrays and add it to the list
    // for all vop dyn arrays you must create only one array types
    // share it! 
    TEMPLATE(LIST, dyn_array_vop_st) *list_dyn_array_vop = TEMPLATE(create_list, dyn_array_vop_st)();
    
    // create first element
    TEMPLATE(DYN_ARRAY, vop_st) array_vop;
    TEMPLATE(create, vop_st)(4, &array_vop);
    int a = 1;
    TEMPLATE(append, vop_st)(&array_vop, (void *)&a);
    int b = 2;
    TEMPLATE(append, vop_st)(&array_vop, (void *)&b);
    char *c = "first\0";
    TEMPLATE(append, vop_st)(&array_vop, (void *)c);
    char *d = "second\0";
    TEMPLATE(append, vop_st)(&array_vop, (void *)d);
    
    int types[4] = {INT, INT, STRING, STRING};
    for (int i = 0; i < 4; i++)
        TEMPLATE(append, int)(array_vop.types, types[i]);  

    TEMPLATE(print, vop_st)(&array_vop);
    
    // add first element
    TEMPLATE(add_to_list, dyn_array_vop_st)(list_dyn_array_vop, array_vop);
    
    int index = 0;
    TEMPLATE(DYN_ARRAY, vop_st) elem;
    TEMPLATE(get_by_index, dyn_array_vop_st)(list_dyn_array_vop, index, &elem);
    int s = 80;
    char *t = "third\0";
    TEMPLATE(append, vop_st)(&elem, (void *)&s);
    TEMPLATE(append, vop_st)(&elem, (void *)t);
    TEMPLATE(append, int)(elem.types, INT);
    TEMPLATE(append, int)(elem.types, STRING);
    TEMPLATE(insert_by_index, dyn_array_vop_st)(list_dyn_array_vop, index, elem);
    
    // create second element
    TEMPLATE(DYN_ARRAY, vop_st) array_vop2;
    TEMPLATE(create, vop_st)(4, &array_vop2);
    
    int p = 100;
    TEMPLATE(append, vop_st)(&array_vop2, (void *)&p);
    int q = -123;
    TEMPLATE(append, vop_st)(&array_vop2, (void *)&q);
    char *r = "hello\0";
    TEMPLATE(append, vop_st)(&array_vop2, (void *)r);
    char *m = "world\0";
    TEMPLATE(append, vop_st)(&array_vop2, (void *)m);
    
    printf("append to array_vop2.types\n");
    int types2[4] = {INT, INT, STRING, STRING};
    for (int i = 0; i < 4; i++) {
        TEMPLATE(append, int)(array_vop2.types, types2[i]);
    }
    //array_vop2.types = array_vop.types;
    TEMPLATE(print, vop_st)(&array_vop2);

    // add second element
    TEMPLATE(add_to_list, dyn_array_vop_st)(list_dyn_array_vop, array_vop2);
    
 
    TEMPLATE(print_list, dyn_array_vop_st)(list_dyn_array_vop);
    TEMPLATE(destroy_list, dyn_array_vop_st)(list_dyn_array_vop);
    return 0;
}

typedef int (*f)();

typedef struct {
    char* name;
    f test; 
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[12] = {
        {"Test creation for list with int values", test_creation_int_list},
        {"Test empty list verification", test_empty_list_verification},
        {"Test checking presence of element in list", test_contains_in_list},
        {"Test int vec2", test_vec2_list},
        {"Test removing from list", test_removing_from_list},
        {"Test getting element by index", test_get_elem_by_index},
        {"Test insert element by index", test_insert_elem_by_index},
        {"Test indexOf", test_indexOf},
        {"Test adding element by index", test_add_element_by_index},
        {"Test removing element by index", test_remove_element_by_index},
        {"Test list of dynamic arrays", test_int_dyn_array},
        {"Test list with void* dynamic arrays", test_vop_dyn_array}
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

