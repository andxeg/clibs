#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "log.h"
#include "defs.h"
#include "Goal.h"
#include "Menu.h"
#include "iofile.h"
#include "file_schema.h"
#include "main_processing.h"

#include "Training.h"

const char* TERMINAL_DATA_DIR = "dir://flash/HOST";
const char* GOODS_FILE_FORMAT = "*.TXT";

// Properties of the File screen (Goal)
// ====================================
static const ST_DSP_LINE txFile[] = {
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line0
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line1
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line2
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line3
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line4
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} }
};

// Properties of the Magnetic screen (Goal)
// ========================================
static const ST_DSP_LINE txMagnetic[] =
{
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line0
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line1
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line2
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} }
};

// Properties of the Track screen (Goal)
// =====================================
static const ST_DSP_LINE txTrack[] =
{
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line0
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_RED,   100, FALSE, {1, 3, 0, 3}, {1, 1, 1, 1, GL_COLOR_WHITE}, {3, 0, 3, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line1
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, TRUE,  100, FALSE, {2, 4, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line2
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLUE,  100, FALSE, {1, 3, 0, 3}, {1, 1, 1, 1, GL_COLOR_WHITE}, {3, 0, 3, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line3
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, TRUE,  100, FALSE, {2, 4, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line4
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_GREEN, 100, FALSE, {1, 3, 0, 3}, {1, 1, 1, 1, GL_COLOR_WHITE}, {3, 0, 3, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}}, // Line5
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, TRUE,  100, FALSE, {2, 4, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XSMALL}} }
};

// Properties of the default printer (Goal)
// ========================================
static const ST_PRN_LINE xPrinter =
{
	GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {1, 0, 1, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}
};

void print_startup_message(T_GL_HGRAPHIC_LIB hGraphicLib) {
	int iRet;
	T_GL_HWIDGET hScreen = NULL;

	hScreen = GoalCreateScreen(hGraphicLib, txFile, NUMBER_OF_LINES(txFile), GL_ENCODING_UTF8);
	CHECK(hScreen!=NULL, lblKO);                   // Create screen and clear it
	iRet = GoalClrScreen(hScreen, GL_COLOR_BLACK, KEY_VALID, true);
	CHECK(iRet>=0, lblKO);

	GoalDspLine(hScreen, 0, "In main menu you can:", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 0, "1. Import goods from file", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 1, "2. Print list of goods", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 2, "3. Add/Delete/Edit good in list", &txFile[2], 3*1000, true);
	GoalDspLine(hScreen, 3, "4. Find goods by template", &txFile[3], 3*1000, true);
	GoalDspLine(hScreen, 4, "5. Form shopping cart and pay for it", &txFile[3], 3*1000, true);
	goto lblEnd;

lblKO:                                             // None-classified low level error
	GL_Dialog_Message(hGraphicLib, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;

lblEnd:
	if (hScreen) {
		GoalDestroyScreen(&hScreen);
	}
}

int import_file_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {

	int iRet;
	T_GL_HWIDGET screen = NULL;
	char filename[FILENAME_LIMIT];

	if (GL_Dialog_File(hGraphicLib,
			"Select file", TERMINAL_DATA_DIR, GOODS_FILE_FORMAT,
			filename, sizeof(filename), 0,
			GL_TIME_INFINITE) == GL_KEY_VALID)
	{
		read_file_with_goods(filename, file_schema, hGraphicLib);
		print_file_schema(file_schema);
	}

	if (screen) {
		GoalDestroyScreen(&screen);
	}

	return 0;
}

int get_fields_values_len(FILE_SCHEMA* file_schema) {
	int result = 0;
	TEMPLATE(DYN_ARRAY, int) length_max = file_schema->header->length_max;
	int fields_count = length_max.length;

	int i;
	int limit = 0;
	for (i = 0; i < fields_count; i++) {
		TEMPLATE(get, int)(&length_max, i, &limit);
		result += limit;
	}

	return result;
}

int get_max_field_len(FILE_SCHEMA* file_schema) {
	int result = 0;
	TEMPLATE(DYN_ARRAY, string) fields = file_schema->header->fields;

	int i;
	int len;
	char* str;
	for (i = 0; i < fields.length; i++) {
		TEMPLATE(get, string)(&fields, i, &str);
		len = strlen(str);
		result = (result < len) ? len : result;
	}

	return result;
}

int print_good_fields(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema,
		TEMPLATE(DYN_ARRAY, vop) fields_value)
{
	int fields_amount = fields_value.length;
	int total_len = get_fields_values_len(file_schema);
	// add fields_amount - 1 bytes for separators
	// add fields_amount * max_fields_len for field names
	int max_field_len = get_max_field_len(file_schema);
	char* good_fields = (char*) malloc(sizeof(char) * (total_len + (max_field_len + 2) * fields_amount +
									   (fields_amount - 1) + 1));
	if (good_fields == NULL) {
		print_message(hGraphicLib, "Internal error: cannot allocate memory");
		return 1;
	}

	good_fields[0] ='\0' ;

	int i;
	int* val;
	char* str;
	int curr_pos;
	for (i = 0; i < fields_amount; i++) {
		char* field_name;
		TEMPLATE(get, string)(&file_schema->header->fields, i, &field_name);
		GOOD_FIELD_TYPE type;
		TEMPLATE(get, good_field)(&file_schema->header->types, i, &type);
		curr_pos = strlen(good_fields);
		switch (type) {
		case BOOL_GOOD:
		case NUMBER_GOOD:
			TEMPLATE(get, vop)(&fields_value, i, (void**)(&val));
			sprintf(good_fields + curr_pos, "%s: %d\n", field_name, *val);
			break;
		case STRING_GOOD:
			TEMPLATE(get, vop)(&fields_value, i, (void**)(&str));
			sprintf(good_fields + curr_pos, "%s: %s\n", field_name, str);
			break;
		default:
			break;
		}
	}

	print_message(hGraphicLib, good_fields);
	free(good_fields);
	return 0;
}

void get_first_fields(FILE_SCHEMA* file_schema, char** first_fields) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);

	int i;
	char* field_value;
	TEMPLATE(DYN_ARRAY, vop) array_void;
	for (i = 0; i < goods_count; i++) {
		// get goods fields values
		TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, i, &array_void);
		TEMPLATE(get, vop)(&array_void, 0, (void**)(&field_value));
		first_fields[i] = field_value;
	}
}

int print_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	GOOD_FIELD_TYPE first_field_type;
	TEMPLATE(get, good_field)(&file_schema->header->types, 0, &first_field_type);
	if (first_field_type != STRING_GOOD) {
		print_message(hGraphicLib, "first good field must be a string");
		return 1;
	}

	// first field values
	char** first_fields = (char** ) malloc((goods_count + 2) * sizeof(char*));
	get_first_fields(file_schema, first_fields);
	first_fields[goods_count] = "Exit";
	first_fields[goods_count + 1] = NULL;

	char choice = 0;
	TEMPLATE(DYN_ARRAY, vop) array_void;
	do {
		// display menu with first fields
		choice = GL_Dialog_Menu(hGraphicLib, "List of goods", first_fields, choice,
				GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

		if (choice == goods_count) {
			break;
		}

		// get good fields values
		TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, choice, &array_void);
		GL_Dialog_Message(gGoalGraphicLibInstance, first_fields[choice],
				first_fields[choice], GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);

		// print fields
		print_good_fields(hGraphicLib, file_schema, array_void);

	} while (choice != goods_count);

	free(first_fields);
	return 0;
}

char* read_field(T_GL_HGRAPHIC_LIB graphicLib, char* field_name,
		int min_size, int max_size, GOOD_FIELD_TYPE type, int is_pattern)
{
	char* field = (char* ) malloc(sizeof(char) * (max_size + 1));

	int i;
	char c;
	ulong result;
	char* template;

	switch (type) {
	case BOOL_GOOD:
		if (is_pattern) {
			result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
					"/u|01*", field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		} else {
			result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
					"/b|01", field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		}
		(void)result;
		break;
	case NUMBER_GOOD:
		// '/u' * max_size + '|' + digits + '\0'
		template = (char *) malloc(max_size*2 + 1 + 10 + 1);
		for (i = 0; i < max_size*2; i += 2) {
			template[i] = '/';
			template[i+1] = 'u';
		}

		template[i++] = '|';

		for (c = '0'; c <= '9'; c++, i++) {
			template[i] = c;
		}

		if (is_pattern) {
			// add '*'
			template = (char *) realloc(template, max_size*2 + 1 + 10 + 1 + 1);
			template[i++] = '*';
		}

		template[i] = '\0';
		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				template, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;

		free(template);
		break;
	case STRING_GOOD:
		// '/u' * max_size + '|' + aplhas + digits + ' ' + '.' + '\0'
		template = (char *) malloc(max_size*2 + 1 + 2*26 + 10 + 1 + 1 + 1);
		for (i = 0; i < max_size*2; i += 2) {
			template[i] = '/';
			template[i+1] = 'u';
		}

		template[i++] = '|';

		for (c = 'a'; c <= 'z'; c++, i++) {
			template[i] = c;
		}

		for (c = 'A'; c <= 'Z'; c++, i++) {
			template[i] = c;
		}

		for (c = '0'; c <= '9'; c++, i++) {
			template[i] = c;
		}

		template[i++] = ' ';
		template[i++] = '.';

		if (is_pattern) {
			// add '*'
			template = (char *) realloc(template, max_size*2 + 1 + 2*26 + 10 + 1 + 1 + 1 + 1);
			template[i++] = '*';
		}

		template[i] = '\0';

		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				template, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;
		free(template);
		break;
	default:
		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				0, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;
		break;
	}

	if (strlen(field) < min_size) {
		print_message(graphicLib, "field size is less than permitted");
		free(field);
		return NULL;
	}
	return field;
}

int convert_to_int_value(char* str, int* value) {
	if (strlen(str) == 0) {
		return 0;
	}

	char* eptr;
	long result = strtol(str, &eptr, 10);
	if (result == 0) {
		if (errno == EINVAL) {
			ELOG("error while convert string to number");
			return 1;
		}
		if (errno == ERANGE) {
			ELOG("converted integer value is out of range");
			return 1;
		}
	}

	*value = (int) result;
	return 0;
}

int convert_to_bool_value(char* str, int* value) {
    int result;
    if (convert_to_int_value(str, &result)) {
        return 1;
    }

    if (result != 0 && result != 1) {
        return 1;
    }

    *value = result;
    return 0;
}


int add_new_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;
    TEMPLATE(DYN_ARRAY, good_field)* types = &file_schema->header->types;
    TEMPLATE(DYN_ARRAY, int)* min_limit = &file_schema->header->length_min;
    TEMPLATE(DYN_ARRAY, int)* max_limit = &file_schema->header->length_max;
    TEMPLATE(LIST, dyn_array_vop)* goods = file_schema->goods;

	int i;
	GOOD_FIELD_TYPE type;
	int max_len;
	int min_len;
	char* field_name;
	char* field_value;
	int* val;
	TEMPLATE(DYN_ARRAY, vop) good_fields;
	TEMPLATE(create, vop)(fields->length, &good_fields);
	for (i = 0; i < fields->length; i++) {
		TEMPLATE(get, good_field)(types, i, &type);
		TEMPLATE(get, int)(min_limit, i, &min_len);
		TEMPLATE(get, int)(max_limit, i, &max_len);
		TEMPLATE(get, string)(fields, i, &field_name);

		print_message(hGraphicLib, field_name);

		field_value = read_field(hGraphicLib, field_name, min_len, max_len, type, 0);
		if (field_value == NULL) {
			TEMPLATE(destroy, vop)(&good_fields);
			print_message(hGraphicLib, "Error in input field");
			return 1;
		}

		if (type == BOOL_GOOD) {
			val = (int *) malloc(sizeof(int));
			if (convert_to_bool_value(field_value, val)) {
				free(field_value); free(val);
				TEMPLATE(destroy, vop)(&good_fields);
				print_message(hGraphicLib, "cannot convert to bool value");
				return 1;
			}

			TEMPLATE(append, vop)(&good_fields, (void *)val);
			TEMPLATE(append, int)(good_fields.types, BOOL);
			free(field_value);
		} else if (type == NUMBER_GOOD) {
			val = (int *) malloc(sizeof(int));
			if (convert_to_int_value(field_value, val)) {
				free(field_value); free(val);
				TEMPLATE(destroy, vop)(&good_fields);
				print_message(hGraphicLib, "cannot convert to integer value");
				return 1;
			}

			TEMPLATE(append, vop)(&good_fields, (void *)val);
			TEMPLATE(append, int)(good_fields.types, INT);
			free(field_value);
		} else if (type == STRING_GOOD) {
			TEMPLATE(append, vop)(&good_fields, (void*)field_value);
			TEMPLATE(append, int)(good_fields.types, STRING);
		} else {
			print_message(hGraphicLib, "Internal error: unknown good type");
			if (field_value != NULL) {
				free(field_value);
			}
			return 1;
		}
	}

	// add good to list
	if (TEMPLATE(add_to_list, dyn_array_vop)(goods, good_fields)) {
		print_message(hGraphicLib, "cannot add good to list");
		return 1;
	}

	print_message(hGraphicLib, "good was successfully added");
	return 0;
}

int delete_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index) {
	TEMPLATE(LIST, dyn_array_vop)* goods = file_schema->goods;

	if(TEMPLATE(remove_elem_by_index, dyn_array_vop)(goods, index)) {
		print_message(hGraphicLib, "cannot remove good");
		return 1;
	}

	print_message(hGraphicLib, "good was successfully removed");
	return 0;
}

int edit_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index) {
	const  char *menu[] = {
			"Delete",
			"Edit",
			"Exit",
			0
	};

	int menu_len = 3;
	char choice = 0;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", menu, choice,
				GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

		if (choice == menu_len - 1) {
			break;
		} else if (choice == 0) {
			delete_good(hGraphicLib, file_schema, index);
			break;
		} else {
			// EDIT
			break;
		}


	} while (choice != menu_len - 1);

	return 0;
}

int modify_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	// first field values
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
	}

	char** first_fields = (char** ) malloc((goods_count + 3) * sizeof(char*));
	get_first_fields(file_schema, first_fields);
	first_fields[goods_count] = "Add new good";
	first_fields[goods_count + 1] = "Exit";
	first_fields[goods_count + 2] = NULL;

	char choice = 0;
	do {
		choice = GL_Dialog_Choice(hGraphicLib, "List of goods", first_fields,
				choice, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
	    (void)choice;

		if (choice == goods_count + 1) {
			break;
		} else if (choice == goods_count) {
			add_new_good(hGraphicLib, file_schema);
			break;
		} else {
			edit_good(hGraphicLib, file_schema, choice);
			break;
		}

	} while(choice != goods_count + 1);

	free(first_fields);
	return 0;
}

void print_message_search_menu(T_GL_HGRAPHIC_LIB hGraphicLib) {
	int iRet;
	T_GL_HWIDGET hScreen = NULL;

	hScreen = GoalCreateScreen(hGraphicLib, txFile, NUMBER_OF_LINES(txFile), GL_ENCODING_UTF8);
	CHECK(hScreen!=NULL, lblKO);                   // Create screen and clear it
	iRet = GoalClrScreen(hScreen, GL_COLOR_BLACK, KEY_VALID, true);
	CHECK(iRet>=0, lblKO);

	GoalDspLine(hScreen, 0, "Search goods by template:", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 1, "1. Enter field values", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 2, "2. You can use '*' for match any value", &txFile[1], 3*1000, true);
	GoalDspLine(hScreen, 3, "3. See results", &txFile[3], 3*1000, true);
	goto lblEnd;

lblKO:                                             // None-classified low level error
	GL_Dialog_Message(hGraphicLib, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;

lblEnd:
	if (hScreen) {
		GoalDestroyScreen(&hScreen);
	}
}

int read_search_pattern(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop)* pattern) {
	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;
	TEMPLATE(DYN_ARRAY, good_field)* types = &file_schema->header->types;
	TEMPLATE(DYN_ARRAY, int)* min_limit = &file_schema->header->length_min;
	TEMPLATE(DYN_ARRAY, int)* max_limit = &file_schema->header->length_max;
	TEMPLATE(LIST, dyn_array_vop)* goods = file_schema->goods;

	int i;
	GOOD_FIELD_TYPE type;
	int max_len;
	int min_len;
	char* field_name;
	char* field_value = NULL;
	char* asterisk = "*";
	int* val;
	for (i = 0; i < fields->length; i++) {
		TEMPLATE(get, good_field)(types, i, &type);
		TEMPLATE(get, int)(min_limit, i, &min_len);
		TEMPLATE(get, int)(max_limit, i, &max_len);
		TEMPLATE(get, string)(fields, i, &field_name);

		print_message(hGraphicLib, field_name);

		field_value = read_field(hGraphicLib, field_name, min_len, max_len, type, 1);
		if (field_value == NULL) {
			print_message(hGraphicLib, "Error in input field");
			return 1;
		}

		if (strcmp(field_value, "*\0") == 0) {
			char* str = (char *) malloc(sizeof(char) * 2);
			str = strcpy(str, asterisk);
			TEMPLATE(append, vop)(pattern, (void *)str);
			TEMPLATE(append, int)(pattern->types, STRING);
			continue;
		}

		if (type == BOOL_GOOD) {
			val = (int *) malloc(sizeof(int));
			if (convert_to_bool_value(field_value, val)) {
				free(val);
				print_message(hGraphicLib, "cannot convert to bool value");
				if (field_value != NULL) {
					free(field_value);
				}
				return 1;
			}

			TEMPLATE(append, vop)(pattern, (void *)val);
			TEMPLATE(append, int)(pattern->types, BOOL);
			free(field_value);
		} else if (type == NUMBER_GOOD) {
			val = (int *) malloc(sizeof(int));
			if (convert_to_int_value(field_value, val)) {
				free(val);
				print_message(hGraphicLib, "cannot convert to integer value");
				return 1;
			}

			TEMPLATE(append, vop)(pattern, (void *)val);
			TEMPLATE(append, int)(pattern->types, INT);
			free(field_value);
		} else if (type == STRING_GOOD) {
			TEMPLATE(append, vop)(pattern, (void*)field_value);
			TEMPLATE(append, int)(pattern->types, STRING);
		} else {
			print_message(hGraphicLib, "Internal error: unknown good type");
			if (field_value != NULL) {
				free(field_value);
			}
			return 1;
		}
	}

	print_message(hGraphicLib, "pattern was successfully read");
	return 0;
}

int show_search_results(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(LIST, dyn_array_vop)* res) {
	FILE_SCHEMA* tmp_file_schema = (FILE_SCHEMA *) malloc(sizeof(FILE_SCHEMA));

	tmp_file_schema->header = (FILE_HEADER *) malloc(sizeof(FILE_HEADER));
	tmp_file_schema->header->fields = file_schema->header->fields;
	tmp_file_schema->header->types = file_schema->header->types;
	tmp_file_schema->header->length_min = file_schema->header->length_min;
	tmp_file_schema->header->length_max = file_schema->header->length_max;

	tmp_file_schema->goods = res;
	print_list_with_goods(hGraphicLib, tmp_file_schema);

	free(tmp_file_schema->header);
	free(tmp_file_schema);
	return 0;
}

int find_goods_by_template(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	print_message_search_menu(hGraphicLib);
	int fields_amount = file_schema->header->fields.length;
	TEMPLATE(DYN_ARRAY, vop) pattern;
	TEMPLATE(create, vop)(fields_amount, &pattern);

	if (read_search_pattern(hGraphicLib, file_schema, &pattern)) {
		print_message(hGraphicLib, "error in pattern reading");
		TEMPLATE(destroy, vop)(&pattern);
		return 1;
	}

	TEMPLATE(LIST, dyn_array_vop)* res = TEMPLATE(search, dyn_array_vop)(file_schema->goods, &pattern);

	print_message(hGraphicLib, "search");
	show_search_results(hGraphicLib, file_schema, res);

	TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
	TEMPLATE(destroy, vop)(&pattern);
	return 0;
}

int get_categories_pattern(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, char** bool_fields, bool* checked, int bool_fields_count, TEMPLATE(DYN_ARRAY, vop)* pattern) {
	int fields_amount = file_schema->header->fields.length;

	int i, j;
	j = 0;
	int* val;
	char* asterisk = "*";
	GOOD_FIELD_TYPE type;
	for (i = 0; i < fields_amount; i++) {
		TEMPLATE(get, good_field)(&file_schema->header->types, i, &type);
		if (type != BOOL_GOOD) {
			char* str = (char *) malloc(sizeof(char) * 2);
			str = strcpy(str, asterisk);
			TEMPLATE(append, vop)(pattern, (void *)str);
			TEMPLATE(append, int)(pattern->types, STRING);
			continue;
		}

		val = (int *) malloc(sizeof(int));
		*val = (checked[j] == true) ? 1 : 0;
		TEMPLATE(append, vop)(pattern, (void *)val);
		TEMPLATE(append, int)(pattern->types, INT);
		++j;
	}

	return 0;
}


int find_goods_by_categories(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	int fields_amount = file_schema->header->fields.length;

	// pattern is strictly defined - without '*'
	TEMPLATE(DYN_ARRAY, vop) pattern;
	TEMPLATE(create, vop)(fields_amount, &pattern);

	// add bool fields to multi choice dialog
	int bool_fields_count = 0;
	char** bool_fields = (char **) malloc(sizeof(char*) * (fields_amount + 2));

	int i;
	char* str;
	GOOD_FIELD_TYPE type;
	for (i = 0; i < fields_amount; i++) {
		TEMPLATE(get, string)(&file_schema->header->fields, i, &str);
		TEMPLATE(get, good_field)(&file_schema->header->types, i, &type);
		if (type == BOOL_GOOD) {
			bool_fields[bool_fields_count++] = str;
		}
	}

	bool_fields[bool_fields_count] = "Find";
	bool_fields[bool_fields_count+1] = "Exit";
	bool_fields[bool_fields_count+2] = 0;

	bool_fields = (char**) realloc(bool_fields, sizeof(char*) * (bool_fields_count + 3));
	bool* checked = (bool *) malloc(sizeof(bool) * (bool_fields_count + 2));
	for (i = 0; i < bool_fields_count + 2; i++) {
		checked[i] = false;
	}

	char choice = 0;
	do {

		choice = GL_Dialog_MultiChoice (hGraphicLib, "Categories",
				bool_fields, choice, checked, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		(void)choice;

		if (choice == bool_fields_count + 1) break;

		if (choice == bool_fields_count) {
			get_categories_pattern(hGraphicLib, file_schema, bool_fields,
								   checked, bool_fields_count, &pattern);
			SLOG("pattern");
			TEMPLATE(print, vop)(&pattern);
			TEMPLATE(LIST, dyn_array_vop)* res = TEMPLATE(search, dyn_array_vop)(file_schema->goods,
													                             &pattern);
			print_message(hGraphicLib, "search");
			show_search_results(hGraphicLib, file_schema, res);
			TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
			break;
		}

		checked[choice] = (checked[choice] == true) ? false : true;
	} while (choice != bool_fields_count + 1);

	TEMPLATE(destroy, vop)(&pattern);
	free(bool_fields);
	free(checked);
	return 0;
}

void pay(T_GL_HGRAPHIC_LIB hGraphicLib, char* pan, int len) {
	// Flush pan
	pan[0] = '\0';

	// Local variables
    // ***************
	T_GL_HWIDGET hScreen=NULL;
	T_GL_HWIDGET xDocument=NULL;
	ST_PRN_LINE xLine;
    Telium_File_t *hMag2=NULL;
	byte ucLen=0;
    int iSta;
    char tcTmp[128];
    char *pcSrc, *pcDst;
    char tcTrk2[128];
    byte p; // Printer line index
	int iKey, iTimeout=0, iRet;

	// Swipe card in progress
	// **********************
	strcpy(tcTrk2, "No readable data");

	// Open peripherals
	// ================
	if (IsISO2() == 1)
	{
		hMag2 = Telium_Fopen("SWIPE2", "r*");         // Open mag2 peripheral
		CHECK(hMag2, lblKO);
	}

	// Prompt Swipe a card
	// ===================
	hScreen = GoalCreateScreen(gGoalGraphicLibInstance, txMagnetic, NUMBER_OF_LINES(txMagnetic), GL_ENCODING_UTF8);
	CHECK(hScreen!=NULL, lblKO);                      // Create screen and clear it
	iRet = GoalClrScreen(hScreen, GL_COLOR_BLACK, KEY_CANCEL, false);
	CHECK(iRet>=0, lblKO);

	iRet = GoalDspLine(hScreen, 0, "Please Swipe", &txMagnetic[0], 0, false);
	CHECK(iRet>=0, lblKO);                            // Prompt for a swiping card
	iRet = GoalDspLine(hScreen, 1, "Magnetic Card", &txMagnetic[1], 0, true);
	CHECK(iRet>=0, lblKO);

	// Wait for magnetic card
	// ======================
	iRet = TimerStart(0, 10*1000);                                   // Timer0 starts to 30s
	CHECK(iRet>=0, lblKO);
	do
	{
		iSta = Telium_Ttestall(SWIPE2, 5);       // Wait for the first event SWIPE31
		if (iSta != 0)
			break;                                                   // Swipe detected
		iKey = GoalGetKey(hScreen, gGoalGraphicLibInstance, true, 100, false);         // Get key pressed/touched (shortcut)
		CHECK(iKey!=GL_KEY_CANCEL, lblEnd);                          // Exit on cancel key
		iTimeout = TimerGet(0);                                      // Retrieve timer value
	}while(iTimeout>0);

	CHECK(iTimeout!=0, lblEnd);                                      // Exit on timeout

	iSta |= Telium_Ttestall(iSta ^ (SWIPE2), 5); // Wait for the second event SWIPE2
	iSta |= Telium_Ttestall(iSta ^ (SWIPE2), 5); // Wait for the third event SWIPE3

	// Retrieve and analyze ISO2
	// =========================
	if(iSta & SWIPE2)
	{
		memset(tcTmp, 0, sizeof(tcTmp));
		memset(tcTrk2, 0, sizeof(tcTrk2));
		iRet = Telium_Is_iso2(hMag2, &ucLen, (byte*)tcTmp);  // *** Read ISO2 to ascii format ***
		if (iRet != ISO_OK)
			IsoError (iRet, tcTrk2);
		else
		{
			CHECK(strlen(tcTmp)<128, lblKO);
			pcSrc = tcTmp;
			pcDst = tcTrk2;
			while(*pcSrc) {                                  // Find start sentinel
				if(*pcSrc++ == 'B')
					break;
			}
			while(*pcSrc) {                                  // Copy all data between start and end sentinels
				if(*pcSrc == 'F')
					break;
				if(*pcSrc == 'D')
					*pcSrc = '=';
				*pcDst++ = *pcSrc++;
			}
		}
	}

	if (tcTrk2[0]==0) strcpy(tcTrk2, "Empty");

	GoalDestroyScreen(&hScreen);                             // Destroy screen

	// Get pan from track
	// ==================
	int i;
	char * pch;
	pch = strtok (tcTrk2,"=");
	if (pch != NULL && isdigit(pch[0])) {
		strcpy(pan, pch);
		for (i = 0; i < (strlen(pan) - 4); i++) {
			pan[i] = '*';
		}
	}

	goto lblEnd;

	// Errors treatment
	// ****************
lblKO:                                                       // None-classified low level error
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;
lblEnd:
	TimerStop(0);                                            // Stop Timer0
	if(hMag2) {
		Telium_Fclose(hMag2);                                // Close mag2 peripheral
	}

	if (hScreen) {
		GoalDestroyScreen(&hScreen);                         // Destroy screen
	}

	if (xDocument) {
		GoalDestroyDocument(&xDocument);                     // Destroy document
	}

}

int get_index_of_price_field(FILE_SCHEMA* file_schema) {
	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;

	int i;
	char* str;
	for (i = 0; i < fields->length; i++) {
		TEMPLATE(get, string)(fields, i, &str);
		if (strcmp(str, "PRICE\0") == 0) {
			return i;
		}
	}

	return -1;
}

int get_good_price(FILE_SCHEMA* file_schema, TEMPLATE(DYN_ARRAY, vop)* good_fields) {
	// TODO
	int* price;
	int index = get_index_of_price_field(file_schema);
	if (index < 0) {
		return 0;
	}

	TEMPLATE(get, vop)(good_fields, index, (void**)(&price));
	return *price;
}

int print_document(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema,
		           TEMPLATE(DYN_ARRAY, int)* indexes, TEMPLATE(DYN_ARRAY, int)* counts, char* pan) {
	T_GL_HWIDGET hDocument, hLayout, hPrint;
	T_GL_DIM usLine = 0;
	hDocument = GL_Document_Create(hGraphicLib);
	hLayout = GL_Layout_Create(hDocument);

	TEMPLATE(LIST, dyn_array_vop)* goods = file_schema->goods;

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetMargins(hPrint, 0, 10, 0, 10, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(hPrint, GL_COLOR_WHITE);
	GL_Widget_SetBackColor(hPrint, GL_COLOR_BLACK);
	GL_Widget_SetText(hPrint, "CHECK");

	int i;
	unsigned long long price = 0;
	int index = 0;
	int count = 0;
	unsigned long long total_sum = 0;
	char* first_field = NULL;
	char document_line[DOC_LINE_LIMIT];
	TEMPLATE(DYN_ARRAY, vop) good_fields;
	for (i = 0; i < indexes->length; i++) {
		TEMPLATE(get, int)(indexes, i, &index);
		TEMPLATE(get, int)(counts, i, &count);

		TEMPLATE(get_by_index, dyn_array_vop)(goods, index, &good_fields);
		TEMPLATE(get, vop)(&good_fields, 0, (void**)(&first_field));

		price = get_good_price(file_schema, &good_fields);
		total_sum += price * count;

		// good name
		sprintf(document_line, "%s", first_field);
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_XLARGE);
		GL_Widget_SetItem(hPrint, 0, usLine++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_LEFT);
		GL_Widget_SetText(hPrint, document_line);

		// count of good
		sprintf(document_line, "count: %d", count);
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
		GL_Widget_SetItem(hPrint, 0, usLine++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_LEFT);
		GL_Widget_SetText(hPrint, document_line);

		// price
		sprintf(document_line, "> %d", price * count);
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
		GL_Widget_SetItem(hPrint, 0, usLine++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
		GL_Widget_SetText(hPrint, document_line);
	}

	// print total_sum
	sprintf(document_line, "Total sum: %d", total_sum);
	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
	GL_Widget_SetText(hPrint, document_line);

	sprintf(document_line, "Customer card: %s\n\n\n\n\n\n", pan);
	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_LEFT);
	GL_Widget_SetText(hPrint, document_line);

	GL_Document_Print(hDocument, 0);
	GL_Widget_Destroy(hDocument);

	GL_Document_Print(hDocument, 0);
	GL_Widget_Destroy(hDocument);
	return 0;
}

int pay_for_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema,
		         TEMPLATE(DYN_ARRAY, int)* indexes, TEMPLATE(DYN_ARRAY, int)* counts) {
	TEMPLATE(print, int)(indexes);
	TEMPLATE(print, int)(counts);

	// payment
	char pan[PAN_LIMIT];
	pay(hGraphicLib, pan, sizeof(pan) / sizeof(pan[0]));
	if (strlen(pan) == 0) {
		return 1;
	}

	// print document
	print_document(hGraphicLib, file_schema, indexes, counts, pan);
	return 0;
}

int get_count_of_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	const  char *menu[] = {
			"Enter count",
			"Exit",
			0
	};

	int count = 0;
	char choice = 0;
	int max_len = 5;
	char* field_value;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", menu, choice,
				GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

		if (choice == 1) break;

		field_value = read_field(hGraphicLib, "Enter count of good", 0, max_len, NUMBER_GOOD, 0);
		if (field_value == NULL) {
			print_message(hGraphicLib, "Error in input field");
			return 1;
		}

		if (convert_to_int_value(field_value, &count)) {
			free(field_value);
			print_message(hGraphicLib, "cannot convert to integer value");
			return 1;
		}

		free(field_value);
	} while (choice != 1);

	return count;
}

int form_cart_and_buy(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	// first field values
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 0;
	}

	char** first_fields = (char** ) malloc((goods_count + 3) * sizeof(char*));
	get_first_fields(file_schema, first_fields);
	first_fields[goods_count] = "Buy";
	first_fields[goods_count + 1] = "Exit";
	first_fields[goods_count + 2] = NULL;

	int count = 0;
	TEMPLATE(DYN_ARRAY, int) indexes; // keep indexed of goods
	TEMPLATE(DYN_ARRAY, int) counts;  // keep number of each goods in cart
	TEMPLATE(create, int)(1, &indexes);
	TEMPLATE(create, int)(1, &counts);

	int i;
	char choice = 0;
	do {
		choice = GL_Dialog_Choice(hGraphicLib, "Choose goods", first_fields,
				choice, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
	    (void)choice;

		if (choice == goods_count + 1) {
			break;
		} else if (choice == goods_count) {
			if (indexes.length == 0) {
				print_message(hGraphicLib, "empty shopping cart");
				continue;
			}
			for (i = 0; i < CARD_READ_LIMIT; i++) {
				if (pay_for_goods(hGraphicLib, file_schema, &indexes, &counts) != 0) {
					char msg[LOG_MSG_LENGTH_LIMIT];
					sprintf(msg, "Cannot read card. Attempt %d from %d", i+1, CARD_READ_LIMIT);
					print_message(hGraphicLib, msg);
				} else {
					print_message(hGraphicLib, "Thank you!");
					break;
				}
			}

			if (i == 5) {
				print_message(hGraphicLib, "Payment was not completed");
			}
			break;
		} else {
			count = get_count_of_good(hGraphicLib, file_schema);
			if (count != 0) {
				TEMPLATE(append, int)(&indexes, choice);
				TEMPLATE(append, int)(&counts, count);
			}
		}

	} while(choice != goods_count + 1);

	TEMPLATE(destroy, int)(&indexes);
	TEMPLATE(destroy, int)(&counts);
	free(first_fields);
	return 0;
}

void create_menu(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	char choice = 0;
	int main_menu_item_delay = 2;                  // delay for open menu item

	const char* main_menu[] = {
			"Import file with goods",
			"Print list of goods",
			"Modify list of goods",                // Add / Delete
			"Find goods by template",
			"Find goods by categories",
			"Form shopping cart and pay for it",
			"Exit",
			0
	};

	initUIContext(_OFF_);

	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", main_menu, choice,
				GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

		switch (choice) {
		case 0:                                    // IMPORT
			if (file_schema == NULL) {
				print_message(hGraphicLib, "Internal error: file_schema == NULL");
				break;
			}

			if (is_empty_file_schema(file_schema) == 0) {
				print_message(hGraphicLib, "Delete current list of goods");
				destroy_file_schema(file_schema);
				file_schema = create_file_schema();
			}
			import_file_with_goods(hGraphicLib, file_schema);
			break;
		case 1:                                    // PRINT
			if (file_schema == NULL) {
				print_message(hGraphicLib, "Internal error: file_schema == NULL");
				break;
			}
			print_list_with_goods(hGraphicLib, file_schema);
			break;
		case 2:                                    // ADD / DELETE
			modify_list_with_goods(hGraphicLib, file_schema);
			break;
		case 3:                                    // FIND
			find_goods_by_template(hGraphicLib, file_schema);
			break;
		case 4:
			find_goods_by_categories(hGraphicLib, file_schema);
			break;
		case 5:                                    // FORM CART AND BUY
			form_cart_and_buy(hGraphicLib, file_schema);
			break;
		default:
			// Exit
			break;
		}
	} while (choice != 6);
}

void main_processing(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	create_menu(hGraphicLib, file_schema);
}
