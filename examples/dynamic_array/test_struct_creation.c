#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
    int *array;
} TEST_ST;

int create(int size, TEST_ST *st) {
    st->a = 1;
    st->b = 2;
    st->array = (int *) calloc(size, sizeof(int));
    if (st->array == NULL)
        return 1;
    for (int i = 0; i < size; i++) {
        st->array[i] = i;
    }
    return 0;
}

void destroy(TEST_ST *st) {
    free(st->array);
}

void print_array_int(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
       printf("%d ", a[i]); 
    }
    putchar('\n');
}

int main() {
    TEST_ST st;
    if (create(10, &st) == 0)
        printf("structure was successfully created\n");
    else {
        fprintf(stderr, "error in structure creation\n");
        return 1;
    }
    printf("a -> %d\n", st.a);
    printf("b -> %d\n", st.b);
    print_array_int(st.array, 10);
    destroy(&st);
    return 0;
}


