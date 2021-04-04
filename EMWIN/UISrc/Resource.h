#ifndef RESOURCE_H
#define RESOURCE_H
#include "sys.h"
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

//
// Fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font23S_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font23SL_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32S_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32SL_AA4;

//
// Images
//
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonOFF_110x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonON_110x40;

#endif // RESOURCE_H
