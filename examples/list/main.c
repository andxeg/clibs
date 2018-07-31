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
    TEMPLATE(LIST, string)* list_string = TEMPLATE(create_list, string)();
    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string)(list_string));
    TEMPLATE(add_to_list, string)(list_string, "first\0");
    TEMPLATE(add_to_list, string)(list_string, "second\0");
    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string)(list_string));
    TEMPLATE(print_list, string)(list_string);
    TEMPLATE(destroy_list, string)(list_string);
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
    TEMPLATE(add_elem_by_index, int_vec2_st)(list_int_vec2, index, d);
    
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
    TEMPLATE(LIST, string)* list_string = TEMPLATE(create_list, string)();
    TEMPLATE(add_to_list, string)(list_string, "first\0");
    TEMPLATE(add_to_list, string)(list_string, "second\0");
    TEMPLATE(add_to_list, string)(list_string, "third\0");
    TEMPLATE(print_list, string)(list_string);
    
    char* a;
    int index = 2;
    if (TEMPLATE(get_by_index, string)(list_string, index, &a)) {
        printf("cannot find %d-th in list\n", index);
        return 1;
    } else {
        printf("%d-th element -> %s\n", index, a);
    }

    TEMPLATE(remove_from_list, string)(list_string, "third\0");
    TEMPLATE(print_list, string)(list_string);
    
    index = 0;
    if (TEMPLATE(get_by_index, string)(list_string, index, &a)) {
        printf("cannot find %d-th in list\n", index);
        return 1;
    } else {
        printf("%d-th element -> %s\n", index, a);
    }



    TEMPLATE(remove_from_list, string)(list_string, "first\0");
    TEMPLATE(print_list, string)(list_string);
    TEMPLATE(remove_from_list, string)(list_string, "second\0");
    TEMPLATE(print_list, string)(list_string);

    printf("empty value -> %d\n", TEMPLATE(is_empty_list, string)(list_string));
    TEMPLATE(print_list, string)(list_string);
    TEMPLATE(destroy_list, string)(list_string);
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

typedef int (*f)();

typedef struct {
    char* name;
    f test; 
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[9] = {
        {"Test creation for list with int values", test_creation_int_list},
        {"Test empty list verification", test_empty_list_verification},
        {"Test checking presence of element in list", test_contains_in_list},
        {"Test int vec2", test_vec2_list},
        {"Test removing from list", test_removing_from_list},
        {"Test getting element by index", test_get_elem_by_index},
        {"Test indexOf", test_indexOf},
        {"Test adding element by index", test_add_element_by_index},
        {"Test removing element by index", test_remove_element_by_index}
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

