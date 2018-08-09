#ifndef DYNAMIC_ARRAYS_H
#define DYNAMIC_ARRAYS_H
#include "templates.h"
#include "good_field_types.h"
#include "dynamic_array_types.h"


#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T int
#define TYPE_NAME int
#define TYPE_NUM INT
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T char
#define TYPE_NAME char
#define TYPE_NUM CHAR
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T char*
#define TYPE_NAME string
#define TYPE_NUM STRING
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T char*
#define TYPE_NAME string_st
#define TYPE_NUM STRING_ST
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T GOOD_FIELD_TYPE
#define TYPE_NAME good_field
#define TYPE_NUM GOOD_FIELD
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T void*
#define TYPE_NAME vop // pointer to void
#define TYPE_NUM VOIDP
#include "dynamic_array_template.h"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T void*
#define TYPE_NAME vop_st // pointer to void
#define TYPE_NUM VOIDP_ST
#include "dynamic_array_template.h"

#endif

