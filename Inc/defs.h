#ifndef DEFS_H
#define DEFS_H

#include "templates.h"

#define ERROR_LENGTH_LIMIT 128
#define LOG_MSG_LENGTH_LIMIT 1024
#define INITIAL_FIELDS_COUNT 10
#define GOODS_FILE_SEPARATOR '|'
#define DEFAULT_FIELD_SIZE 20
#define PAN_LIMIT 32
#define CARD_READ_LIMIT 5
#define DOC_LINE_LIMIT 64
#define FILE_TYPE_LEN 10
#define DYNAMIC_FILE_TYPE 1
#define STATIC_FILE_TYPE 2
#define ERROR_FILE_TYPE 3

#define FILENAME_LIMIT 256
#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))
#define NUMBER_OF_LINES(a) (sizeof(a)/sizeof((a)[0]))

typedef struct TEMPLATE(DYN_ARRAY, good_field) TEMPLATE(DYN_ARRAY, good_field);
typedef struct TEMPLATE(DYN_ARRAY, int) TEMPLATE(DYN_ARRAY, int);
typedef struct TEMPLATE(DYN_ARRAY, char) TEMPLATE(DYN_ARRAY, char);
typedef struct TEMPLATE(DYN_ARRAY, string) TEMPLATE(DYN_ARRAY, string);
typedef struct TEMPLATE(DYN_ARRAY, string_st) TEMPLATE(DYN_ARRAY, string_st);
typedef struct TEMPLATE(DYN_ARRAY, vop) TEMPLATE(DYN_ARRAY, vop);
typedef struct TEMPLATE(DYN_ARRAY, vop_st) TEMPLATE(DYN_ARRAY, vop_st);

#endif
