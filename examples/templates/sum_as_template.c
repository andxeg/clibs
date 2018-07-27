#ifdef T
#include "templates.h"
void TEMPLATE(sum, T) (int n, T *a, T *b) {
    int i;
    for (i = 0; i < n; i++)
        a[i] += b[i];
}
#endif

