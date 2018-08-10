/**
 * Menu.c
 *
 * This file manage the menu.
 *
 */

#include <sdk_tplus.h>
#include "Goal.h"

#include "log.h"
#include "Menu.h"
#include "lists.h"
#include "iofile.h"
#include "file_schema.h"
#include "dynamic_arrays.h"
#include "good_field_types.h"
#include "dynamic_array_types.h"
#include "main_processing.h"

/*+************* #DEFINES **************************************************+*/

/*+************* CONSTANTS *************************************************+*/

/*+************* STRUCTURES ************************************************+*/


/*+************* VARIABLES *************************************************+*/
static Telium_File_t *sKeyboard = NULL;
static Telium_File_t *sDisplay = NULL;

// Disable header, footer, led in order to Maximize canvas size
int sSavedStatusHeader, sSavedStatusLeds, sSavedStatusFooter;


static const ST_DSP_LINE txUms[] =
{
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line0
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line1
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line2
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} }
};

static const ST_DSP_LINE txFile[] =
{
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

/*+************* FUNCTION PROTOTYPES ***************************************+*/

#define ID_LABEL_1_1 0
/**
 * Callback function for custom window
 *
 */
static bool OnKeyPress(T_GL_HMESSAGE hMessage) {
	T_GL_WCHAR ulKey;
	T_GL_HWIDGET hLabel;
	ulKey = GL_Message_GetKey(hMessage);
	hLabel = GL_Message_GetWidgetById(hMessage, ID_LABEL_1_1);
	if (ulKey == GL_KEY_CANCEL) {
		GL_Message_SetResult(hMessage, ID_LABEL_1_1);
	}

	if (ulKey == GL_KEY_1) {
		if (GL_Widget_GetBackColor(hLabel) == GL_COLOR_GREEN) {
			GL_Widget_SetBackColor(hLabel, GL_COLOR_ORANGE);
		} else {
			GL_Widget_SetBackColor(hLabel, GL_COLOR_GREEN);
		}
	}

	return true;
}

/**
 * Callback function for custom window
 *
 */

static bool OnTimeout(T_GL_HMESSAGE hMessage) {
	GL_Message_SetResult(hMessage, GL_RESULT_INACTIVITY);
	return true;
}

/**
 * Custom window
 * Work with display
 *
 */

void GL_SampleLayout(T_GL_HGRAPHIC_LIB hGraphicLib) {
	T_GL_HWIDGET hWindow, hLayout, hLabel, hTimer;
	T_GL_DIM usRow, usColumn;
	ulong ulResult;

	hWindow = GL_Window_Create(hGraphicLib);
	hLayout = GL_Layout_Create(hWindow);
	for (usRow = 0; usRow < 3; usRow++) {
		for (usColumn = 0; usColumn < 3; usColumn++) {
			hLabel = GL_Label_Create(hLayout);
			GL_Widget_SetItem(hLabel, usColumn, usRow);
			GL_Widget_SetText(hLabel, "%d:%d", usColumn, usRow);
			switch (usRow) {
			case 0:
				GL_Widget_SetBackColor(hLabel, GL_COLOR_RED);
			    break;
			case 1:
				GL_Widget_SetBackColor(hLabel, GL_COLOR_GREEN);
				if (usColumn == 1) {
					GL_Widget_SetId(hLabel, ID_LABEL_1_1);
				}
				break;
			case 2:
				GL_Widget_SetBackColor(hLabel, GL_COLOR_BLUE);
				break;
			default: break;
			}
		}
	}

	GL_Widget_RegisterCallback(hWindow, GL_EVENT_KEY_DOWN, OnKeyPress);

	hTimer= GL_Timer_Create(hWindow);
	GL_Timer_SetInterval(hTimer, 30 * 1000);
	GL_Widget_RegisterCallback(hTimer, GL_EVENT_TIMER_OUT, OnTimeout);

	ulResult = GL_Window_MainLoop(hWindow);

	GL_Widget_Destroy(hWindow);
}

/**
 * Document sample
 * Work with printer
 *
 */
void GL_SampleDocument(T_GL_HGRAPHIC_LIB hGraphicLib) {
	T_GL_HWIDGET hDocument, hLayout, hPrint;
	T_GL_DIM usLine = 0;

	hDocument = GL_Document_Create(hGraphicLib);
	hLayout = GL_Layout_Create(hDocument);

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBorders(hPrint, 4, 4, 4, 4, GL_UNIT_PIXEL, GL_COLOR_BLACK);
	GL_Widget_SetText(hPrint, "BORDER");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetMargins(hPrint, 0, 10, 0, 10, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(hPrint, GL_COLOR_WHITE);
	GL_Widget_SetBackColor(hPrint, GL_COLOR_BLACK);
	GL_Widget_SetText(hPrint, "REVERSE");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_LEFT);
	GL_Widget_SetText(hPrint, "LEFT");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_XXLARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
	GL_Widget_SetText(hPrint, "RIGHT");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_LARGE);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
	GL_Widget_SetText(hPrint, "INGENICO");

	hPrint = GL_Print_Create(hLayout);
	GL_Widget_SetFontScale(hPrint, GL_SCALE_MEDIUM);
	GL_Widget_SetItem(hPrint, 0, usLine++);
	GL_Widget_SetBackAlign(hPrint, GL_ALIGN_RIGHT);
	GL_Widget_SetText(hPrint, "GROUP\n\n\n");

	GL_Document_Print(hDocument, 10 * 1000);

	GL_Widget_Destroy(hDocument);
}

/**
 * Peripheral synchronization sample
 *
 */

static bool OnKeyPress2(T_GL_HMESSAGE hMessage) {
	T_GL_WCHAR ulKey;
	ulKey = GL_Message_GetKey(hMessage);
	GL_Message_SetResult(hMessage, ulKey);
	return true;
}

void SampleEvents(T_GL_HGRAPHIC_LIB hGraphicLib) {
	Telium_File_t* hKbd = NULL;
	Telium_File_t* hMdm = NULL;
	T_GL_HWIDGET hWindow = NULL;
	char tcBuf[50];
	ulong ulKey;
	int iLen, iRet;

	hKbd = Telium_Fopen("KEYBOARD", "r*");
	if (hKbd == NULL)
		goto lblKO;
	hMdm = Telium_Fopen("MODEM", "rw*");
	if (hMdm == NULL)
		goto lblKO;
	hWindow = GL_Window_Create(hGraphicLib);
	GL_Widget_RegisterCallback(hWindow, GL_EVENT_KEY_DOWN, OnKeyPress2);
	Telium_Reset_buf(hKbd, _receive_id);
	Telium_Reset_buf(hMdm, _receive_id);
	Telium_Mask_event(hMdm, COM_REC_NOT_EMPTY);
	Telium_Fprintf(hMdm, "ATDT, T00475810363\r");
	iRet = Telium_Ttestall(MODEM | KEYBOARD, 10 * 100);

	if (MODEM & iRet) {
		iLen = Telium_Fread(tcBuf, 1, 3, hMdm);
		if (iLen != 3)
			goto lblKO;
	}

	if (KEYBOARD & iRet) {
		ulKey = GL_Window_MainLoop(hWindow);
	}

	if (iRet == 0) {
		/* case of timeout */
	}

lblKO:
		/* close here keyboard/modem and destroy window*/
	return;
}

/*
 * Sample dialog
 *
 */
void GL_SampleDialogText(T_GL_HGRAPHIC_LIB graphicLib) {
    char text[100] = "330412345678";
    ulong result;
    // Example of entering a text
    result = GL_Dialog_Text (graphicLib, "Text input", "Goods info :","/d/d/|/d/d/|/d/d/|/d/d/|/d/d/|/d/d/|/d/d", text, sizeof(text), GL_TIME_INFINITE);
    (void)result; // <- Just here to avoid a compilation warning
	int x = 10;   // time value
	T_GL_HWIDGET GM_Tab_Gw;
	T_GL_HWIDGET label;
	GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
	label     = GL_Label_Create(GM_Tab_Gw);
	GL_Widget_SetText(label, text);
	GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
	GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
	GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
	GL_Widget_Destroy(GM_Tab_Gw);
}


/*
 * Sample of dialog virtual keyboard
 *
 */
void GL_SampleDialogVirtualKeyboard1(T_GL_HGRAPHIC_LIB graphicLib) {
    char text[100] = "330412345678";
    ulong result;
    // Example of entering a text by defining a mask user
    result = GL_Dialog_VirtualKeyboard (graphicLib, 0, "Enter your mail : ","/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u|0123456789abcdefghijklmnopqrstuvwxyz@_.", text, sizeof(text), GL_TIME_INFINITE);
    (void)result; // <- Just here to avoid a compilation warning
    int x = 5;   // time value
	T_GL_HWIDGET GM_Tab_Gw;
	T_GL_HWIDGET label;
	GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
	label     = GL_Label_Create(GM_Tab_Gw);
	GL_Widget_SetText(label, text);
	GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
	GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
	GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
	GL_Widget_Destroy(GM_Tab_Gw);
}

/*
 * Sample of dialog virtual keyboard
 *
 */
void GL_SampleDialogVirtualKeyboard2(T_GL_HGRAPHIC_LIB graphicLib) {
    char text[100] = "330412345678";
    ulong result;
    // Example of entering a text
    result = GL_Dialog_VirtualKeyboard (graphicLib, "Virtual keyboard input", "Telephone number :","(/d/d)/d/d./d/d./d/d./d/d./d/d", text, sizeof(text), GL_TIME_INFINITE);
    (void)result; // <- Just here to avoid a compilation warning
    int x = 5;   // time value
	T_GL_HWIDGET GM_Tab_Gw;
	T_GL_HWIDGET label;
	GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
	label     = GL_Label_Create(GM_Tab_Gw);
	GL_Widget_SetText(label, text);
	GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
	GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
	GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
	GL_Widget_Destroy(GM_Tab_Gw);
}

/*
 * Sample of dialog virtual keyboard
 *
 */

void GL_SampleDialogVirtualKeyboard3(T_GL_HGRAPHIC_LIB graphicLib) {
    char text[100] = "330412345678";
    ulong result;
    // Example of entering a text
    result = GL_Dialog_VirtualKeyboard (graphicLib, "Virtual keyboard input", "Text edition : ",0, text, sizeof(text), GL_TIME_INFINITE);
    (void)result; // <- Just here to avoid a compilation warning
    int x = 5;   // time value
	T_GL_HWIDGET GM_Tab_Gw;
	T_GL_HWIDGET label;
	GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
	label     = GL_Label_Create(GM_Tab_Gw);
	GL_Widget_SetText(label, text);
	GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
	GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
	GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
	GL_Widget_Destroy(GM_Tab_Gw);
}



/**
 * Main menu
 *
 */
void menuGoalDisplay(void)
{
	int i;
	char choice = 0;

	const char *MenuUser[] =
		{ "Read file", "Show sample window",
		  "Show sample document", "Peripheral Synchronization test",
		  "Test text dialog", "Test virtual keyboard",
		  "Test dynamic array",
		  "Test list of dynamic array",
		  "GL Dialog Menu",
		  "Test file size",
		  "Magnetic",
		  "[TEST TASK MAIN MENU]",
		  "Exit", 0 };

	initUIContext(_OFF_);

  do
  {
    //Display the menu using GOAL
    choice = GL_Dialog_Menu(gGoalGraphicLibInstance, "Select action", MenuUser, choice,
            GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

    switch (choice)
    {
    case 0:
		// Displays a GOAL dialog during 2s
		GL_Dialog_Message(gGoalGraphicLibInstance, "Read file",
				"Read file", GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);

		char filename[256];
		T_GL_HFILE file;

		if (GL_Dialog_File(gGoalGraphicLibInstance, "Select file", "dir://flash/HOST", "*.TXT",  filename, sizeof(filename), 0, GL_TIME_INFINITE) == GL_KEY_VALID) {
			file = GL_File_Open(filename, GL_FILE_OPEN_EXISTING, GL_FILE_ACCESS_READ);
			if (file == NULL) {
				char buffer[100];
				sprintf(buffer, "cannot open '%s'", filename);
				int x = 5;   // time value
				T_GL_HWIDGET GM_Tab_Gw;
				T_GL_HWIDGET label;
				GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
				label     = GL_Label_Create(GM_Tab_Gw);
				GL_Widget_SetText(label, buffer);
				GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
				GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
				GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
				GL_Widget_Destroy(GM_Tab_Gw);
				break;
			}

			char buffer[10];
			// read symbols
			unsigned long count;
//			count = GL_File_Read(file, buffer, sizeof(buffer));

			int c;
			int read_bytes = 0;
			while (1) {
				count = GL_File_Read(file, &c, sizeof(char));
				if (count == 0) {
					print_message(gGoalGraphicLibInstance, "read 0 bytes");
					break;
				}
				if (c == '\n') {
					print_message(gGoalGraphicLibInstance, "read end of line");
				} else if (c == EOF) {
					print_message(gGoalGraphicLibInstance, "read end of file");
					break;
				} else if (c == '\r') {
					print_message(gGoalGraphicLibInstance, "read carriage return");
				} else {
					char msg[30];
					sprintf(msg, "read symbol -> %c", c);
					print_message(gGoalGraphicLibInstance, msg);
				}
			}

//			if (count > 0) {
//				buffer[count] = '\0';
//				int x = 5;   // time value
//				T_GL_HWIDGET GM_Tab_Gw;
//				T_GL_HWIDGET label;
//				GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
//				label     = GL_Label_Create(GM_Tab_Gw);
//				GL_Widget_SetText(label, buffer);
//				GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
//				GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
//				GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
//				GL_Widget_Destroy(GM_Tab_Gw);
//			}
			GL_File_Close(file);

			SLOG("read file");
        }
		break;
    case 1:
		// Displays a GOAL dialog during 2s
		GL_Dialog_Message(gGoalGraphicLibInstance, "Show sample window",
		  "Sample window...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
		  2 * GL_TIME_SECOND);
		GL_SampleLayout(gGoalGraphicLibInstance);
		break;
    case 2:
		// Displays a GOAL dialog during 2s
		GL_Dialog_Message(gGoalGraphicLibInstance, "Show sample document",
		  "Sample document...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
		  2 * GL_TIME_SECOND);
		GL_SampleDocument(gGoalGraphicLibInstance);
		break;
    case 3:
		// Displays a GOAL dialog during 2s
		GL_Dialog_Message(gGoalGraphicLibInstance, "Peripheral Synchronization test",
		  "Peripheral Synchronization test...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
		  2 * GL_TIME_SECOND);
		SampleEvents(gGoalGraphicLibInstance);
		break;
    case 4:
		// Displays a GOAL dialog during 2s
		GL_Dialog_Message(gGoalGraphicLibInstance, "Test text dialog",
		  "Test text dialog...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
		  2 * GL_TIME_SECOND);
		GL_SampleDialogText(gGoalGraphicLibInstance);
		break;
    case 5:
		 // Displays a GOAL dialog during 2s
		 GL_Dialog_Message(gGoalGraphicLibInstance, "Test virtual keyboard",
				 "Test virtual keyboard...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
				 2 * GL_TIME_SECOND);
		 GL_SampleDialogVirtualKeyboard1(gGoalGraphicLibInstance);
		 GL_SampleDialogVirtualKeyboard2(gGoalGraphicLibInstance);
		 GL_SampleDialogVirtualKeyboard3(gGoalGraphicLibInstance);
		 break;
    case 6:
    	GL_Dialog_Message(gGoalGraphicLibInstance, "Test dynamic array",
    			"Test dynamic array...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);

		// Open a file for writing to HOST disk
//    	char* output_file = "file://flash/HOST/RESULT.TXT";
//		file = GL_File_Open(output_file, GL_FILE_OPEN_ALWAYS, GL_FILE_ACCESS_WRITE);
//		if (file) {
//			GL_File_Write(file, "Hello", 5);
//			GL_File_Close(file);
//		}
//		break;

    	TEMPLATE(DYN_ARRAY, int) array_int;
    	TEMPLATE(append, int)(&array_int, 1);
    	TEMPLATE(append, int)(&array_int, 2);
    	TEMPLATE(append, int)(&array_int, 3);
    	TEMPLATE(append, int)(&array_int, 4);

    	char* result = "file://flash/HOST/RESULTS.TXT";
		T_GL_HFILE output;
		output = GL_File_Open(result, GL_FILE_CREATE_ALWAYS, GL_FILE_ACCESS_WRITE);
		if (output == NULL) {
			break;
		}

		for (i = 0; i < array_int.length; i++) {
			int elem = array_int.data[i];
			if (GL_File_Write(output, &elem, sizeof(elem)) != sizeof(elem)) {
				print_message(gGoalGraphicLibInstance, "Error while write to file");
			}
		}

    	TEMPLATE(destroy, int)(&array_int);
    	GL_File_Close(output);
    	break;
    case 7:
    	GL_Dialog_Message(gGoalGraphicLibInstance, "Test list of dynamic array",
    			"Test list of dynamic array...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);

    	break;

    case 8:
    	GL_Dialog_Message(gGoalGraphicLibInstance, "GL Dialog Menu",
    	    			"GL Dialog Menu...", GL_ICON_INFORMATION, GL_BUTTON_VALID,
    					2 * GL_TIME_SECOND);
//		const char *SubMenu[] =
//		{	"First",
//			"Second",
//			"Third",
//			"Exit", 0 };

    	int count = 8;
		char** SubMenu = (char** ) malloc(count * sizeof(char*));
		SubMenu[0] = "First\0";
		SubMenu[1] = "Second\0";
		SubMenu[2] = "Third\0";
		SubMenu[3] = "Exit\0";
		SubMenu[4] = "Print elements\0";
		SubMenu[5] = "Add new element\0";
		SubMenu[6] = "Delete element\0";
		SubMenu[7] = 0;

		int elem_number = 2;
		char** Elements = (char** ) malloc(elem_number * sizeof(char*));

		char* new_elem = (char* ) malloc(sizeof(char) * (strlen("element1") + 1));
		memcpy(new_elem, "element1\0", strlen("element1") + 1);
		Elements[0] = new_elem;

		new_elem = (char* )malloc(sizeof(char) * (strlen("element2") + 1));
		memcpy(new_elem, "element2\0", strlen("element2") + 1);
		Elements[1] = new_elem;

		char subchoice = 0;
		do {

			//Display the menu using GOAL
			subchoice = GL_Dialog_Menu(gGoalGraphicLibInstance, "Select item", SubMenu, subchoice,
					GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

			if (subchoice >= 0 && subchoice <= (count - 3)) {
				GL_Dialog_Message(gGoalGraphicLibInstance, SubMenu[subchoice],
						SubMenu[subchoice], GL_ICON_INFORMATION, GL_BUTTON_VALID,
						2 * GL_TIME_SECOND);
			}

			if (subchoice == 4) {
				int i;
				for (i = 0; i < elem_number; i++) {
					int x = 4;
					T_GL_HWIDGET GM_Tab_Gw;
					T_GL_HWIDGET label;
					GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
					label     = GL_Label_Create(GM_Tab_Gw);
					GL_Widget_SetText(label, Elements[i]);
					GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
					GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
					GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
					GL_Widget_Destroy(GM_Tab_Gw);
				}
			} else if (subchoice == 5) {
				char element[100] = "element\0";
				ulong result;
				// Example of entering a text by defining a mask user
				result = GL_Dialog_VirtualKeyboard (gGoalGraphicLibInstance, 0, "Enter your mail : ","/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u/u|0123456789abcdefghijklmnopqrstuvwxyz@_.", element, sizeof(element), GL_TIME_INFINITE);
				(void)result; // <- Just here to avoid a compilation warning
				int x = 2;   // time value
				T_GL_HWIDGET GM_Tab_Gw;
				T_GL_HWIDGET label;
				GM_Tab_Gw = GL_Window_Create(gGoalGraphicLibInstance);
				label     = GL_Label_Create(GM_Tab_Gw);
				GL_Widget_SetText(label, element);
				GL_Widget_SetBackAlign(label, GL_ALIGN_CENTER);
				GL_Widget_SetFontScale(label, GL_SCALE_SMALL);
				GL_Window_Dispatch(GM_Tab_Gw, x * 1000); // display the message
				GL_Widget_Destroy(GM_Tab_Gw);

				++elem_number;
				new_elem = (char* )malloc(sizeof(char) * (strlen(element) + 1));
				memcpy(new_elem, element, strlen(element) + 1);
				char** p = (char** ) realloc(Elements, elem_number * sizeof(char*));
				if (p != NULL) {
					Elements = p;
					Elements[elem_number - 1] = new_elem;
				}
			} else if (subchoice == 6) {

			}

		} while (subchoice != 3);

		free(SubMenu);
		for (i = 0; i < elem_number; i++) {
			free(Elements[i]);
		}
		free(Elements);
    	break;

    case 9:

    	GL_Dialog_Message(gGoalGraphicLibInstance, "Test file size",
    					"Test file size", GL_ICON_INFORMATION, GL_BUTTON_VALID,
    					2 * GL_TIME_SECOND);
    	// Local variables
        // ***************
    	T_GL_HWIDGET hScreen = NULL;
    	T_GL_HFILE hFile;
    	char tcData[512];
        int iLenData;
    	int iLen, iRet;

    	// Wait for USB key or MMC card
    	// ****************************
    	hScreen = GoalCreateScreen(gGoalGraphicLibInstance, txUms, NUMBER_OF_LINES(txUms), GL_ENCODING_UTF8);
    	CHECK(hScreen!=NULL, lblKO);                   // Create screen and clear it
    	iRet = GoalClrScreen(hScreen, GL_COLOR_BLACK, KEY_CANCEL, true);
    	CHECK(iRet>=0, lblKO);

    	// Retrieve data from file VFS.TXT located in HOST disk
    	// ****************************************************
    	hFile = GL_File_Open("file://flash/HOST/GOODS.TXT", GL_FILE_OPEN_EXISTING, GL_FILE_ACCESS_READ);
    	CHECK(hFile!=NULL, lblFileMissing);
    	iLenData = GL_File_GetSize(hFile);             // File length in bytes
        memset (tcData, 0, sizeof(tcData));
        iRet = GL_File_Read(hFile, tcData, iLenData);  // Read data from file
        CHECK(iRet==iLenData, lblHostKO);
        iRet = GL_File_Close(hFile);                   // Close file
    	CHECK(iRet==GL_SUCCESS, lblHostKO);

    	iRet = GoalDspLine(hScreen, 0, tcData, &txFile[1], 5*1000, true);

    	GoalDestroyScreen(&hScreen);                   // Destroy screen

        // Copy data into file VFS.TXT located in USB/MMC disk
        // ***************************************************
    	hScreen = GoalCreateScreen(gGoalGraphicLibInstance, txFile, NUMBER_OF_LINES(txFile), GL_ENCODING_UTF8);
    	CHECK(hScreen!=NULL, lblKO);                   // Create a new screen and clear it
    	iRet = GoalClrScreen(hScreen, GL_COLOR_BLACK, 0, false);
    	CHECK(iRet>=0, lblKO);
    	iRet = GoalDspLine(hScreen, 0, "1-Open File", &txFile[1], 500, true);
    	CHECK(iRet>=0, lblKO);                         //

    	iRet = GoalDspLine(hScreen, 1, "2-Write File", &txFile[2], 500, true);
    	CHECK(iRet>=0, lblKO);

    	iRet = GoalDspLine(hScreen, 2, "3-Close File", &txFile[3], 10*100, true);
        goto lblEnd;

    	// Errors treatment
        // ****************
    lblKO:                                             // None-classified low level error
    	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
        goto lblEnd;
    lblHostKO:                                         // HOST disk failed
    	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "HOST Disk Failed", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
        goto lblEnd;
    lblFileMissing:                                    // File not found
    	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "File VFS.TXT\nMissing from Host", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
    	goto lblEnd;
    lblEnd:

    	if (hScreen)
    		GoalDestroyScreen(&hScreen);              // Destroy screen
    	break;
    case 10:
    	Magnetic();
    	break;
    case 11:
    	// Test task main processing
    	GL_Dialog_Message(gGoalGraphicLibInstance, "TEST TASK MAIN MENU",
    			"Hello in test task main menu", GL_ICON_INFORMATION, GL_BUTTON_VALID,
				2 * GL_TIME_SECOND);
    	if (file_schema == NULL) {
    		file_schema = create_file_schema();
    	}
    	main_processing(gGoalGraphicLibInstance, file_schema);
    	break;
    default:
		//exit
		break;
    }
  } while (choice != 12);

  releaseUIContext();
  return;
}


/**
 * Initialize the display context to maximize the canvas on user area
 */
void initUIContext(int displayHeaderFooterLeds)
{
    if(displayHeaderFooterLeds != _OFF_ && displayHeaderFooterLeds != _ON_) {
		displayHeaderFooterLeds = _OFF_;
	}
	sSavedStatusHeader=DisplayHeader(displayHeaderFooterLeds);
	sSavedStatusLeds=DisplayLeds(displayHeaderFooterLeds);
	sSavedStatusFooter=DisplayFooter(displayHeaderFooterLeds);

    // Open display
    sDisplay = Telium_Fopen("DISPLAY", "w*");
    // Open keyboard
    sKeyboard = Telium_Fopen("KEYBOARD", "r*");

    return;
}

/**
 * Restore display context
 */
void releaseUIContext(void)
{
    // close keyboard
    if (sKeyboard)
    {
        Telium_Fclose(sKeyboard);
        sKeyboard = NULL;
    }
    // close display
    if (sDisplay)
    {
        Telium_Fclose(sDisplay);
        sDisplay = NULL;
    }

	// Restore display context
	DisplayHeader(sSavedStatusHeader);
	DisplayLeds(sSavedStatusLeds);
	DisplayFooter(sSavedStatusFooter);

	return;
}


