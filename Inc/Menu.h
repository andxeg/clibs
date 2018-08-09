/**
 * Menu.h
 *
 * Header for the Menu source file.
 *
 */
#ifndef __MENU_H
#define __MENU_H

#include <GL_GraphicLib.h>
#include "file_schema.h"

/*+************* #DEFINES **************************************************+*/

/*+************* CONSTANTS *************************************************+*/

/*+************* STRUCTURES ************************************************+*/

/*+************* VARIABLES *************************************************+*/
extern T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance;
extern FILE_SCHEMA* file_schema;
extern T_GL_HFILE error_log_file;
extern T_GL_HFILE log_file;

/*+************* FUNCTION PROTOTYPES ***************************************+*/
extern void menuGoalDisplay(void);
extern void initUIContext(int);
extern void releaseUIContext(void);


#endif //__MENU_H
