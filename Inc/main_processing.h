/**
 * main_processing.h
 *
 * Header for the main_processing source file.
 *
 */
#ifndef MAIN_PROCESSING_H
#define MAIN_PROCESSING_H

#include <GL_GraphicLib.h>
#include "file_schema.h"

/*+************* #DEFINES **************************************************+*/

/*+************* CONSTANTS *************************************************+*/

/*+************* STRUCTURES ************************************************+*/

/*+************* VARIABLES *************************************************+*/
extern T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance;
// extern FILE_SCHEMA* file_schema;

/*+************* FUNCTION PROTOTYPES ***************************************+*/
void main_processing(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
void print_startup_message(T_GL_HGRAPHIC_LIB hGraphicLib);
int import_file_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
void get_first_fields(FILE_SCHEMA* file_schema, char** first_fields);
int print_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int print_good_fields(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop) fields_value);
int get_fields_values_len(FILE_SCHEMA* file_schema);
int get_max_field_len(FILE_SCHEMA* file_schema);
char* read_field(T_GL_HGRAPHIC_LIB graphicLib, char* field_name, int min_size, int max_size, GOOD_FIELD_TYPE type);
int convert_to_bool_value(char* array_char, int* value);
int convert_to_int_value(char* array_char, int* value);
int add_new_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int delete_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index);
int edit_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index);
int modify_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
void create_menu(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);

#endif // MAIN_PROCESSING_H
