#include "defs.h"
#include "log.h"


void _log(T_GL_HFILE fd, const char* file, int line, const char* func, const char* msg) {
    char log_entry[LOG_MSG_LENGTH_LIMIT];
    sprintf(log_entry, "[%s:%d] In function %s: %s\n", file, line, func, msg);
    GL_File_Write(fd, log_entry, sizeof(char) * strlen(log_entry));
}

ST_DSP_LINE line_prop = { {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line0
		                  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE, 100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} };

void print_message(T_GL_HGRAPHIC_LIB hGraphicLib, char* message) {
	int iRet;
	T_GL_HWIDGET screen = NULL;

	screen = GoalCreateScreen(hGraphicLib, &line_prop, 1, GL_ENCODING_UTF8);
	CHECK(screen != NULL, lblKO);                   // Create a new screen and clear it
	iRet = GoalClrScreen(screen, GL_COLOR_BLACK, 0, false);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalDspLine(screen, 0, message, &line_prop, 2*1000, true);
	goto lblEnd;

lblKO:                                             // None-classified low level error
	GL_Dialog_Message(hGraphicLib, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;

lblEnd:
	if (screen) {
		GoalDestroyScreen(&screen);
	}
}

void print_to_log(T_GL_HFILE fd, char* message) {
    GL_File_Write(fd, message, sizeof(char) * strlen(message));
}


void print_to_error_log(T_GL_HFILE fd, char* message) {
    GL_File_Write(fd, message, sizeof(char) * strlen(message));
}
