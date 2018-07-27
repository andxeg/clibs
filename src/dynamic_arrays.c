#include "templates.h"
#include "dynamic_arrays.h"
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
#include "dynamic_array_template.c"

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
#include "dynamic_array_template.c"

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
#include "dynamic_array_template.c"

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
#include "dynamic_array_template.c"

