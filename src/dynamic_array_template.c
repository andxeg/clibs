#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM
#include "templates.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array_types.h"

int TEMPLATE(create, TYPE_NAME) (int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    a->length = 0;
    a->capacity = size;
    a->data = (T *) calloc(size, sizeof(T));
    if (a->data == NULL) {
        fprintf(stderr, "create: error in calloc\n");
    }
    return 0;
}

void TEMPLATE(destroy, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    free(a->data);
    a->data = NULL;    
}

int TEMPLATE(is_empty, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->length == 0 ? 1 : 0; 
}

int TEMPLATE(get, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, int index, T *result) {
    if (a->length < (index + 1))   {
        fprintf(stderr, "get:index out of range\n");
        return 1;    
    }
    *result = a->data[index];
    return 0;
}

int TEMPLATE(insert, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, int index, T elem) {
    if (a->length < (index + 1)) {
        fprintf(stderr, "insert: index out of range\n");
        return 1;
    }
    a->data[index] = elem;
    return 0;
}
 
int TEMPLATE(append, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, T elem) {
    if (a->length < a->capacity) {
        a->data[a->length] = elem;
        a->length++;
        printf("append: success\n");
        return 0;
    }

    int new_cap = a->capacity * INCREASE_FACTOR; 
    T* p = (T*) realloc(a->data, new_cap * sizeof(T));
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
 
void TEMPLATE(print, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    printf("len: %d\n", a->length);
    printf("cap: %d\n", a->capacity);
    for (int i = 0; i < a->length; i++) {
    #if TYPE_NUM == INT || TYPE_NUM == GOOD_FIELD_TYPE
        printf("%d ", a->data[i]);
    #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE
        printf("%f ", a->data[i]);
    #elif TYPE_NUM == STRING
        printf("%s ", a->data[i]);
    #else
        printf("%d ", a->data[i]);
    #endif
    }
    putchar('\n');
}

#endif
#endif
#endif

