#include <stdlib.h>

#include "log.h"
#include "defs.h"
#include "Goal.h"
#include "Menu.h"
#include "iofile.h"
#include "file_schema.h"
#include "main_processing.h"

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
		iRet = read_file_with_goods(filename, file_schema, hGraphicLib);
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
	for (i = 0; i < fields_amount-1; i++) {
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

	// add last field
	char* field_name;
	TEMPLATE(get, string)(&file_schema->header->fields, fields_amount-1, &field_name);
	GOOD_FIELD_TYPE type;
	TEMPLATE(get, good_field)(&file_schema->header->types, fields_amount-1, &type);
	curr_pos = strlen(good_fields);
	switch (type) {
	case BOOL_GOOD:
	case NUMBER_GOOD:
		TEMPLATE(get, vop)(&fields_value, fields_amount-1, (void**)(&val));
		sprintf(good_fields + curr_pos, "%s: %d", field_name, *val);
		break;
	case STRING_GOOD:
		TEMPLATE(get, vop)(&fields_value, fields_amount-1, (void**)(&str));
		sprintf(good_fields + curr_pos, "%s: %s", field_name, str);
		break;
	default:
		break;
	}

	print_message(hGraphicLib, good_fields);
	free(good_fields);
	return 0;
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

	// collect first field values
	char** first_fields = (char** ) malloc((goods_count + 2) * sizeof(char*));
	first_fields[goods_count] = "Exit";
	first_fields[goods_count + 1] = NULL;

	int i;
	char* field_value;
	TEMPLATE(DYN_ARRAY, vop) array_void;
	for (i = 0; i < goods_count; i++) {
		// get goods fields values
		TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, i, &array_void);
		TEMPLATE(get, vop)(&array_void, 0, (void**)(&field_value));
		first_fields[i] = field_value;
	}

	char choice = 0;
	do {
		// display menu with first fields
		choice = GL_Dialog_Menu(hGraphicLib, "Select good", first_fields, choice,
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

void create_menu(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	char choice = 0;
	int main_menu_item_delay = 2;                  // delay for open menu item

	const char* main_menu[] = {
			"Import file with goods",
			"Print list of goods",
			"Modify list of goods",
			"Find goods by template",
			"Form shopping cart and pay for it",
			"Exit",
			0
	};

	initUIContext(_OFF_);

	do {
		choice = GL_Dialog_Menu(gGoalGraphicLibInstance, "Select action", main_menu, choice,
				GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

		GL_Dialog_Message(gGoalGraphicLibInstance, main_menu[choice],
				main_menu[choice], GL_ICON_INFORMATION, GL_BUTTON_VALID,
				main_menu_item_delay * GL_TIME_SECOND);

		switch (choice) {
		case 0:
			// TODO
			// add dialog_file for choosing file
			// read file schema
			// Add GL_Dialog_Progress
			import_file_with_goods(hGraphicLib, file_schema);
			break;
		case 1:
			// TODO
			// print list with goods
			print_list_with_goods(hGraphicLib, file_schema);
			break;
		case 2:
			// TODO
			// print list with goods
			// BUTTON_CANCEL -> delete selected good
			// BUTTON_VALID -> add new good
			// F -> edit selected good
			//
			break;
		case 3:
			// TODO
			// find good
			// for each field print dialog_virtual_keyboard
			// check each field for valid value
			// then find goods by this template
			// print result in separate menu
			// in this menu goods are immutable
			break;
		case 4:
			// TODO
			// form shopping cart
			// print check box
			// for each good read count of goods
			// push button "pay"
			// then insert magnetic card
			// read pan from card
			// print message with printer
			break;
		default:
			// Exit
			break;
		}
	} while (choice != 5);
}

void main_processing(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	print_startup_message(hGraphicLib);
	create_menu(hGraphicLib, file_schema);

}
