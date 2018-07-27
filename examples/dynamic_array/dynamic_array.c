#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

int create(int size, DYN_ARRAY_INT *a) {
    a->length = 0;
    a->capacity = (size <= 0) ? 1 : size;
    a->data = (int *) calloc(size, sizeof(int));
    if (a->data == NULL) {
        fprintf(stderr, "create: error in calloc");
        return 1;
    }
    return 0;
}

void destroy(DYN_ARRAY_INT *a) {
    free(a->data);
    a->data = NULL;
}

int is_empty(DYN_ARRAY_INT *a) {
    return a->length == 0 ? 1 : 0;
}

int get(DYN_ARRAY_INT *a, int index, int *result) {
    if (a->length < (index + 1)) {
        fprintf(stderr, "get: index out of range\n");
        return 1;
    }
    *result = a->data[index];
    return 0;
}

int insert(DYN_ARRAY_INT *a, int index, int elem) {
    if (a->length < (index + 1)) {
        fprintf(stderr, "insert: index out of range\n");
        return 1;
    }
    a->data[index] = elem;
    return 0;
}

int append(DYN_ARRAY_INT *a, int elem) {
    if (a->length < a->capacity) {
        a->data[a->length] = elem;
        a->length++;
        printf("append: success\n");
        return 0;
    }

    int new_cap = a->capacity * INCREASE_FACTOR; 
    int *p = (int *) realloc(a->data, new_cap * sizeof(int));
    if (p == NULL) {
        fprintf(stderr, "append: error in realloc");
        return 1;
    }
    printf("append: realloc was success\n");
    a->data = p;
    a->data[a->length] = elem;
    a->length++;
    a->capacity = new_cap;
    printf("append: success\n");
    return 0; 
} 

void print(DYN_ARRAY_INT *a) {
    printf("len: %d\n", a->length);
    printf("capacity: %d\n", a->capacity);
    for (int i = 0; i < a->length; i++) {
        printf("%d ", a->data[i]);
    }
    putchar('\n');
}

