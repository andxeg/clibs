#include "types.h"
#include "templates.h"
#include "lists.h"
#include "dynamic_arrays.h"


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
#include "list_template.c"

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
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T float
#define TYPE_NAME float
#define TYPE_NUM FLOAT
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T TEMPLATE(DYN_ARRAY, int)
#define TYPE_NAME dyn_array_int
#define TYPE_NUM INT_DYN_ARRAY  
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T TEMPLATE(DYN_ARRAY, string)
#define TYPE_NAME dyn_array_string
#define TYPE_NUM STRING_DYN_ARRAY
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T int*
#define TYPE_NAME int_vec2
#define TYPE_NUM INT_VEC2
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T int*
#define TYPE_NAME int_vec3
#define TYPE_NUM INT_VEC3
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T int*
#define TYPE_NAME int_vec2_st
#define TYPE_NUM INT_VEC2_ST
#include "list_template.c"

#ifdef T
#undef T
#endif
#ifdef TYPE_NAME
#undef TYPE_NAME
#endif
#ifdef TYPE_NUM
#undef TYPE_NUM
#endif
#define T int*
#define TYPE_NAME int_vec3_st
#define TYPE_NUM INT_VEC3_ST
#include "list_template.c"


