#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

int test_append() {
    DYN_ARRAY_INT array;
    if (create(1, &array) == 1) {
        printf("error in creation");
        return 1;
    } 

    print(&array);
    append(&array, 10);
    append(&array, 12);
    print(&array);
    append(&array, 13);
    append(&array, 14);
    print(&array);
    append(&array, -1);
    print(&array);
    destroy(&array);
    return 0;
}

int test_empty() {
    DYN_ARRAY_INT array;
    if (create(1, &array) == 1) {
        printf("error in creation");
        return 1;
    } 
    
    if (is_empty(&array))
        printf("array is empty\n");
    else
        printf("array is not empty\n");


    print(&array);
    append(&array, 10);
    append(&array, 12);
    print(&array);
    
    if (is_empty(&array))
        printf("array is empty\n");
    else
        printf("array is not empty\n");

    append(&array, 13);
    append(&array, 14);
    print(&array);
    append(&array, -1);
    print(&array);

    if (is_empty(&array))
        printf("array is empty\n");
    else
        printf("array is not empty\n");
    
    destroy(&array);
    return 0;
}

int test_insert() {
    DYN_ARRAY_INT array;
    if (create(1, &array) == 1) {
        printf("error in creation");
        return 1;
    } 

    append(&array, 10);
    append(&array, 12);
    append(&array, 13);
    append(&array, 14);
    print(&array);
    insert(&array, 0, -10);
    insert(&array, 1, -12);
    print(&array);
    append(&array, -1);
    print(&array);
    insert(&array, 4, 1);
    print(&array);
    insert(&array, 5, 100);
    print(&array);
    destroy(&array);
    return 0;
}

int test_get() {
    DYN_ARRAY_INT array;
    if (create(1, &array) == 1) {
        printf("error in creation");
        return 1;
    } 

    print(&array);
    append(&array, 10);
    append(&array, 12);
    append(&array, 13);
    append(&array, 14);
    append(&array, -1);
    int elem1 = -100;
    int elem2 = -100;
    int elem3 = -100;
    int elem4 = -100;
    get(&array, 0, &elem1);
    get(&array, 3, &elem2);
    get(&array, 4, &elem3);
    get(&array, 5, &elem4);
    printf("%d, %d, %d, %d\n", elem1, elem2, elem3, elem4);
    print(&array);
    destroy(&array);
    return 0;
}

int main(int argc, char **argv) {
    printf("\nTEST APPEND\n");
    test_append();

    printf("\nTEST EMPTY\n");
    test_empty();

    printf("\nTEST INSERT\n");
    test_insert();
    
    printf("\nTEST GET\n");
    test_get();
    return 0;
}

