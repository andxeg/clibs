#ifndef ALL_POSSIBLE_SUMS_H
#define ALL_POSSIBLE_SUMS_H
#include "templates.h"

#ifdef T
#undef T
#endif
#define T float
#include "sum_as_template.h"

#ifdef T
#undef T
#endif
#define T double
#include "sum_as_template.h"

#ifdef T
#undef T
#endif
#define T int
#include "sum_as_template.h"

#endif

