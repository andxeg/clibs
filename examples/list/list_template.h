#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM

#include "types.h"

typedef struct TEMPLATE(LIST_NODE, TYPE_NAME) TEMPLATE(LIST_NODE, TYPE_NAME);

struct TEMPLATE(LIST_NODE, TYPE_NAME) {
    T data;
    TEMPLATE(LIST_NODE, TYPE_NAME) *next;
    TEMPLATE(LIST_NODE, TYPE_NAME) *prev;
};

typedef TEMPLATE(LIST_NODE, TYPE_NAME) TEMPLATE(LIST, TYPE_NAME);


TEMPLATE(LIST, TYPE_NAME)* TEMPLATE(create_list, TYPE_NAME)();
void TEMPLATE(destroy_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list);
int TEMPLATE(is_empty_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list);
int TEMPLATE(size_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list);
int TEMPLATE(contains_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem);

// TEMPLATE(DYN_ARRAY, T) search(TEMPLATE(LIST, TYPE_NAME) *list, void **template, int types); 
// search goods by template
// template is array of void *
// each element can be char*, int, vec2, vec3
// char* - regex
// int - concrete value
// vec2 - interval [a, b]
// return array of founded element

int TEMPLATE(add_to_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem);
int TEMPLATE(remove_from_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem); // delete first node with data == elem
int TEMPLATE(get_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T* result);
int TEMPLATE(indexOf, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem);
void TEMPLATE(print_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list);


int TEMPLATE(add_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T elem);
int TEMPLATE(remove_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index);

#endif
#endif
#endif

