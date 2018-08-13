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


// Sample of dialog multi choice

void GL_SampleDialogMultiChoice(T_GL_HGRAPHIC_LIB graphicLib) {

    const char * menu [] = {
        "Zero",
        "One",
        "Two",
        "Three",
        "Four",
        "Five",
        "Six",
        "Seven",
        "Height",
        "Nine",
        "Ten",
        "Eleven",
        "Twelve",
        "Exit",
        0 //<- DO NOT FORGET THIS TO STOP THE LIST
    };

    // Initial state of checkboxes
    bool checked [] = {
        true, //"Zero",
        false,//"One",
        false,//"Two",
        true, //"Three",
        true, //"Four",
        false,//"Five",
        true, //"Six",
        false,//"Seven",
        false,//"Height",
        false,//"Nine",
        false,//"Ten",
        true, //"Eleven",
        true, //"Twelve",
        false, // utf8
    };
    ulong result;
    do {
		// Display the multi choice and wait selection
		result = GL_Dialog_MultiChoice (graphicLib, "Menu",
				menu, result, checked, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		(void)result;
		if (result == 13) break;
		checked[result] = (checked[result] == true) ? false : true;
    } while (result != 13);
}




/**
 * Main menu
 *
 */
void menuGoalDisplay(void)
{
	int i;
	char choice = 0;

	const char *MenuUser[] = {
			"Simple cash register",
			"Exit",
			0
	};

	initUIContext(_OFF_);

  do
  {
    //Display the menu using GOAL
    choice = GL_Dialog_Menu(gGoalGraphicLibInstance, "Select action", MenuUser, choice,
            GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

    if (choice == 0) {
    	// Test task main processing
    	if (file_schema == NULL) {
    		file_schema = create_file_schema();
    	}
    	main_processing(gGoalGraphicLibInstance, file_schema);
    } else {
    	break;
    }

  } while (choice != 1);

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


