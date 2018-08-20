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

void display_startup_message(T_GL_HGRAPHIC_LIB hGraphicLib) {
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

lblKO:                                            // None-classified low level error
	GL_Dialog_Message(hGraphicLib, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;

lblEnd:
	if (hScreen) {
		GoalDestroyScreen(&hScreen);
	}
}

int backup_file_schema(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int type) {
	// backup current file_schema if it is not empty
	int iRet;
	if (is_empty_file_schema(file_schema) == 0) {
		iRet = write_file_schema_to_file(BACKUP_FILE_SCHEMA, file_schema, STATIC_FILE_TYPE, hGraphicLib);
		if (iRet == 1) {
			print_message(hGraphicLib, "Cannot backup current list of good");
			return 1;
		} else {
			if (type == DELETE_FILE_SCHEMA) {
				// delete current file_schema from RAM
				destroy_file_schema(file_schema);
				file_schema = create_file_schema();
			}
		}
	}

	return 0;
}

int restore_file_schema(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int iRet;

	// destroy current file_schema in RAM if it is not empty
	if (is_empty_file_schema(file_schema) == 0) {
		destroy_file_schema(file_schema);
		file_schema = create_file_schema();
	}

	iRet = read_file_with_goods(BACKUP_FILE_SCHEMA, file_schema, hGraphicLib);
	CHECK(iRet == 0, lblReleaseResources);
	print_message(hGraphicLib, "list of goods was successfully restored");
	goto lblEnd;

lblReleaseResources:
	print_message(hGraphicLib, "cannot restore list of goods");
	return 1;

lblEnd:

	return 0;
}

int import_file_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	T_GL_HWIDGET screen = NULL;
	char filename[FILENAME_LIMIT];

	if (GL_Dialog_File(hGraphicLib,
			"Select file", TERMINAL_DATA_DIR, GOODS_FILE_FORMAT,
			filename, sizeof(filename), 0,
			GL_TIME_INFINITE) == GL_KEY_VALID)
	{
		int iRet = 0;
		iRet = backup_file_schema(hGraphicLib, file_schema, DELETE_FILE_SCHEMA);
		CHECK(iRet == 0, lblReleaseResources);

		// read new list of goods
		iRet = read_file_with_goods(filename, file_schema, hGraphicLib);
		if (iRet == 1) {
			print_message(hGraphicLib, "Error while import list of goods");
			iRet = restore_file_schema(hGraphicLib, file_schema);
			CHECK(iRet == 0, lblReleaseResources);
		} else {
			print_message(hGraphicLib, "List with goods\nwas successfully imported");
			print_file_schema(file_schema);
			iRet = backup_file_schema(hGraphicLib, file_schema, ONLY_BACKUP_FILE_SCHEMA);
			CHECK(iRet == 0, lblReleaseResources);
		}
	}
	goto lblEnd;

lblReleaseResources:
	if (screen != NULL) {
		GoalDestroyScreen(&screen);
	}
	return 1;

lblEnd:
	if (screen != NULL) {
		GoalDestroyScreen(&screen);
	}

	return 0;
}

int get_fields_values_len(FILE_SCHEMA* file_schema) {
	int result = 0;
	TEMPLATE(DYN_ARRAY, int)* length_max = &file_schema->header->length_max;
	int fields_count = length_max->length;

	int i;
	int limit = 0;
	for (i = 0; i < fields_count; i++) {
		TEMPLATE(get, int)(length_max, i, &limit);
		result += limit;
	}

	return result;
}

int get_max_field_len(FILE_SCHEMA* file_schema) {
	int result = 0;
	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;

	int i;
	int len;
	char* str;
	for (i = 0; i < fields->length; i++) {
		TEMPLATE(get, string)(fields, i, &str);
		len = strlen(str);
		result = (result < len) ? len : result;
	}

	return result;
}

// show_type == 0 -> display
// show_type == 1 -> print
int show_good_fields(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema,
		TEMPLATE(DYN_ARRAY, vop) fields_value, int show_type, T_GL_HWIDGET hLayout, T_GL_DIM* usLine)
{
	int fields_amount = fields_value.length;
	int total_len = get_fields_values_len(file_schema);
	// add fields_amount - 1 bytes for separators
	// add fields_amount * max_fields_len for field names
	int max_field_len = get_max_field_len(file_schema);
	char* good_fields = NULL;
	good_fields = (char*) umalloc(sizeof(char) * (total_len + (max_field_len + 2) * fields_amount +
									   (fields_amount - 1) + 1));
	CHECK(good_fields != NULL, lblMemoryError);
	good_fields[0] ='\0' ;

	int iRet = 0;
	int i;
	int* val;
	char* str;
	int curr_pos;
	for (i = 0; i < fields_amount; i++) {
		char* field_name;
		iRet = TEMPLATE(get, string)(&file_schema->header->fields, i, &field_name);
		CHECK(iRet == 0, lblGetError);
		GOOD_FIELD_TYPE type;
		iRet = TEMPLATE(get, good_field)(&file_schema->header->types, i, &type);
		CHECK(iRet == 0, lblGetError);
		curr_pos = strlen(good_fields);
		switch (type) {
		case BOOL_GOOD:
		case NUMBER_GOOD:
			iRet = TEMPLATE(get, vop)(&fields_value, i, (void**)(&val));
			CHECK(iRet == 0, lblGetError);
			if (show_type == 0) {
				sprintf(good_fields + curr_pos, "%s: %d\n", field_name, *val);
			} else {
				sprintf(good_fields + curr_pos, "%d\n", *val);
			}
			break;
		case STRING_GOOD:
			iRet = TEMPLATE(get, vop)(&fields_value, i, (void**)(&str));
			CHECK(iRet == 0, lblGetError);
			if (show_type == 0) {
				sprintf(good_fields + curr_pos, "%s: %s\n", field_name, str);
			} else {
				sprintf(good_fields + curr_pos, "%s\n", str);
			}
			break;
		default:
			break;
		}
	}

	if (show_type == 0) {
		print_message(hGraphicLib, good_fields);
	} else {
		T_GL_HWIDGET hPrint;
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_MEDIUM);
		GL_Widget_SetItem(hPrint, 0, (*usLine)++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_CENTER);
		GL_Widget_SetText(hPrint, good_fields);

		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_XLARGE);
		GL_Widget_SetItem(hPrint, 0, (*usLine)++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_CENTER);
		GL_Widget_SetText(hPrint, "------------------------------");
	}
	goto lblEnd;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get good field value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor good fields", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (good_fields != NULL) {
		ufree(good_fields);
	}
	return 1;

lblEnd:
	if (good_fields != NULL) {
		ufree(good_fields);
	}
	return 0;
}

int get_first_fields(FILE_SCHEMA* file_schema, char** first_fields, T_GL_HGRAPHIC_LIB hGraphicLib) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);

	int i;
	int iRet = 0;
	char* field_value;
	TEMPLATE(DYN_ARRAY, vop) array_void;
	for (i = 0; i < goods_count; i++) {
		// get goods fields values
		iRet = TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, i, &array_void);
		CHECK(iRet == 0, lblGetGoodError);
		iRet = TEMPLATE(get, vop)(&array_void, 0, (void**)(&field_value));
		CHECK(iRet == 0, lblGetFieldError);
		first_fields[i] = field_value;
	}
	goto lblEnd;

lblGetGoodError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get good from list", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	return 1;

lblGetFieldError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get first field", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	return 1;

lblEnd:
	return 0;
}

int display_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	int iRet = 0;
	GOOD_FIELD_TYPE first_field_type;
	TEMPLATE(get, good_field)(&file_schema->header->types, 0, &first_field_type);
	if (first_field_type != STRING_GOOD) {
		print_message(hGraphicLib, "first good field must be a string");
		return 1;
	}

	// first field values
	char** first_fields = NULL;
	first_fields = (char** ) umalloc((goods_count + 2) * sizeof(char*));
	CHECK(first_fields != NULL, lblMemoryError);
	iRet = get_first_fields(file_schema, first_fields, hGraphicLib);
	CHECK(iRet == 0, lblGetFirstFieldsError);
	first_fields[goods_count] = "Exit";
	first_fields[goods_count + 1] = NULL;

	T_GL_WCHAR choice = 0;
	TEMPLATE(DYN_ARRAY, vop) array_void;
	do {
		// display menu with first fields
		choice = GL_Dialog_Menu(hGraphicLib, "List of goods", first_fields, choice,
				GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);

		if (choice == goods_count || choice == GL_KEY_CANCEL) {
			break;
		}

		// get good fields values
		TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, (int)choice, &array_void);
		GL_Dialog_Message(gGoalGraphicLibInstance, first_fields[(unsigned char)choice],
				first_fields[(unsigned char)choice], GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);

		// display fields
		show_good_fields(hGraphicLib, file_schema, array_void, 0, NULL, NULL);

	} while (choice != GL_KEY_CANCEL && choice != goods_count);

	goto lblEnd;

lblGetFirstFieldsError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get first fields", GL_ICON_ERROR, GL_BUTTON_VALID, 2*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor first fields", GL_ICON_ERROR, GL_BUTTON_VALID, 2*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (first_fields != NULL ) {
		ufree(first_fields);
	}
	return 1;

lblEnd:
	if (first_fields != NULL ) {
		ufree(first_fields);
	}
	return 0;
}

char* read_field(T_GL_HGRAPHIC_LIB graphicLib, char* field_name,
		int min_size, int max_size, GOOD_FIELD_TYPE type, int is_pattern)
{
	char* field = NULL;
	field = (char*) umalloc(sizeof(char) * (max_size + 1));
	CHECK(field != NULL, lblMemoryError);

	int i;
	ulong result;
	char* template = NULL;

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
		// "\u" * max_size, one '|', 10 digits, one '*' and '\0'
		template = (char*) umalloc(max_size*2 + 1 + 10 + 1 + 1);
		CHECK(template != NULL, lblMemoryError);
		for (i = 0; i < max_size*2; i += 2) {
			template[i] = '/';
			template[i+1] = 'u';
		}

		if (is_pattern) {
			strncpy(template + i, "|0123456789*", strlen("|0123456789*"));
		} else {
			strncpy(template + i, "|0123456789", strlen("|0123456789"));
		}

		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				template, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;
		break;
	case STRING_GOOD:
		// "/u" * max_size, one '|', 52 letters (upper and lower cases), 10 digits,
		// one space, one dot, one '*' and '\0'
		template = (char *) umalloc(max_size*2 + 1 + 52 + 10 + 3 + 1);
		CHECK(template != NULL, lblMemoryError);
		for (i = 0; i < max_size*2; i += 2) {
			template[i] = '/';
			template[i+1] = 'u';
		}

		if (is_pattern) {
			strncpy(template + i, "|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .*",
					strlen("|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .*"));
		} else {
			strncpy(template + i, "|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .",
					strlen("|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ."));
		}

		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				template, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;
		break;
	default:
		result = GL_Dialog_VirtualKeyboard (graphicLib, field_name, "Field value: ",
				0, field, sizeof(char) * (max_size + 1), GL_TIME_INFINITE);
		(void)result;
		break;
	}

	CHECK(strlen(field) >= min_size, lblIncorrectFieldSize);
	goto lblEnd;

lblIncorrectFieldSize:
	GL_Dialog_Message(graphicLib, NULL, "Field size is less than permitted", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(graphicLib, NULL, "Cannot allocate memory\nfor field value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (field != NULL) {
		ufree(field);
	}

	if (template != NULL) {
		ufree(template);
	}
	return NULL;

lblEnd:
	if (template != NULL) {
		ufree(template);
	}
	return field;
}

int convert_to_int_value(char* str, int* value) {
	// if string is empty then 0
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

    int iRet = 0;
	int i;
	GOOD_FIELD_TYPE type;
	int max_len;
	int min_len;
	char* field_name;
	char* field_value = NULL;
	int* val = NULL;
	TEMPLATE(DYN_ARRAY, vop) good_fields;
	iRet = TEMPLATE(create, vop)(fields->length, &good_fields);
	CHECK(iRet == 0, lblCreationError);
	for (i = 0; i < fields->length; i++) {
		iRet = TEMPLATE(get, good_field)(types, i, &type);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(min_limit, i, &min_len);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(max_limit, i, &max_len);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, string)(fields, i, &field_name);
		CHECK(iRet == 0, lblGetError);

		field_value = read_field(hGraphicLib, field_name, min_len, max_len, type, 0);
		CHECK(field_value != NULL, lblErrorField);

		if (type == BOOL_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_bool_value(field_value, val);
			CHECK(iRet == 0, lblConvertToBoolError);
			TEMPLATE(append, vop)(&good_fields, (void *)val);
			TEMPLATE(append, int)(good_fields.types, BOOL);
			free(field_value);  // free memory for the next value
		} else if (type == NUMBER_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_int_value(field_value, val);
			CHECK(iRet == 0, lblConvertToNumError);
			TEMPLATE(append, vop)(&good_fields, (void *)val);
			TEMPLATE(append, int)(good_fields.types, INT);
			free(field_value);  // free memory for the next value
		} else if (type == STRING_GOOD) {
			TEMPLATE(append, vop)(&good_fields, (void*)field_value);
			TEMPLATE(append, int)(good_fields.types, STRING);
		} else {
			goto lblUnknownGoodType;
		}
		field_value =  NULL;
		val = NULL;
	}

	// add good to list
	iRet = TEMPLATE(add_to_list, dyn_array_vop)(goods, good_fields);
	CHECK(iRet == 0, lblAddToListError)
	goto lblEnd;

lblAddToListError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot add good to list", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblUnknownGoodType:
	GL_Dialog_Message(hGraphicLib, NULL, "Internal error: unknown good type", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblConvertToNumError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert field\nto integer value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblConvertToBoolError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert field\nto bool value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblErrorField:
	GL_Dialog_Message(hGraphicLib, NULL, "Error in input field", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblCreationError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot create array\nfor good fields", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get field name,\ntype or min/max len", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor field value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (field_value != NULL) {
		ufree(field_value);
	}

	if (val != NULL) {
		ufree(val);
	}
	TEMPLATE(destroy, vop)(&good_fields);
	return 1;

lblEnd:
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
	int iRet = 0;
	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;
	TEMPLATE(DYN_ARRAY, good_field)* types = &file_schema->header->types;
	TEMPLATE(DYN_ARRAY, int)* min_limit = &file_schema->header->length_min;
	TEMPLATE(DYN_ARRAY, int)* max_limit = &file_schema->header->length_max;
	TEMPLATE(DYN_ARRAY, vop) good_fields;
	iRet = TEMPLATE(get_by_index, dyn_array_vop)(file_schema->goods, index, &good_fields);
	CHECK(iRet == 0, lblGetError);

	char** fields_name = NULL;
	int fields_amount = fields->length;
	fields_name = (char** ) umalloc(sizeof(char*) * (fields_amount + 2));
	CHECK(fields_name != NULL, lblMemoryError);

	int i;
	char* str;
	for (i = 0; i < fields_amount; i++) {
		iRet = TEMPLATE(get, string)(fields, i, &str);
		CHECK(iRet == 0, lblGetError);
		fields_name[i] = str;
	}
	fields_name[fields_amount] = "Exit";
	fields_name[fields_amount + 1] = NULL;

	GOOD_FIELD_TYPE type;
	int max_len;
	int min_len;
	char* field_value = NULL;
	int* val = NULL;
	T_GL_WCHAR choice = 0;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select field", fields_name, choice,
				GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);

		if (choice == fields_amount || choice == GL_KEY_CANCEL) {
			break;
		}

		// read value of new field and edit good
		iRet = TEMPLATE(get, good_field)(types, choice, &type);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(min_limit, choice, &min_len);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(max_limit, choice, &max_len);
		CHECK(iRet == 0, lblGetError);

		field_value = read_field(hGraphicLib, fields_name[choice], min_len, max_len, type, 0);
		if (field_value == NULL) {
			continue;
		}

		if (type == BOOL_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_bool_value(field_value, val);
			CHECK(iRet == 0, lblConvertToBoolError);
			iRet = TEMPLATE(insert, vop)(&good_fields, choice, (void *)val);
			CHECK(iRet == 0, lblInsertError);
			free(field_value);  // free memory for the next value
		} else if (type == NUMBER_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_int_value(field_value, val);
			CHECK(iRet == 0, lblConvertToNumError);
			iRet = TEMPLATE(insert, vop)(&good_fields, choice, (void *)val);
			CHECK(iRet == 0, lblInsertError);
			free(field_value);  // free memory for the next value
		} else if (type == STRING_GOOD) {
			TEMPLATE(insert, vop)(&good_fields, choice, (void*)field_value);
			CHECK(iRet == 0, lblInsertError);
		} else {
			goto lblUnknownGoodType;
		}
		field_value =  NULL;
		val = NULL;

	} while (choice != GL_KEY_CANCEL && choice != fields_amount);

	goto lblEnd;

lblConvertToBoolError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert\n value to bool", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblConvertToNumError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert\n value to integer", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblUnknownGoodType:
	GL_Dialog_Message(hGraphicLib, NULL, "Internal error: unknown good type", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblInsertError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot insert new value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate\nmemory for field names", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get\ngood fields", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (fields_name != NULL) {
		ufree(fields_name);
	}

	if (field_value != NULL) {
		ufree(field_value);
	}

	if (val != NULL) {
		ufree(val);
	}

	return 1;

lblEnd:

	return 0;
}

int change_good(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, int index) {
	const  char *menu[] = {
			"Delete",
			"Edit",
			"Exit",
			0
	};

	int iRet = 0;
	int menu_len = 3;
	T_GL_WCHAR choice = 0;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", menu, choice,
				GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);

		if (choice == menu_len - 1 || choice == GL_KEY_CANCEL) {
			break;
		} else if (choice == 0) {
			iRet = delete_good(hGraphicLib, file_schema, index);
			CHECK(iRet == 0, lblChangeGoodError);
			break;
		} else {
			iRet = edit_good(hGraphicLib, file_schema, index);
			CHECK(iRet == 0, lblChangeGoodError);
			break;
		}


	} while (choice != GL_KEY_CANCEL && choice != menu_len - 1);
	goto lblEnd;

lblChangeGoodError:
	return 1;

lblEnd:
	return 0;
}

int delete_several_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	TEMPLATE(LIST, dyn_array_vop)* goods = file_schema->goods;
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(goods);

	int i;
	int iRet = 0;
	char** first_fields = NULL;
	bool* checked = NULL;
	first_fields = (char** ) umalloc((goods_count + 2) * sizeof(char*));
	CHECK(first_fields != NULL, lblMemoryError);
	iRet = get_first_fields(file_schema, first_fields, hGraphicLib);
	CHECK(iRet == 0, lblGetFirstFieldsError);
	first_fields[goods_count] = "Delete";
	first_fields[goods_count + 1] = NULL;

	checked = (bool *) umalloc(sizeof(bool) * (goods_count + 1)); // check +1 instead +2
	CHECK(checked != NULL, lblMemoryError);
	for (i = 0; i < goods_count; i++) {
		checked[i] = false;
	}


	T_GL_WCHAR choice = 0;
	do {
		choice = GL_Dialog_MultiChoice(hGraphicLib, "Categories",
				first_fields, choice, checked, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		(void)choice;

		if (choice == GL_KEY_CANCEL) {
			break;
		}

		if (choice == goods_count) {
			for (i = goods_count-1; i >= 0; i--) {
				if (checked[i]) {
					iRet = delete_good(hGraphicLib, file_schema, i);
					CHECK(iRet == 0, lblDeleteGoodError);
				}
			}
			break;
		}

		checked[choice] = !checked[choice];
	} while (choice != GL_KEY_CANCEL);
	goto lblEnd;

lblGetFirstFieldsError:
	goto lblReleaseResources;

lblDeleteGoodError:
	goto lblReleaseResources;

lblMemoryError:
	goto lblReleaseResources;

lblReleaseResources:
	if (checked != NULL) {
		ufree(checked);
	}

	if (first_fields != NULL) {
		ufree(first_fields);
	}

	return 1;

lblEnd:
	return 0;
}

int modify_list_with_goods(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	// first field values
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	int iRet = 0;
	char** first_fields = NULL;
	first_fields = (char** ) umalloc((goods_count + 5) * sizeof(char*));
	CHECK(first_fields != NULL, lblMemoryError);

	iRet = get_first_fields(file_schema, first_fields, hGraphicLib);
	CHECK(iRet == 0, lblGetFirstFieldsError);
	first_fields[goods_count] = "--------------------";
	first_fields[goods_count + 1] = "Add new good";
	first_fields[goods_count + 2] = "Delete several goods";
	first_fields[goods_count + 3] = "Exit";
	first_fields[goods_count + 4] = NULL;

	T_GL_WCHAR choice = 0;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "List of goods", first_fields,
				choice, GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);
	    (void)choice;

		if (choice == goods_count + 3 || choice == GL_KEY_CANCEL) {
			break;
		} else if (choice == goods_count) {
			continue;
		} else if (choice == goods_count + 1) {
			iRet = add_new_good(hGraphicLib, file_schema);
			CHECK(iRet == 0, lblAddGoodError);
			iRet = backup_file_schema(hGraphicLib, file_schema, ONLY_BACKUP_FILE_SCHEMA);
			CHECK(iRet == 0, lblReleaseResources);
			break;
		} else if (choice == goods_count + 2) {
			iRet = delete_several_goods(hGraphicLib, file_schema);
			CHECK(iRet == 0, lblBatchDeleteError);
			iRet = backup_file_schema(hGraphicLib, file_schema, ONLY_BACKUP_FILE_SCHEMA);
			CHECK(iRet == 0, lblReleaseResources);
			break;
		}else {
			iRet = change_good(hGraphicLib, file_schema, choice);
			CHECK(iRet == 0, lblEditGoodError);
			iRet = backup_file_schema(hGraphicLib, file_schema, ONLY_BACKUP_FILE_SCHEMA);
			CHECK(iRet == 0, lblReleaseResources);
			break;
		}

	} while(choice != GL_KEY_CANCEL && choice != goods_count + 3);
	goto lblEnd;

lblBatchDeleteError:
	goto lblReleaseResources;

lblEditGoodError:
	goto lblReleaseResources;

lblAddGoodError:
	goto lblReleaseResources;

lblGetFirstFieldsError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get first fields", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate\nmemory", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (first_fields != NULL) {
		ufree(first_fields);
	}
	return 1;

lblEnd:
	if (first_fields != NULL) {
		ufree(first_fields);
	}
	return 0;
}

void display_message_search_menu(T_GL_HGRAPHIC_LIB hGraphicLib) {
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

	int iRet = 0;
	int i;
	GOOD_FIELD_TYPE type;
	int max_len;
	int min_len;
	char* field_name;
	char* field_value = NULL;
	char* asterisk = "*";
	int* val = NULL;
	for (i = 0; i < fields->length; i++) {
		iRet = TEMPLATE(get, good_field)(types, i, &type);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(min_limit, i, &min_len);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, int)(max_limit, i, &max_len);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, string)(fields, i, &field_name);
		CHECK(iRet == 0, lblGetError);

		field_value = read_field(hGraphicLib, field_name, min_len, max_len, type, 1);
		CHECK(field_value != NULL, lblErrorField);

		if (strcmp(field_value, "*\0") == 0) {
			char* str = (char *) umalloc(sizeof(char) * 2);
			CHECK(str != NULL, lblMemoryError);
			str = strcpy(str, asterisk);
			TEMPLATE(append, vop)(pattern, (void *)str);
			TEMPLATE(append, int)(pattern->types, STRING);
			ufree(field_value); // free memory for the next value
			field_value = NULL;
			continue;
		}

		if (type == BOOL_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_bool_value(field_value, val);
			CHECK(iRet == 0, lblConvertToBoolError);
			TEMPLATE(append, vop)(pattern, (void *)val);
			TEMPLATE(append, int)(pattern->types, BOOL);
			ufree(field_value); // free memory for the next value
		} else if (type == NUMBER_GOOD) {
			val = (int *) umalloc(sizeof(int));
			CHECK(val != NULL, lblMemoryError);
			iRet = convert_to_int_value(field_value, val);
			CHECK(iRet == 0, lblConvertToNumError);
			TEMPLATE(append, vop)(pattern, (void *)val);
			TEMPLATE(append, int)(pattern->types, INT);
			ufree(field_value); // free memory for the next value
		} else if (type == STRING_GOOD) {
			TEMPLATE(append, vop)(pattern, (void*)field_value);
			TEMPLATE(append, int)(pattern->types, STRING);
		} else {
			goto lblUnknownGoodType;
		}
		field_value = NULL;
		val = NULL;
	}
	goto lblEnd;

lblUnknownGoodType:
	GL_Dialog_Message(hGraphicLib, NULL, "Internal error: unknown good type", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblConvertToNumError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert field\nto integer value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblConvertToBoolError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert field\nto bool value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor pattern field", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblErrorField:
	GL_Dialog_Message(hGraphicLib, NULL, "Error in input field", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get\nvalue from array", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (field_value != NULL) {
		ufree(field_value);
	}

	if (val != NULL) {
		ufree(val);
	}
	return 1;

lblEnd:
	if (field_value != NULL) {
		ufree(field_value);
	}

	if (val != NULL) {
		ufree(val);
	}
	print_message(hGraphicLib, "pattern was successfully read");
	return 0;
}

int display_search_results(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(LIST, dyn_array_vop)* res) {
	FILE_SCHEMA* tmp_file_schema = (FILE_SCHEMA *) umalloc(sizeof(FILE_SCHEMA));

	tmp_file_schema->header = (FILE_HEADER *) umalloc(sizeof(FILE_HEADER));
	tmp_file_schema->header->fields = file_schema->header->fields;
	tmp_file_schema->header->types = file_schema->header->types;
	tmp_file_schema->header->length_min = file_schema->header->length_min;
	tmp_file_schema->header->length_max = file_schema->header->length_max;

	tmp_file_schema->goods = res;
	display_list_with_goods(hGraphicLib, tmp_file_schema);

	ufree(tmp_file_schema->header);
	ufree(tmp_file_schema);
	return 0;
}

int find_goods_by_template(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int iRet = 0;

	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	display_message_search_menu(hGraphicLib);
	int fields_amount = file_schema->header->fields.length;
	TEMPLATE(DYN_ARRAY, vop) pattern;
	iRet = TEMPLATE(create, vop)(fields_amount, &pattern);
	CHECK(iRet == 0, lblCreationError);
	TEMPLATE(LIST, dyn_array_vop)* res = NULL;
	iRet = read_search_pattern(hGraphicLib, file_schema, &pattern);
	CHECK(iRet == 0, lblPatternError);
	res = TEMPLATE(search, dyn_array_vop)(file_schema->goods, &pattern);
	display_search_results(hGraphicLib, file_schema, res);
	goto lblEnd;

lblPatternError:
	GL_Dialog_Message(hGraphicLib, NULL, "Error in pattern reading", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblCreationError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate\nmemory for pattern", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (res != NULL) {
		TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
	}
	TEMPLATE(destroy, vop)(&pattern);
	return 1;

lblEnd:
	if (res != NULL) {
		TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
	}
	TEMPLATE(destroy, vop)(&pattern);
	return 0;
}

int get_categories_pattern(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema,
		char** bool_fields, bool* checked, int bool_fields_count,
		TEMPLATE(DYN_ARRAY, vop)* pattern)
{
	TEMPLATE(DYN_ARRAY, good_field)* types = &file_schema->header->types;
	int fields_amount = file_schema->header->fields.length;

	int i, j;
	j = 0;
	int* val;
	char* str;
	char* asterisk = "*";
	GOOD_FIELD_TYPE type;
	for (i = 0; i < fields_amount; i++) {
		TEMPLATE(get, good_field)(types, i, &type);
		if (type != BOOL_GOOD || (type == BOOL_GOOD && checked[j] == false)) {
			str = (char*) umalloc(sizeof(char) * 2);
			CHECK(str != NULL, lblErrorAllocation)
			str = strncpy(str, asterisk, 2);
			TEMPLATE(append, vop)(pattern, (void*)str);
			TEMPLATE(append, int)(pattern->types, STRING);
			j = (type == BOOL_GOOD) ? j + 1 : j;
			str = NULL;
			continue;
		}

		val = (int*) umalloc(sizeof(int));
		CHECK(val != NULL, lblErrorAllocation);
		*val = (checked[j] == true) ? 1 : 0;
		TEMPLATE(append, vop)(pattern, (void* ) val);
		TEMPLATE(append, int)(pattern->types, BOOL);
		val = NULL;
		++j;
	}
	goto lblEnd;

lblErrorAllocation:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor field value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	if (str != NULL) {
		ufree(str);
	}

	if (val != NULL) {
		ufree(val);
	}
	return 1;

lblEnd:
	if (str != NULL) {
		ufree(str);
	}

	if (val != NULL) {
		ufree(val);
	}
	return 0;
}

int print_header_for_search_result(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, T_GL_HWIDGET hLayout, T_GL_DIM* usLine) {
	T_GL_HWIDGET hPrint;
	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, (*usLine)++);
	GL_Widget_SetBorders(hPrint, 4, 4, 4, 4, GL_UNIT_PIXEL, GL_COLOR_BLACK);
	GL_Widget_SetText(hPrint, "SEARCH RESULT");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XLARGE);
	GL_Widget_SetItem(hPrint, 0, (*usLine)++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_CENTER);
	GL_Widget_SetText(hPrint, "FIELDS:");

	TEMPLATE(DYN_ARRAY, string)* fields = &file_schema->header->fields;
	int fields_amount = fields->length;
	int max_field_len = get_max_field_len(file_schema);
	char* good_fields = NULL;
	good_fields = (char*) umalloc(sizeof(char) * (max_field_len * fields_amount +
						          fields_amount + 1));
	CHECK(good_fields != NULL, lblMemoryError);
	good_fields[0] ='\0' ;

	int i;
	int iRet = 0;
	int curr_pos;
	char* field_name;
	for (i = 0; i < fields->length; i++) {
		iRet = TEMPLATE(get, string)(fields, i, &field_name);
		CHECK(iRet == 0, lblGetError);
		curr_pos = strlen(good_fields);
		sprintf(good_fields + curr_pos, "%s\n", field_name);
	}

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_MEDIUM);
	GL_Widget_SetItem(hPrint, 0, (*usLine)++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_CENTER);
	GL_Widget_SetText(hPrint, good_fields);

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XLARGE);
	GL_Widget_SetItem(hPrint, 0, (*usLine)++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_CENTER);
	GL_Widget_SetText(hPrint, "------------------------------");
	goto lblEnd;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get field name", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\nfor fields names", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (good_fields != NULL) {
		ufree(good_fields);
	}
	return 1;

lblEnd:
	ufree(good_fields);
	return 0;
}

int print_search_results(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema, TEMPLATE(LIST, dyn_array_vop)* res) {
	if (TEMPLATE(size_list, dyn_array_vop)(res) == 0) {
		return 0;
	}

	T_GL_HWIDGET hDocument, hLayout, hPrint;
	T_GL_DIM usLine = 0;
	hDocument = GL_Document_Create(hGraphicLib);
	hLayout = GL_Layout_Create(hDocument);

	print_header_for_search_result(hGraphicLib, file_schema, hLayout, &usLine);

	int i;
	int iRet = 0;
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(res);
	TEMPLATE(DYN_ARRAY, vop) fields_value;
	for (i = 0; i < goods_count; i++) {
		iRet = TEMPLATE(get_by_index, dyn_array_vop)(res, i, &fields_value);
		CHECK(iRet == 0, lblGetError);
		iRet = show_good_fields(hGraphicLib, file_schema, fields_value, 1, hLayout, &usLine);
		CHECK(iRet == 0, lblGoodPrintError);
	}

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);

	GL_Widget_SetText(hPrint, "END\n\n\n\n\n\n");
	goto lblEnd;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot good\nfrom list of goods", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblGoodPrintError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot print good", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	GL_Widget_Destroy(hDocument);
	return 1;

lblEnd:
	GL_Document_Print(hDocument, 0);
	GL_Widget_Destroy(hDocument);
	return 0;
}

int get_bool_fields_amount(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	TEMPLATE(DYN_ARRAY, good_field)* types = &file_schema->header->types;
	int result = 0;

	int i;
	GOOD_FIELD_TYPE type;
	for (i = 0; i < types->length; i++) {
		TEMPLATE(get, good_field)(types, i, &type);
		if (type == BOOL_GOOD) {
			++result;
		}
	}

	return result;
}

int find_goods_by_categories(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (file_schema == NULL || goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 1;
	}

	int iRet = 0;
	int fields_amount;
	int bool_fields_count;
	bool* checked;
	char** bool_fields;
	TEMPLATE(DYN_ARRAY, vop) pattern;

	fields_amount = file_schema->header->fields.length;
	iRet = TEMPLATE(create, vop)(fields_amount, &pattern);
	CHECK(iRet == 0, lblCreatePatternError);
	bool_fields_count = get_bool_fields_amount(hGraphicLib, file_schema);
	bool_fields = (char **) umalloc(sizeof(char*) * (bool_fields_count + 3));
	CHECK(bool_fields != NULL, lblErrorAllocation);

	int i;
	char* str;
	bool_fields_count = 0;
	GOOD_FIELD_TYPE type;
	for (i = 0; i < fields_amount; i++) {
		iRet = TEMPLATE(get, string)(&file_schema->header->fields, i, &str);
		CHECK(iRet == 0, lblGetError);
		iRet = TEMPLATE(get, good_field)(&file_schema->header->types, i, &type);
		CHECK(iRet == 0, lblGetError);
		if (type == BOOL_GOOD) {
			bool_fields[bool_fields_count++] = str;
		}
	}

	bool_fields[bool_fields_count] = "Find";
	bool_fields[bool_fields_count + 1] = "Exit";
	bool_fields[bool_fields_count + 2] = NULL;

	checked = (bool *) umalloc(sizeof(bool) * (bool_fields_count + 2)); // check +1 instead +2
	CHECK(checked != NULL, lblErrorAllocation);
	for (i = 0; i < bool_fields_count + 2; i++) {
		checked[i] = false;
	}

	T_GL_WCHAR choice = 0;
	TEMPLATE(LIST, dyn_array_vop)* res = NULL;
	do {
		choice = GL_Dialog_MultiChoice(hGraphicLib, "Categories",
				bool_fields, choice, checked, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		(void)choice;

		if (choice == bool_fields_count + 1 || choice == GL_KEY_CANCEL) break;

		if (choice == bool_fields_count) {
			iRet = get_categories_pattern(hGraphicLib, file_schema, bool_fields, checked, bool_fields_count, &pattern);
			CHECK(iRet == 0, lblGetPatternError);
			SLOG("pattern");
			TEMPLATE(print, vop)(&pattern);
			res = TEMPLATE(search, dyn_array_vop)(file_schema->goods, &pattern);
			display_search_results(hGraphicLib, file_schema, res);
			print_search_results(hGraphicLib, file_schema, res);
			break;
		}

		checked[choice] = !checked[choice];
	} while (choice != GL_KEY_CANCEL && choice != bool_fields_count + 1);

	goto lblEnd;

lblGetPatternError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get pattern", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblGetError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get bool\nfield name", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblCreatePatternError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot create pattern", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblErrorAllocation:
	print_message(hGraphicLib, "cannot allocate memory");
	goto lblReleaseResources;

lblReleaseResources:
	TEMPLATE(destroy, vop)(&pattern);
	if (bool_fields != NULL) {
		ufree(bool_fields);
	}

	if (checked != NULL) {
		ufree(checked);
	}

	if (res != NULL) {
		TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
	}

	return 1;

lblEnd:
	TEMPLATE(destroy, vop)(&pattern);
	if (bool_fields != NULL) {
		ufree(bool_fields);
	}

	if (checked != NULL) {
		ufree(checked);
	}

	if (res != NULL) {
		TEMPLATE(destroy_list_lite, dyn_array_vop)(res);
	}

	return 0;
}

void pay(T_GL_HGRAPHIC_LIB hGraphicLib, char* pan, int len) {
	// Flush pan
	pan[0] = '\0';

	// Local variables
    // ***************
	T_GL_HWIDGET hScreen=NULL;
	T_GL_HWIDGET xDocument=NULL;
//	ST_PRN_LINE xLine;
    Telium_File_t *hMag2=NULL;
	byte ucLen=0;
    int iSta;
    char tcTmp[128];
    char *pcSrc, *pcDst;
    char tcTrk2[128];
//    byte p; // Printer line index
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

		// price
		sprintf(document_line, "price: %d", price);
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
		GL_Widget_SetItem(hPrint, 0, usLine++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
		GL_Widget_SetText(hPrint, document_line);

		// count of good
		sprintf(document_line, "count: %d", count);
		hPrint = GL_Print_Create(hLayout);
		GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
		GL_Widget_SetItem(hPrint, 0, usLine++);
		GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
		GL_Widget_SetText(hPrint, document_line);

		// count of good
		sprintf(document_line, "total: %d", price * count);
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

	int iRet = 0;
	int count = 0;
	T_GL_WCHAR choice = 0;
	int max_len = 5;
	char* field_value = NULL;
	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", menu, choice,
				GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);

		if (choice == 1 || choice == GL_KEY_CANCEL) break;

		field_value = read_field(hGraphicLib, "Enter count of good", 0, max_len, NUMBER_GOOD, 0);
		CHECK(field_value != NULL, lblReadCountError);
		iRet = convert_to_int_value(field_value, &count);
		CHECK(iRet == 0, lblConvertToNumError);
		ufree(field_value);
		field_value = NULL;
	} while (choice != GL_KEY_CANCEL && choice != 1);
	goto lblEnd;

lblConvertToNumError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot convert\ninput to integer value", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReadCountError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot read good's count", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblReleaseResources;

lblReleaseResources:
	if (field_value != NULL) {
		ufree(field_value);
	}
	return 0;
lblEnd:
	return count;
}

int form_cart_and_buy(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	// first field values
	int goods_count = TEMPLATE(size_list, dyn_array_vop)(file_schema->goods);
	if (goods_count == 0) {
		print_message(hGraphicLib, "List with goods is empty");
		return 0;
	}

	int iRet = 0;
	bool* checked = NULL;
	char** first_fields = NULL;
	first_fields = (char** ) umalloc((goods_count + 3) * sizeof(char*));
	CHECK(first_fields != NULL, lblMemoryError);
	iRet = get_first_fields(file_schema, first_fields, hGraphicLib);
	CHECK(iRet == 0, lblGetFirstFieldsError);

	first_fields[goods_count] = "Buy";
	first_fields[goods_count + 1] = "Exit";
	first_fields[goods_count + 2] = NULL;

	checked = (bool* ) umalloc(sizeof(bool) * (goods_count + 2));
	CHECK(checked != NULL, lblMemoryError);
	int i;
	for (i = 0; i < goods_count + 2; i++) {
		checked[i] = false;
	}

	int count = 0;
	TEMPLATE(DYN_ARRAY, int) indexes; // keep indexed of goods
	TEMPLATE(DYN_ARRAY, int) counts;  // keep number of each goods in cart
	iRet = TEMPLATE(create, int)(1, &indexes);
	CHECK(iRet == 0, lblCreationError);
	iRet = TEMPLATE(create, int)(1, &counts);
	CHECK(iRet == 0, lblCreationError);

	T_GL_WCHAR choice = 0;
	do {

		choice = GL_Dialog_MultiChoice (hGraphicLib, "Choose goods", first_fields,
				choice, checked, GL_BUTTON_CANCEL, GL_KEY_0, GL_TIME_INFINITE);
	    (void)choice;

		if (choice == goods_count + 1 || choice == GL_KEY_CANCEL) {
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
				checked[(unsigned char)choice] = true;
			}
		}

	} while(choice != GL_KEY_CANCEL && choice != goods_count + 1);
	goto lblEnd;

lblCreationError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot create array\nfor indexes and count goods", GL_ICON_ERROR, GL_BUTTON_VALID, 2*1000);
	goto lblReleaseResources;

lblMemoryError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot allocate memory\n for first fields", GL_ICON_ERROR, GL_BUTTON_VALID, 2*1000);
	goto lblReleaseResources;

lblGetFirstFieldsError:
	GL_Dialog_Message(hGraphicLib, NULL, "Cannot get first fields", GL_ICON_ERROR, GL_BUTTON_VALID, 2*1000);
	goto lblReleaseResources;

lblReleaseResources:
	TEMPLATE(destroy, int)(&indexes);
	TEMPLATE(destroy, int)(&counts);
	if (first_fields != NULL) {
		ufree(first_fields);
	}

	if (checked != NULL) {
		ufree(checked);
	}

	return 1;

lblEnd:
	TEMPLATE(destroy, int)(&indexes);
	TEMPLATE(destroy, int)(&counts);
	if (first_fields != NULL) {
		ufree(first_fields);
	}

	if (checked != NULL) {
		ufree(checked);
	}

	return 0;
}

void create_menu(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	T_GL_WCHAR choice = 0;

	const char* main_menu[] = {
			"Import file with goods",
			"Display list of goods",
			"Modify list of goods",                // Add / Delete
			"Find goods by template",
			"Find goods by categories",
			"Form shopping cart and pay for it",
			"Exit",
			0
	};

	do {
		choice = GL_Dialog_Menu(hGraphicLib, "Select action", main_menu, choice,
				GL_BUTTON_CANCEL , GL_KEY_0, GL_TIME_INFINITE);

		switch (choice) {
		case 0:                                    // IMPORT
			if (file_schema == NULL) {
				print_message(hGraphicLib, "Internal error: file_schema == NULL");
				break;
			}

			import_file_with_goods(hGraphicLib, file_schema);
			break;
		case 1:                                    // PRINT
			if (file_schema == NULL) {
				print_message(hGraphicLib, "Internal error: file_schema == NULL");
				break;
			}
			display_list_with_goods(hGraphicLib, file_schema);
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
	} while (choice != GL_KEY_CANCEL && choice != 6);
}

void main_processing(T_GL_HGRAPHIC_LIB hGraphicLib, FILE_SCHEMA* file_schema) {
	create_menu(hGraphicLib, file_schema);
}
