#ifdef T
#ifdef TYPE_NAME
#ifdef TYPE_NUM

#include "types.h"
#include "templates.h"
#include <stdlib.h>
#include <stdio.h>
#include "dynamic_arrays.h"

TEMPLATE(LIST, TYPE_NAME)* TEMPLATE(create_list, TYPE_NAME)() {
    TEMPLATE(LIST, TYPE_NAME)* result = (TEMPLATE(LIST, TYPE_NAME)*)malloc(sizeof(TEMPLATE(LIST, TYPE_NAME)));
    if (result == NULL) {
        fprintf(stderr, "create_list: error in malloc\n");
        return NULL;
    }
    
    result->next = result;
    result->prev = result;
    
    return result;
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
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3
        free(node->data);
        #endif
        
        free(node);
    }
    
    free(list);
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
        if (TEMPLATE(array_equal, int)(&curr_node->data, &elem))
        #elif TYPE_NUM == STRING_DYN_ARRAY
        if (TEMPLATE(array_equal, string)(&curr_node->data, &elem))
        #elif TYPE_NUM == VOP_DYN_ARRAY
        if (TEMPLATE(array_equal, vop)(&curr_node->data, &elem))
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1])
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] && 
            curr_node->data[2] == elem[2])
        #else
        if (curr_node->data == elem)
        #endif
            return 1;
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

int TEMPLATE(add_to_list, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, T elem) {
    TEMPLATE(LIST_NODE, TYPE_NAME)* last = list->prev;
    TEMPLATE(LIST_NODE, TYPE_NAME)* new_node = (TEMPLATE(LIST_NODE, TYPE_NAME)*)malloc(sizeof(TEMPLATE(LIST_NODE, TYPE_NAME)));
    if (new_node == NULL) {
        fprintf(stderr, "add_to_list: error in malloc\n");
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
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1]) {
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] && 
            curr_node->data[2] == elem[2]) {
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
    
    fprintf(stderr, "get_by_index: index out of range\n"); 
    return 1; 
}

int TEMPLATE(insert_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T elem) {
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME) *curr_node= list->next;
    while (curr_node != list) {
        if (curr_index == index) {
             #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
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

    fprintf(stderr, "insert_by_index: index out of range\n");
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
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1]) {
        #elif TYPE_NUM == INT_VEC3 || TYPE_NUM == INT_VEC3_ST
        if (curr_node->data[0] == elem[0] && curr_node->data[1] == elem[1] && 
            curr_node->data[2] == elem[2]) {
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
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    int length = TEMPLATE(size_list, TYPE_NAME)(list);
    printf("len: %d ", length);
    putchar('[');
    while (curr_node->next != list)  {
        #if TYPE_NUM == INT_DYN_ARRAY
        TEMPLATE(print, int)(&curr_node->data);
        #elif TYPE_NUM == STRING_DYN_ARRAY
        TEMPLATE(print, string)(&curr_node->data);
        #elif TYPE_NUM == VOP_DYN_ARRAY
        TEMPLATE(print, vop)(&curr_node->data);
        #elif TYPE_NUM == INT
        printf("%d, ", curr_node->data);
        #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE
        printf("%f, ", curr_node->data);
        #elif TYPE_NUM == STRING
        printf("%s, ", curr_node->data);
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
              TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
        int len;
        #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        len = 2;
        #else
        len = 3;
        #endif
        printf("[");
        for (int i = 0; i < len-1; i++) {
            printf("%d,", curr_node->data[i]);
        }
        printf("%d], ", curr_node->data[len-1]);
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
        #elif TYPE_NUM == INT
        printf("%d", curr_node->data);
        #elif TYPE_NUM == FLOAT || TYPE_NUM == DOUBLE
        printf("%f", curr_node->data);
        #elif TYPE_NUM == STRING
        printf("%s", curr_node->data);
        #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3 ||\
              TYPE_NUM == INT_VEC2_ST || TYPE_NUM == INT_VEC3_ST
        int len;
        #if TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC2_ST
        len = 2;
        #else
        len = 3;
        #endif
        printf("[");
        for (int i = 0; i < len-1; i++) {
            printf("%d,", curr_node->data[i]);
        }
        printf("%d]", curr_node->data[len-1]);
        #endif
    }

    printf("]\n");
}


int TEMPLATE(add_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index, T elem) {
    if (index < 0) {
        fprintf(stderr, "add_elem_by_index: index is non-positive\n");
        return 1;
    }
    int curr_index = 0;
    TEMPLATE(LIST_NODE, TYPE_NAME)* curr_node = list->next;
    while (curr_node != list) {
        if (curr_index == index) {
            TEMPLATE(LIST_NODE, TYPE_NAME)* prev = curr_node->prev;
            TEMPLATE(LIST_NODE, TYPE_NAME)* new_node = (TEMPLATE(LIST_NODE, TYPE_NAME)*)malloc(sizeof(TEMPLATE(LIST_NODE, TYPE_NAME)));
            if (new_node == NULL) {
                fprintf(stderr, "add_elem_by_index: error in malloc\n");
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
        fprintf(stderr, "add_elem_by_index: index out of range\n");
        return 1;
    }   

    if (TEMPLATE(add_to_list, TYPE_NAME)(list, elem)) {
        fprintf(stderr, "add_elem_by_index: cannot add elem to the end of list\n");
        return 1;
    }

    return 0; 
}

int TEMPLATE(remove_elem_by_index, TYPE_NAME)(TEMPLATE(LIST, TYPE_NAME) *list, int index) {
    if (index < 0) {
        fprintf(stderr, "remove_elem_by_index: index is non-positive\n");
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
            #elif TYPE_NUM == INT_VEC2 || TYPE_NUM == INT_VEC3
            free(curr_node->data);
            #endif

            free(curr_node);
            return 0;
        }
        ++curr_index;
        curr_node = curr_node->next; 
    }
    
    if (curr_index == 0 || index > curr_index) {
        fprintf(stderr, "remove_elem_by_index: index out of range\n");
        return 1;
    }   

    return 0;
}    

#endif
#endif
#endif

