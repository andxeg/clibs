#define INCREASE_FACTOR 2

typedef struct {
    int *data;
    int length;
    int capacity;
} DYN_ARRAY_INT;

int create(int size, DYN_ARRAY_INT *a);
void destroy(DYN_ARRAY_INT *a);
int is_empty(DYN_ARRAY_INT *a);
int get(DYN_ARRAY_INT *a, int index, int *result);
int insert(DYN_ARRAY_INT *a, int index, int elem);
int append(DYN_ARRAY_INT *a, int elem);
void print(DYN_ARRAY_INT *a);

