#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM
#include "defs.h"
#include "templates.h"

#define INCREASE_FACTOR 2

/*
#if TYPE_NUM != INT
typedef struct TEMPLATE(DYN_ARRAY, int) TEMPLATE(DYN_ARRAY, int);
#else
typedef struct TEMPLATE(DYN_ARRAY, TYPE_NAME) TEMPLATE(DYN_ARRAY, TYPE_NAME);
#endif
*/

// lines below work in linux (gcc 7.3.0) but don't work in windows
//typedef struct TEMPLATE(DYN_ARRAY, vop) TEMPLATE(DYN_ARRAY, vop);
//typedef struct TEMPLATE(DYN_ARRAY, TYPE_NAME) TEMPLATE(DYN_ARRAY, TYPE_NAME);


struct TEMPLATE(DYN_ARRAY, TYPE_NAME) {
    T* data;
    int length;
    int capacity;
    TEMPLATE(DYN_ARRAY, int)* types;
};

TEMPLATE(DYN_ARRAY, TYPE_NAME)* TEMPLATE(create2, TYPE_NAME) (int size);
void TEMPLATE(destroy2, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(create, TYPE_NAME) (int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
void TEMPLATE(destroy, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(is_empty, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(array_length, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(array_capacity, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(get, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, int index, T *result);
int TEMPLATE(append, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, T elem);
int TEMPLATE(insert, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, int index, T elem);
void TEMPLATE(print, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(array_equal, TYPE_NAME) (TEMPLATE(DYN_ARRAY, TYPE_NAME) *first, TEMPLATE(DYN_ARRAY, TYPE_NAME) *second);
TEMPLATE(DYN_ARRAY, int)* TEMPLATE(get_array_types, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(set_array_types, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a, TEMPLATE(DYN_ARRAY, int) *types);
T* TEMPLATE(get_raw_data, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(shrink_to_fit, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(recreate, TYPE_NAME)(int size, TEMPLATE(DYN_ARRAY, TYPE_NAME) *a);
int TEMPLATE(array_match, TYPE_NAME)(TEMPLATE(DYN_ARRAY, TYPE_NAME)* a, TEMPLATE(DYN_ARRAY, vop)* pattern);

#endif
#endif
#endif

