#ifndef DEFS_H
#define DEFS_H

#define ERROR_LENGTH_LIMIT 1024
#define LOG_MSG_LENGTH_LIMIT 1024
#define INITIAL_FIELDS_COUNT 10
#define GOODS_FILE_SEPARATOR '|'
#define DEFAULT_FIELD_SIZE 20

#define FILENAME_LIMIT 256
#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))
#define NUMBER_OF_LINES(a) (sizeof(a)/sizeof((a)[0]))

#endif
