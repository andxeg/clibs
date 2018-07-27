#ifdef T
#ifdef T1
#include "templates.h"
#include <stdio.h>
void TEMPLATE(print_array, T) (T *array, int n) {
    int i;
    for (i = 0; i < n; i++) {
    #if T1 == 1
        printf("%f ", array[i]);
    #elif T1 == 0
        printf("%d ", array[i]);
    #else
        printf("%d", array[i]);
    #endif
    }
    putchar('\n');
}
#endif
#endif
 
