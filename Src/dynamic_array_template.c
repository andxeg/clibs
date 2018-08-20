
#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "log.h"
#include "defs.h"
#include "templates.h"
#include "dynamic_array_types.h"
#include "oem_public_def.h"     // ufree, umalloc


TEMPLATE(DYN_ARRAY, TYPE_NAME)* TEMPLATE(create2, TYPE_NAME) (int size) {
    TEMPLATE(DYN_ARRAY, TYPE_NAME)* result = (TEMPLATE(DYN_ARRAY, TYPE_NAME)*) umalloc(sizeof(TEMPLATE(DYN_ARRAY, TYPE_NAME)));
    if (result == NULL) {
        ELOG("create2: error in malloc");
        return NULL;
    }

    if (TEMPLATE(create, TYPE_NAME)(size, result)) {
        ELOG("create2: error in dyn array creation");
        ufree(result);
        return NULL;
    }

    return result;
}

void TEMPLATE(destroy2, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    if (a != NULL) {
        TEMPLATE(destroy, TYPE_NAME)(a);
        ufree(a);
        a = NULL;
    }
}

int TEMPLATE(create, TYPE_NAME) (int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
    a->length = 0;
    if (size <= 0) {
        ELOG("create: array size is not positive, set capacity to 1");
        return 1;
    }
    a->capacity = (size <= 0) ? 1 : size;

    T* p = (T *) calloc(size, sizeof(T));
    if (p == NULL) {
        ELOG("create: calloc error, cannot allocate memory");
        return 1;
    }
    a->data = p;

    #if TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
    TEMPLATE(DYN_ARRAY, int)* array_int = TEMPLATE(create2, int)(size);
    if (array_int == NULL) {
        ELOG("create: error while create array for types");
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
    	int i;
        for (i = 0; i < a->length; i++) {
            free(a->data[i]);
        }
        #endif
        ufree(a->data);
        a->data = NULL;
    }

    #if TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
    if (a->types != NULL) {
        if (a->types->data != NULL) {
            TEMPLATE(destroy2, int)(a->types);
        }
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
        ELOG("get:index out of range");
        return 1;
    }
    *result = a->data[index];
    return 0;
}

int TEMPLATE(insert, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, int index, T elem) {
    if (a->length < (index + 1)) {
        ELOG("insert: index out of range");
        return 1;
    }
#if TYPE_NUM == STRING || TYPE_NUM == VOIDP
    free(a->data[index]);
#endif
    a->data[index] = elem;
    return 0;
}

int TEMPLATE(append, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, T elem) {

    if (a->length < a->capacity) {
        a->data[a->length] = elem;
        a->length++;
        return 0;
    }

    int new_cap = a->capacity * INCREASE_FACTOR;
    T* p = (T*) realloc(a->data, new_cap * sizeof(T));
    if (p == NULL) {
        ELOG("append: error in realloc");
        return 1;
    }

    a->data = p;
    a->data[a->length] = elem;
    a->length++;
    a->capacity = new_cap;
    return 0;
}

// don't use
// write to file line by line
// early was printf
void TEMPLATE(print, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a) {
	char msg[LOG_MSG_LENGTH_LIMIT];

    sprintf(msg, "len: %d cap: %d [", a->length, a->capacity); PRINT_TO_LOG(msg);

    int i;
    for (i = 0; i < a->length-1; i++) {
    #if TYPE_NUM == CHAR
        sprintf(msg, "%c, ", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == INT || TYPE_NUM == GOOD_FIELD_TYPE || TYPE_NUM == BOOL
        sprintf(msg, "%d, ", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE_TYPE
        sprintf(msg, "%f, ", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        sprintf(msg, "%s, ", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
        int type = 0;
        switch (TEMPLATE(get, int)(a->types, i, &type), type) {
            case INT:
            case GOOD_FIELD:
            case BOOL:
                sprintf(msg, "%d, ", *(int *)a->data[i]); PRINT_TO_LOG(msg);
		break;
	    case FLOAT:
	    case DOUBLE_TYPE:
                sprintf(msg, "%f, ", *(double *)a->data[i]); PRINT_TO_LOG(msg);
                break;
            case STRING:
                sprintf(msg, "%s, ", (char *)a->data[i]); PRINT_TO_LOG(msg);
	}
    #else
        sprintf(msg, "%d, ", a->data[i]); PRINT_TO_LOG(msg);
    #endif
    }

    if (a->length >= 1) {
    #if TYPE_NUM == CHAR
        sprintf(msg, "%c", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == INT || TYPE_NUM == GOOD_FIELD_TYPE || TYPE_NUM == BOOL
        sprintf(msg, "%d", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE_TYPE
        sprintf(msg, "%f", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        sprintf(msg, "%s", a->data[i]); PRINT_TO_LOG(msg);
    #elif TYPE_NUM == VOIDP || TYPE_NUM == VOIDP_ST
        int type = 0;
        switch (TEMPLATE(get, int)(a->types, i, &type), type) {
            case INT:
            case GOOD_FIELD:
            case BOOL:
                sprintf(msg, "%d", *(int *)a->data[i]); PRINT_TO_LOG(msg);
		break;
	    case FLOAT:
	    case DOUBLE_TYPE:
                sprintf(msg, "%f", *(double *)a->data[i]); PRINT_TO_LOG(msg);
                break;
            case STRING:
                sprintf(msg, "%s", (char *)a->data[i]); PRINT_TO_LOG(msg);
	}
    #else
        sprintf(msg, "%d", a->data[i]); PRINT_TO_LOG(msg);
    #endif
    }

    sprintf(msg, "]\n"); PRINT_TO_LOG(msg);
}

int TEMPLATE(array_equal, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *first, TEMPLATE(DYN_ARRAY, TYPE_NAME) *second) {
    if (first->length != second->length) {
        return 0;
    }

    int i;
    #if TYPE_NUM != VOIDP
    // equal types
    for (i = 0; i < first->length; i++) {
        #if TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        if (strcmp(first->data[i], second->data[i]) != 0) {
        #else
        if (first->data[i] != second->data[i]) {
        #endif
            return 0;
        }
    }
    #else
    // different types
    for (i = 0; i < first->length; i++) {
        int type1;
        int type2;
        TEMPLATE(get, int)(first->types, i, &type1);
        TEMPLATE(get, int)(second->types, i, &type2);
        if (type1 != type2) {
            return 0;
        }
        switch (type1) {
            case INT:
            case GOOD_FIELD:
                if (*(int *)first->data[i] != *(int *)second->data[i]) {
                    return 0;
                }
                break;
			case FLOAT:
				if (*(float *)first->data[i] != *(float *)second->data[i]) {
					return 0;
				}
				break;
			case DOUBLE_TYPE:
				if (*(double *)first->data[i] != *(double *)second->data[i]) {
					return 0;
				}
				break;
			case STRING:
			case STRING_ST:
				if (strcmp((char *)first->data[i], (char *)second->data[i]) != 0) {
					return 0;
				}
				break;
			default:
				// unknown type
				// print error
				return -1;
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
        ELOG("shrink empty array is not permitted");
        return 1;
    }

    T* p = (T *) realloc(a->data, sizeof(T) * a->length);
    if (p == NULL) {
        ELOG("error while realloc");
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
    if (a->length != pattern->length) {
        ELOG("template and array has different size");
        return 0;
    }

    int i;
    int type1, type2;
    int *val1, *val2;
    char *str1, *str2;
    for (i = 0; i < a->length; i++) {
        TEMPLATE(get, int)(a->types, i, &type1);
        TEMPLATE(get, int)(pattern->types, i, &type2);

        if (type1 == INT || type1 == BOOL) {
            TEMPLATE(get, vop)(a, i, (void**)(&val1));
        } else if (type1 == STRING) {
            TEMPLATE(get, vop)(a, i, (void**)(&str1));
        } else {
            ELOG("unknown type in array");
            return 0;
        }

        if (type2 == INT || type2 == BOOL) {
            TEMPLATE(get, vop)(pattern, i, (void**)(&val2));
        } else if (type2 == STRING) {
            TEMPLATE(get, vop)(pattern, i, (void**)(&str2));
        } else {
            ELOG("unknown type in template");
            return 0;
        }


        if ((type1 == INT && type2 == INT) ||
        	(type1 == BOOL && type2 == BOOL)) {
            if (*val1 != *val2) return 0;
        } else if ((type1 == INT || type1 == BOOL) && type2 == STRING) {
            if (strcmp(str2, "*\0") != 0) return 0;
        } else if ((type1 == INT && type2 == BOOL) ||
        		   (type1 == STRING && type2 == INT) ||
				   (type1 == STRING && type2 == BOOL) ||
				   (type1 == BOOL && type2 == INT)) {
            return 0;
        } else {
            // type1 == STRING && type2 == STRING
//            if (strcmp(str2, "*\0") == 0) continue;
//            if (strcmp(str1, str2) != 0) return 0;
        	if (string_match(str1, str2)) {
        		continue;
        	} else {
        		return 0;
        	}
        }

    }
    #endif

    return 1;
}


#endif
#endif
#endif

