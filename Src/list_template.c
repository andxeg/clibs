#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "log.h"
#include "types.h"
#include "templates.h"
#include "dynamic_arrays.h"

TEMPLATE(LIST, TYPE_NAME)* TEMPLATE(create_list, TYPE_NAME)() {
    TEMPLATE(LIST, TYPE_NAME)* result = (TEMPLATE(LIST, TYPE_NAME)*) umalloc(sizeof(TEMPLATE(LIST, TYPE_NAME)));
    if (result == NULL) {
        ELOG("error in malloc");
        return NULL;
    }

    result->next = result;
    result->prev = result;

    return result;
}

// delete only list node
// don't delete node data
void TEMPLATE(destroy_list_lite, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list) {
    while (list->next != list) {
        TEMPLATE(LIST_NODE, TYPE_NAME)* node = list->next;
        list->next = node->next;
        node->next->prev = list;
        node->next = node->prev = NULL;
        ufree(node);
    }

    ufree(list);
}

void TEMPLATE(destroy_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list) {
    while (list->next != list) {
        TEMPLATE(LIST_NODE, TYPE_NAME)* node = list->next;
        list->next = node->next;
        node->next->prev = list;
        node->next = node->prev = NULL;

        // free memory with internal data
        #if TYPE_NUM == INT_DYN_ARRAY
        TEMPLATE(destroy, int)(&node->data);
        #elif TYPE_NUM == STRING_DYN_ARRAY
        TEMPLATE(destroy, string)(&node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY
        TEMPLATE(destroy, vop)(&node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        TEMPLATE(destroy, vop_st)(&node->data);
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 || TYPE_NUM == STRING
        free(node->data);
        #endif

        ufree(node);
    }

    ufree(list);
}

int TEMPLATE(is_empty_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list) {
    return list->next == list && list->prev == list;
}

int TEMPLATE(size_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list) {
    int size = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        ++size;
        curr_node = curr_node->next;
    }
    return size;
}

int TEMPLATE(contains_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem) {
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        #if TYPE_NUM == INT_DYN_ARRAY
        if (TEMPLATE(array_equal, int)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == STRING_DYN_ARRAY
        if (TEMPLATE(array_equal, string)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY
        if (TEMPLATE(array_equal, vop)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        if (TEMPLATE(array_equal, vop_st)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1]) {
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] &&
            curr_node->data[2] == elem[2]) {
        #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        if (strcmp(curr_node->data, elem) == 0) {
        #else
        if (curr_node->data == elem) {
        #endif
            return 1;
        }
        curr_node = curr_node->next;
    }

    return 0;
}

// TEMPLATE(DYN_ARRAY, T) search(TEMPLATE(LIST, TYPE_NAME) *list, void **template, int types);
// search goods by template
// template is array of void *
// each element can be char*, int, vec2, vec3
// char* - regex
// int - concrete value
// vec2 - interval [a, b]
// return array of founded element

// NOTE:
// when use this function, destroy result list in your code with destroy_list_lite
// types in template array is differ from types in list
// because template can contain '*' (all values are matched)
// NOTE! Only for VOP_DYN_ARRAY work
TEMPLATE(LIST, TYPE_NAME)* TEMPLATE(search, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME)* list, TEMPLATE(DYN_ARRAY, vop)* pattern) {
	TEMPLATE(LIST, TYPE_NAME)* result = TEMPLATE(create_list, TYPE_NAME)();

	TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
	while (curr_node != list) {
		#if TYPE_NUM == INT_DYN_ARRAY
		{
		#elif TYPE_NUM == STRING_DYN_ARRAY
		{
		#elif TYPE_NUM == VOP_DYN_ARRAY
		if (TEMPLATE(array_match, vop)(&curr_node->data, pattern) == 1) {
		#elif TYPE_NUM == VOP_DYN_ARRAY_ST
		{
		#elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
		{
		#elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
		{
		#elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
		{
		#else
		{
		#endif
			TEMPLATE(add_to_list, TYPE_NAME)(result, curr_node->data);
		}

		curr_node = curr_node->next;
	}

	return result;
}

int TEMPLATE(add_to_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem) {
    TEMPLATE(LIST_NODE, TYPE_NAME)* last = list->prev;
    TEMPLATE(LIST_NODE, TYPE_NAME)* new_node = (TEMPLATE(LIST_NODE, TYPE_NAME)*) umalloc(sizeof(TEMPLATE(LIST_NODE, TYPE_NAME)));
    if (new_node == NULL) {
        ELOG("error in malloc");
        return 1;
    }

    new_node->data = elem;
    new_node->next = list;
    list->prev = new_node;
    new_node->prev = last;
    last->next = new_node;
    return 0;
}

int TEMPLATE(remove_from_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem) {
// delete first node with data == elem
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        #if TYPE_NUM == INT_DYN_ARRAY
        if (TEMPLATE(array_equal, int)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == STRING_DYN_ARRAY
        if (TEMPLATE(array_equal, string)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY
        if (TEMPLATE(array_equal, vop)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        if (TEMPLATE(array_equal, vop_st)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1]) {
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] &&
            curr_node->data[2] == elem[2]) {
        #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        if (strcmp(curr_node->data, elem) == 0) {
        #else
        if (curr_node->data == elem) {
        #endif
            TEMPLATE(LIST_NODE, TYPE_NAME)* prev = curr_node->prev;
            prev->next = curr_node->next;
            curr_node->next->prev = prev;
            curr_node->next = curr_node;
            curr_node->prev = curr_node;
            TEMPLATE(destroy_list, TYPE_NAME)(curr_node);
            return 0;
        }
        curr_node = curr_node->next;
    }

    return 1;
}

int TEMPLATE(get_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T *result) {
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        if (curr_index == index) {
            #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
                TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
              #if TYPE_NUM == INT_VEC2 || INT_VEC2_ST
            (*result)[0] = curr_node->data[0];
            (*result)[1] = curr_node->data[1];
              #elif TYPE_NUM == INT_VEC3 || INT_VEC3_ST
            (*result)[0] = curr_node->data[0];
            (*result)[1] = curr_node->data[1];
            (*result)[2] = curr_node->data[2]
              #endif
            #else
            // note for dyn_array len and cap copy as value
            // for modify element in list call get_by_index then
            // insert_by_index
            *result = curr_node->data;
            #endif
            return 0;
        }
        ++curr_index;
        curr_node = curr_node->next;
    }

    ELOG("index out of range");
    return 1;
}

int TEMPLATE(insert_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T elem) {
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME) *curr_node= list->next;
    while (curr_node != list) {
        if (curr_index == index) {
            // first delete old element if needed
            // then allocate memory if needed
            // and finaly set to index-th position new element
            #if TYPE_NUM == STRING
            free(curr_node->data);
            curr_node->data = elem;
            #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
                TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
              #if TYPE_NUM == INT_VEC2 || INT_VEC2_ST
            curr_node->data[0] = elem[0];
            curr_node->data[1] = elem[1];
              #elif TYPE_NUM == INT_VEC3 || INT_VEC3_ST
            curr_node->data[0] = elem[0];
            curr_node->data[1] = elem[1];
            curr_node->data[2] = elem[2];
              #endif
            #else
            curr_node->data = elem;
            #endif
            return 0;
        }
        ++curr_index;
        curr_node = curr_node->next;
    }

    ELOG("index out of range");
    return 1;
}

int TEMPLATE(indexOf, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem) {
    int index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        #if TYPE_NUM == INT_DYN_ARRAY
        if (TEMPLATE(array_equal, int)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == STRING_DYN_ARRAY
        if (TEMPLATE(array_equal, string)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY
        if (TEMPLATE(array_equal, vop)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        if (TEMPLATE(array_equal, vop_st)(&curr_node->data, &elem)) {
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1]) {
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] &&
            curr_node->data[2] == elem[2]) {
        #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        if (strcmp(curr_node->data, elem) == 0) {
        #else
        if (curr_node->data == elem) {
        #endif
            return index;
        }
        ++index;
        curr_node = curr_node->next;
    }

    return -1;
}

void TEMPLATE(print_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list) {
	char msg[LOG_MSG_LENGTH_LIMIT];

	TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    int length = TEMPLATE(size_list, TYPE_NAME)(list);
    sprintf(msg, "len: %d [", length); PRINT_TO_LOG(msg);
    while (curr_node->next != list)  {
        #if TYPE_NUM == INT_DYN_ARRAY
        TEMPLATE(print, int)(&curr_node->data);
        #elif TYPE_NUM == STRING_DYN_ARRAY
        TEMPLATE(print, string)(&curr_node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY
        TEMPLATE(print, vop)(&curr_node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        TEMPLATE(print, vop_st)(&curr_node->data);
        #elif TYPE_NUM == INT
        sprintf(msg, "%d, ", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE_TYPE
        sprintf(msg, "%f, ", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        sprintf(msg, "%s, ", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
              TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
        int len;
        #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        len = 2;
        #else
        len = 3;
        #endif
        sprintf(msg, "["); PRINT_TO_LOG(msg);
        int i;
        for (i = 0; i < len-1; i++) {
            sprintf(msg, "%d,", curr_node->data[i]); PRINT_TO_LOG(msg);
        }
        sprintf(msg, "%d], ", curr_node->data[len-1]); PRINT_TO_LOG(msg);
        #endif
        curr_node = curr_node->next;
    }

    if (length != 0) {
        #if TYPE_NUM == INT_DYN_ARRAY
        TEMPLATE(print, int)(&curr_node->data);
        #elif TYPE_NUM == STRING_DYN_ARRAY
        TEMPLATE(print, string)(&curr_node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY
        TEMPLATE(print, vop)(&curr_node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY_ST
        TEMPLATE(print, vop_st)(&curr_node->data);
        #elif TYPE_NUM == INT
        sprintf(msg, "%d", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE_TYPE
        sprintf(msg, "%f", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == STRING || TYPE_NUM == STRING_ST
        sprintf(msg, "%s", curr_node->data); PRINT_TO_LOG(msg);
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
              TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
        int len;
        #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        len = 2;
        #else
        len = 3;
        #endif
        sprintf(msg, "["); PRINT_TO_LOG(msg);
        int i;
        for (i = 0; i < len-1; i++) {
            sprintf(msg, "%d,", curr_node->data[i]); PRINT_TO_LOG(msg);
        }
        sprintf(msg, "%d]", curr_node->data[len-1]); PRINT_TO_LOG(msg);
        #endif
    }

    sprintf(msg, "]\n"); PRINT_TO_LOG(msg);
}


int TEMPLATE(add_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T elem) {
    if (index < 0) {
        ELOG("index is non-positive");
        return 1;
    }
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        if (curr_index == index) {
            TEMPLATE(LIST_NODE, TYPE_NAME)* prev = curr_node->prev;
            TEMPLATE(LIST_NODE, TYPE_NAME)* new_node = (TEMPLATE(LIST_NODE, TYPE_NAME)*) umalloc(sizeof(TEMPLATE(LIST_NODE, TYPE_NAME)));
            if (new_node == NULL) {
                ELOG("error in malloc");
                return 1;
            }

            new_node->data = elem;
            prev->next = new_node;
            new_node->prev = prev;
            new_node->next = curr_node;
            curr_node->prev = new_node;
            return 0;
        }
        ++curr_index;
        curr_node = curr_node->next;
    }

    if (index > curr_index) {
        ELOG("index out of range");
        return 1;
    }

    if (TEMPLATE(add_to_list, TYPE_NAME)(list, elem)) {
        ELOG("cannot add elem to the end of list");
        return 1;
    }

    return 0;
}

int TEMPLATE(remove_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index) {
    if (index < 0) {
        ELOG("index is non-positive");
        return 1;
    }
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        if (curr_index == index) {
            TEMPLATE(LIST_NODE, TYPE_NAME)* prev = curr_node->prev;
            prev->next = curr_node->next;
            curr_node->next->prev = prev;

            #if TYPE_NUM == INT_DYN_ARRAY
            TEMPLATE(destroy, int)(&curr_node->data);
            #elif TYPE_NUM == STRING_DYN_ARRAY
            TEMPLATE(destroy, string)(&curr_node->data);
            #elif TYPE_NUM == VOP_DYN_ARRAY
            TEMPLATE(destroy, vop)(&curr_node->data);
            #elif TYPE_NUM == VOP_DYN_ARRAY_ST
            TEMPLATE(destroy, vop_st)(&curr_node->data);
            #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 || TYPE_NUM == STRING
            free(curr_node->data);
            #endif

            free(curr_node);
            return 0;
        }
        ++curr_index;
        curr_node = curr_node->next;
    }

    if (curr_index == 0 || index > curr_index) {
        ELOG("index out of range");
        return 1;
    }

    return 0;
}

#endif
#endif
#endif

