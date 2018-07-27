#include "templates.h"
#include "all_possible_print.h"

#ifdef T
#undef T
#endif
#ifdef T1
#undef T1
#endif
#define T int
#define T1 0
#include "print_array_template.c"

#ifdef T
#undef T
#endif
#ifdef T1
#undef T1
#endif
#define T float
#define T1 1
#include "print_array_template.c"

