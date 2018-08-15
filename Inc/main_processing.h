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
extern const char* BACKUP_FILE_SCHEMA;
// extern FILE_SCHEMA* file_schema;

/*+************* FUNCTION PROTOTYPES ***************************************+*/
void main_processing(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
void display_startup_message(T_GL_HGRAPHIC_LIB hGraphicLib);
void create_menu(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);

int import_file_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int display_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int modify_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int find_goods_by_template(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int find_goods_by_categories(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int form_cart_and_buy(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);

void get_first_fields(FILE_SCHEMA* file_schema, char** first_fields);
int show_good_fields(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop) fields_value, int show_type, T_GL_HWIDGET hLayout, T_GL_DIM* usLine);
int get_fields_values_len(FILE_SCHEMA* file_schema);
int get_max_field_len(FILE_SCHEMA* file_schema);
char* read_field(T_GL_HGRAPHIC_LIB graphicLib, char* field_name, int min_size, int max_size, GOOD_FIELD_TYPE type, int is_pattern);
int convert_to_bool_value(char* array_char, int* value);
int convert_to_int_value(char* array_char, int* value);
int add_new_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int delete_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index);
int edit_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index);
int read_search_pattern(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop)* pattern);
int print_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop) fields_value);
int print_header_for_search_result(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, T_GL_HWIDGET hLayout, T_GL_DIM* usLine);
int print_search_results(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(LIST, dyn_array_vop)* res);
int display_search_results(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(LIST, dyn_array_vop)* res);
void display_message_search_menu(T_GL_HGRAPHIC_LIB hGraphicLib);
int pay_for_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, int)* indexes, TEMPLATE(DYN_ARRAY, int)* counts);
int get_count_of_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);

void pay(T_GL_HGRAPHIC_LIB hGraphicLib, char* pan, int len);
int print_document(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, int)* indexes, TEMPLATE(DYN_ARRAY, int)* counts, char* pan);
int get_good_price(FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop)* good_fields);
int get_index_of_price_field(FILE_SCHEMA* file_schema);

int get_bool_fields_amount(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);
int get_bool_fields_name(FILE_SCHEMA* file_schema, char** first_fields);
int get_categories_pattern(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, char** bool_fields, bool* checked, int bool_fields_count, TEMPLATE(DYN_ARRAY, vop)* pattern);

int backup_file_schema(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int type);
int restore_file_schema(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema);

#endif // MAIN_PROCESSING_H
