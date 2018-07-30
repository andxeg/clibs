#include <stdio.h>
#include <stdlib.h>
#include "lists.h"


int test_list_creation() {
    TEMPLATE(LIST, int)* list_int = TEMPLATE(create_list, int)();
    TEMPLATE(add_to_list, int)(list_int, 1);
    TEMPLATE(add_to_list, int)(list_int, 2);
    TEMPLATE(add_to_list, int)(list_int, 3);
    TEMPLATE(print_list, int)(list_int);
//    TEMPLATE(destroy_list, int)(list_int);
    return 0;
}

typedef int (*f)();

typedef struct {
    char* name;
    f test; 
} TEST_CASE;

int main(int argc, char** argv) {
    TEST_CASE test_cases[1] = {
        {"Test creation list for different types", test_list_creation}
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

