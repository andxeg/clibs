#include <stdio.h>
#include "all_possible_sums.h"
#include "all_possible_print.h"

int main(int argc, char **argv) {
    int ai[3] = {1, 2, 3};
    int bi[3] = {4, 5, 6};
    float af[3] = {1.0, 2.0, 3.0};
    float bf[3] = {1.5, 0.3, 4.5};
    printf("before: \n");
    TEMPLATE(print_array, int) (ai, 3);
    TEMPLATE(print_array, int) (bi, 3);
    TEMPLATE(print_array, float) (af, 3);
    TEMPLATE(print_array, float) (bf, 3);
    TEMPLATE(sum, int) (3, ai, bi);
    TEMPLATE(sum, float) (3, af, bf);
    printf("after: \n");
    TEMPLATE(print_array, int) (ai, 3);
    TEMPLATE(print_array, float) (af, 3);
    return 0;
}

