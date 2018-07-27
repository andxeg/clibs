#ifndef ALL_POSSIBLE_PRINT_H
#define ALL_POSSIBLE_PRINT_H
#include "templates.h"

#ifdef T
#undef T
#endif
#ifdef T1
#undef T1
#endif
#define T int
#define T1 0
#include "print_array_template.h"

#ifdef T
#undef T
#endif
#ifdef T1
#undef T1
#endif
#define T float
#define T1 1
#include "print_array_template.h"

#endif

