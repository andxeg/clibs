#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM
#include "templates.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array_types.h"

TEMPLATE(DYN_ARRAY, TYPE_NAME)* TEMPLATE(create2, TYPE_NAME) (int size) {
    TEMPLATE(DYN_ARRAY, TYPE_NAME)* result = (TEMPLATE(DYN_ARRAY, TYPE_NAME)*)malloc(sizeof(TEMPLATE(DYN_ARRAY, TYPE_NAME)));
    if (result == NULL) {
        fprintf(stderr, "create2: error in malloc\n");
        return NULL;
    }
    
    if (TEMPLATE(create, TYPE_NAME)(size, result)) {
        fprintf(stderr, "create2: error in dyn array creation\n");
        free(result);
        return NULL;
    }
    
    return result;
}

void TEMPLATE(destroy2, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    if (a != NULL) {
        TEMPLATE(destroy, TYPE_NAME)(a);
        free(a);
        a = NULL;
    }
}

int TEMPLATE(create, TYPE_NAME) (int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    a->length = 0;
    if (size <= 0) {
        fprintf(stderr, "create: array size is not positive, set capacity to 1\n");
        return 1;
    }
    a->capacity = (size <= 0) ? 1 : size;

    T* p = (T *) calloc(size, sizeof(T));
    if (p == NULL) {
        fprintf(stderr, "create: calloc error, cannot allocate memory\n");
        return 1;
    }
    a->data = p;

    #if TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
    TEMPLATE(DYN_ARRAY, int)* array_int = TEMPLATE(create2, int)(size);
    if (array_int == NULL) {
        fprintf(stderr, "create: error while create array for types\n");
        return 1;
    }
    a->types = array_int;
    #else
    a->types = NULL;
    #endif 
    return 0;
}

void TEMPLATE(destroy, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    if (a->data != NULL) {
        #if TYPE_NUM == STRING || TYPE_NUM == VOIDP
        for (int i = 0; i < a->length; i++) {
            free(a->data[i]);
        }
        #endif
        free(a->data);
        a->data = NULL;
    }
    
    #if TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST 
    if (a->types != NULL) {
        if (a->types->data != NULL) {
            TEMPLATE(destroy2, int)(a->types);
        }
        //free(a->types);
        a->types = NULL;
    }
    #endif
}

int TEMPLATE(is_empty, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->length == 0 ? 1 : 0; 
}

int TEMPLATE(array_length, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->length;
}

int TEMPLATE(array_capacity, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->capacity;
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
    printf("len: %d cap: %d ", a->length, a->capacity);
    putchar('[');
    int i;
    for (i = 0; i < a->length-1; i++) {
    #if TYPE_NUM == CHAR
        printf("%c, ", a->data[i]);
    #elif TYPE_NUM == INT || TYPE_NUM == GOOD_FIELD_TYPE || TYPE_NUM == BOOL
        printf("%d, ", a->data[i]);
    #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE
        printf("%f, ", a->data[i]);
    #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        printf("%s, ", a->data[i]);
    #elif TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
        int type = 0;
        switch (TEMPLATE(get, int)(a->types, i, &type), type) {
            case INT:
            case GOOD_FIELD:
            case BOOL:
                printf("%d, ", *(int *)a->data[i]);
		break;
	    case FLOAT:
	    case DOUBLE:
                printf("%f, ", *(double *)a->data[i]);
                break;
            case STRING:
                printf("%s, ", (char *)a->data[i]);
	}
    #else
        printf("%d, ", a->data[i]);
    #endif
    }

    if (a->length >= 1) {
    #if TYPE_NUM == CHAR
        printf("%c", a->data[i]);
    #elif TYPE_NUM == INT || TYPE_NUM == GOOD_FIELD_TYPE || TYPE_NUM == BOOL
        printf("%d", a->data[i]);
    #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE
        printf("%f", a->data[i]);
    #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        printf("%s", a->data[i]);
    #elif TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
        int type = 0;
        switch (TEMPLATE(get, int)(a->types, i, &type), type) {
            case INT:
            case GOOD_FIELD:
            case BOOL:
                printf("%d", *(int *)a->data[i]);
		break;
	    case FLOAT:
	    case DOUBLE:
                printf("%f", *(double *)a->data[i]);
                break;
            case STRING:
                printf("%s", (char *)a->data[i]);
	}
    #else
        printf("%d", a->data[i]);
    #endif
    }

    printf("]\n");    
}

int TEMPLATE(array_equal, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *first, TEMPLATE(DYN_ARRAY, TYPE_NAME) *second) {
    if (first->length != second->length) {
        printf("array_equal: arrays have different lengths\n");
        return 0;
    }
    
    #if TYPE_NUM != VOIDP
    // equal types
    for (int i = 0; i < first->length; i++) {
        #if TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        if (strcmp(first->data[i], second->data[i]) != 0) {
        #else
        if (first->data[i] != second->data[i]) {
        #endif
            printf("array_equal: %d-th elements have different types\n", i+1);
            return 0;
        }
    }
    #else 
    // different types
    for (int i = 0; i < first->length; i++) {
        int type1;
        int type2;
        TEMPLATE(get, int)(first->types, i, &type1);
        TEMPLATE(get, int)(second->types, i, &type2);
        if (type1 != type2) {
            printf("array_equal: arrays has different type in %d-th element\n", i+1);  
            return 0;
        }
        switch (type1) {
            case INT:
            case GOOD_FIELD:
                if (*(int *)first->data[i] != *(int *)second->data[i]) {
                    printf("array_equal: arrays are different in %d-th element\n", i+1);
                    return 0;
                }
		break;
	    case FLOAT:
                if (*(float *)first->data[i] != *(float *)second->data[i]) {
                    printf("array_equal: arrays are different in %d-th element\n", i+1);
                    return 0;
                }
		break;
	    case DOUBLE:
                if (*(double *)first->data[i] != *(double *)second->data[i]) {
                    printf("array_equal: arrays are different in %d-th element\n", i+1);
                    return 0;
                }
		break;
            case STRING:
            case STRING_ST:
                if (strcmp((char *)first->data[i], (char *)second->data[i]) != 0) {
                    printf("array_equal: arrays are different in %d-th element\n", i+1);
                    return 0;
                }
		break;
	}
    }
    #endif

    return 1;
}

TEMPLATE(DYN_ARRAY, int)* TEMPLATE(get_array_types, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->types;
}

int TEMPLATE(set_array_types, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, TEMPLATE(DYN_ARRAY, int) *types) {
    if (a->types != NULL) {
        TEMPLATE(destroy2, int)(a->types);
    }
    
    a->types = types;
    return 0;
}

T* TEMPLATE(get_raw_data, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    return a->data;
}

int TEMPLATE(shrink_to_fit, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    if (a->length == 0) {
        fprintf(stderr, "shrink_to_fit: shrink empty array is not permitted\n");
        return 1;
    }

    T* p = (T *) realloc(a->data, sizeof(T) * a->length);
    if (p == NULL) {
        fprintf(stderr, "shrink_to_fit: error while realloc\n");
        return 1;
    }
        
    a->data = p;    
    a->capacity = a->length;
    return 0;
}

int TEMPLATE(recreate, TYPE_NAME)(int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    TEMPLATE(destroy, TYPE_NAME)(a);
    if (TEMPLATE(create, TYPE_NAME)(size, a)) {
        return 1;
    }

    return 0;      
}

// work only for INT and STRING
// 0 - not match
// 1 - match
int TEMPLATE(array_match, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME)* a, TEMPLATE(DYN_ARRAY, vop)* pattern) {
    #if TYPE_NUM == VOIDP
    printf("array_match for VOIDP\n ");
    if (a->length != pattern->length) {
        fprintf(stderr, "template and array has different size\n");
        return 0;
    }
    
    int i;
    int type1, type2;
    int *val1, *val2;
    char *str1, *str2;
    for (i = 0; i < a->length; i++) {
        TEMPLATE(get, int)(a->types, i, &type1);
        TEMPLATE(get, int)(pattern->types, i, &type2);
	
        if (type1 == INT) {
            TEMPLATE(get, vop)(a, i, (void**)(&val1));
        } else if (type1 == STRING) {
            TEMPLATE(get, vop)(a, i, (void**)(&str1));
        } else {
            fprintf(stderr, "array_match: unknown type in array\n");
            return 0;
        }

        if (type2 == INT) {
            TEMPLATE(get, vop)(pattern, i, (void**)(&val2));
        } else if (type2 == STRING) {
            TEMPLATE(get, vop)(pattern, i, (void**)(&str2));
        } else {
            fprintf(stderr, "array_match: unknown type in template\n");
            return 0;
        }
        
        if (type1 == INT && type2 == INT) {
            printf("number field first -> %d, number second -> %d\n", *val1, *val2);
            if (*val1 != *val2) return 0;
        } else if (type1 == INT && type2 == STRING) {
            printf("number field first -> %d, string second -> %s\n", *val1, str2);
            if (strcmp(str2, "*\0") != 0) return 0;  
        } else if (type1 == STRING && type2 == INT) {
            printf("string field first -> %s, number second -> %d\n", str1, *val2);
            return 0;
        } else {
            // type1 == STRING && type2 == STRING
            printf("string field first -> %s, second -> %s\n", str1, str2);
            if (strcmp(str2, "*\0") == 0) continue;
            if (strcmp(str1, str2) != 0) return 0;
        }
        
    }
    #endif

    return 1;
}


#endif
#endif
#endif

