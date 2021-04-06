#include "DIALOG.h"
#include "LCDConf.h"
#include "string.h"
#include "MESSAGEBOX.h"

// User Headers
#include "Magic.h"
#include "Keypad.h"
#include "Resource.h"

//Defines
#define X_SIZE 1024
#define Y_SIZE 600

#define ID_WINDOW_0             (GUI_ID_USER + 0x00)

#define ID_BUTTON_CH1           (GUI_ID_USER + 0x01)
#define ID_BUTTON_CH2           (GUI_ID_USER + 0x02)
#define ID_BUTTON_CH3           (GUI_ID_USER + 0x03)

#define ID_BUTTON_SETTINGS      (GUI_ID_USER + 0x04)
#define ID_GRAPH              (GUI_ID_USER + 0x05)

#define ID_BUTTON_ONOFF         (GUI_ID_USER + 0x06)
#define ID_BUTTON_SWITCHMODE    (GUI_ID_USER + 0x07)

#define ID_TEXT_TITLE           (GUI_ID_USER + 0x09)
#define ID_TEXT_CURRENTUNIT     (GUI_ID_USER + 0x0A)
#define ID_TEXT_FREQUENCYUNIT   (GUI_ID_USER + 0x0B)
#define ID_TEXT_DUTYUNIT          (GUI_ID_USER + 0x0D)
#define ID_EDIT_CURRENT          (GUI_ID_USER + 0x08)
#define ID_EDIT_FREQUENCY          (GUI_ID_USER + 0x0E)
#define ID_EDIT_DUTY          (GUI_ID_USER + 0x0F)
#define ID_TEXT_CURRENTLABEL (GUI_ID_USER + 0x1E)
#define ID_TEXT_FREQUENCYLABEL (GUI_ID_USER + 0x1F)
#define ID_TEXT_DUTYLABEL (GUI_ID_USER + 0x20)

#define ID_BUTTON_KEY0       (GUI_ID_USER + 0x30)
#define ID_BUTTON_KEY1       (GUI_ID_USER + 0x31)
#define ID_BUTTON_KEY2       (GUI_ID_USER + 0x32)
#define ID_BUTTON_KEY3       (GUI_ID_USER + 0x33)
#define ID_BUTTON_KEY4       (GUI_ID_USER + 0x34)
#define ID_BUTTON_KEY5       (GUI_ID_USER + 0x35)
#define ID_BUTTON_KEY6       (GUI_ID_USER + 0x36)
#define ID_BUTTON_KEY7        (GUI_ID_USER + 0x37)
#define ID_BUTTON_KEY8        (GUI_ID_USER + 0x38)
#define ID_BUTTON_KEY9        (GUI_ID_USER + 0x39)
#define ID_BUTTON_KEYBACK        (GUI_ID_USER + 0x3A)
#define ID_BUTTON_KEYENTER       (GUI_ID_USER + 0x3B)
#define ID_BUTTON_KEYDOT       (GUI_ID_USER + 0x3C)
#define ID_BUTTON_KEYCLR       (GUI_ID_USER + 0x3D)

#define CHANNEL_NUMBERS 3

// Colors
#define DARK_BLUE GUI_MAKE_COLOR(0x613600)
#define LIGHT_BLUE GUI_MAKE_COLOR(0xaa7d67)
#define BLUE GUI_MAKE_COLOR(0x855a41)
#define LEMON GUI_MAKE_COLOR(0x00d6d3)
#define PERFECT_BLUE GUI_MAKE_COLOR(0x0084ff)

static void _DrawGraph();

typedef struct
{
	char CurrentStr[20];
	//float CurrentData;
	char CurrentUnit[8];
	int CurrentHasDot;
	int AfterCurrentDotCount;

	char FrequenctStr[20];
	//float FrequencyData;
	char FrequencyUnit[8];
	int FrequencyHasDot;
	int AfterFrequencyDotCount;

	char DutyStr[20];
	//int DutyData;
} EDIT_DATA;


typedef struct
{
	int ID;              // Id of button
	char aName[20];      // Name to be displayed

	EDIT_DATA ChEditData;

	int OnOff;
	int Mode;
} CHANNEL_DATA;

static CHANNEL_DATA _aChannels[CHANNEL_NUMBERS] = {
	{ID_BUTTON_CH1, "Ch 1",  {"1", "A", 0, 0,   "1000", "Hz", 0, 0,   "50"}, 0, 0}, //Initial Focused
	{ID_BUTTON_CH2, "Ch 2",  {"1", "A", 0, 0,   "1000", "Hz", 0, 0,   "50"}, 0, 0},
	{ID_BUTTON_CH3, "Ch 3",  {"1", "A", 0, 0,   "1000", "Hz", 0, 0,   "50"}, 0, 0}
};

static int nowChannelIndex = 0;


typedef struct
{
	int ID;
} SWITCH_DATA;

static SWITCH_DATA _aSwitchButtons[] = {
	{ID_BUTTON_ONOFF},
	{ID_BUTTON_SWITCHMODE}
};

static int _aEdits[] = {
	ID_EDIT_CURRENT,
	ID_EDIT_FREQUENCY,
	ID_EDIT_DUTY,
};

static int nowEditFocus = -1;

static WM_HWIN _hWin;

typedef struct {
	int dataxsize, dataysize;
	GUI_COLOR linecolor, backcolor;
	GRAPH_DATA_Handle graphdata;
	int div;
	GRAPH_SCALE_Handle hScaleV;
	GRAPH_SCALE_Handle hScaleH;
} GRAPH_USER_DATA;

static GRAPH_USER_DATA _aGraph;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GUI_CONST_STORAGE GUI_FONT* pFont23pBold = &GUI_Font23S_AA4;
static GUI_CONST_STORAGE GUI_FONT* pFont32pBold = &GUI_Font32S_AA4;
static GUI_CONST_STORAGE GUI_FONT* pFont23p = &GUI_Font23SL_AA4;
static GUI_CONST_STORAGE GUI_FONT* pFont32p = &GUI_Font32SL_AA4;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, X_SIZE, Y_SIZE, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { BUTTON_CreateIndirect, "Ch 1", ID_BUTTON_CH1, 0, Y_SIZE * 1 / 12, X_SIZE * 1 / 16, Y_SIZE * 1 / 4, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Ch 2", ID_BUTTON_CH2, 0, Y_SIZE * 1 / 3, X_SIZE * 1 / 16,Y_SIZE * 1 / 4, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Ch 3", ID_BUTTON_CH3, 0, Y_SIZE * 7 / 12  ,X_SIZE * 1 / 16  ,Y_SIZE * 1 / 4  , 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Settings", ID_BUTTON_SETTINGS, 0, Y_SIZE * 5 / 6, X_SIZE * 1 / 16  ,Y_SIZE * 1 / 6  , 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH, X_SIZE * 1 / 16, 50, X_SIZE * 8 / 16 ,Y_SIZE * 3 / 4  , 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OnOff", ID_BUTTON_ONOFF, 250, 520, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SwitchModeButton", ID_BUTTON_SWITCHMODE, X_SIZE * 9 / 16  ,Y_SIZE * 1 / 12 ,X_SIZE * 7 / 16 , Y_SIZE * 1 / 12  , 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Title", ID_TEXT_TITLE, 0, 0, X_SIZE * 4 / 16  ,Y_SIZE * 1 / 12 , 0, 0x0, 0 },

  { TEXT_CreateIndirect, "CurrentUnit", ID_TEXT_CURRENTUNIT, X_SIZE * 7 / 8, Y_SIZE * 1 / 6, X_SIZE * 1 / 8, Y_SIZE * 1 / 12, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "FrequencyUnit", ID_TEXT_FREQUENCYUNIT, X_SIZE * 7 / 8, Y_SIZE * 1 / 4,  X_SIZE * 1 / 8, Y_SIZE * 1 / 12, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "DutyUnit", ID_TEXT_DUTYUNIT, X_SIZE * 7 / 8, Y_SIZE * 1 / 3, X_SIZE * 1 / 8, Y_SIZE * 1 / 12, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "CurrentEdit", ID_EDIT_CURRENT, X_SIZE * 21 / 32, Y_SIZE * 2 / 12 , X_SIZE * 7 / 32, Y_SIZE * 1 / 12, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "FrequencyEdit", ID_EDIT_FREQUENCY, X_SIZE * 21 / 32, Y_SIZE * 3 / 12 , X_SIZE * 7 / 32, Y_SIZE * 1 / 12, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "DutyEdit", ID_EDIT_DUTY, X_SIZE * 21 / 32, Y_SIZE * 4 / 12 , X_SIZE * 7 / 32, Y_SIZE * 1 / 12, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "CurrentLabel", ID_TEXT_CURRENTLABEL, X_SIZE * 9 / 16, Y_SIZE * 1 / 6, X_SIZE * 3 / 32, Y_SIZE * 1 / 12, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "FrequencyLabel", ID_TEXT_FREQUENCYLABEL, X_SIZE * 9 / 16, Y_SIZE * 1 / 4,  X_SIZE * 3 / 32, Y_SIZE * 1 / 12, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "DutyLabel", ID_TEXT_DUTYLABEL, X_SIZE * 9 / 16, Y_SIZE * 1 / 3, X_SIZE * 3 / 32, Y_SIZE * 1 / 12, 0, 0x0, 0 },

  //KeyBoard
  { BUTTON_CreateIndirect, "7", ID_BUTTON_KEY7, 650, 280, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_KEY8, 670 + 50, 280, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_KEY9, 740 + 50, 280, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<x|", ID_BUTTON_KEYBACK, 810 + 50, 280, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_KEY4, 600 + 50, 350, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_KEY5, 670 + 50, 350, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_KEY6, 740 + 50, 350, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "C", ID_BUTTON_KEYCLR, 810 + 50, 350, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Enter", ID_BUTTON_KEYENTER, 810 + 50, 420, 70, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_KEY1, 600 + 50, 420, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_KEY2, 670 + 50, 420, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_KEY3, 740 + 50, 420, 70, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "0", ID_BUTTON_KEY0, 600 + 50, 490, 140, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ".", ID_BUTTON_KEYDOT, 740 + 50, 490, 70, 70, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void _cbButtonChannel(WM_MESSAGE* pMsg)
{
	GUI_RECT Rect;
	GUI_COLOR ColorFrame;
	GUI_COLOR Color;
	GUI_COLOR ColorText;
	GUI_CONST_STORAGE GUI_FONT* pFont23;
	GUI_CONST_STORAGE GUI_FONT* pFont32;
	int ChannelIndex;

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		ChannelIndex = WM_GetId(pMsg->hWin) - ID_BUTTON_CH1;
		if (ChannelIndex == nowChannelIndex)
		{
			Color = LIGHT_BLUE;
			ColorText = LEMON;
			ColorFrame = GUI_WHITE;
			pFont23 = pFont23pBold;
			pFont32 = pFont32pBold;
		}
		else
		{
			Color = PERFECT_BLUE;
			ColorText = GUI_WHITE;
			ColorFrame = GUI_WHITE;
			pFont23 = pFont23p;
			pFont32 = pFont32p;
		}

		WM_GetClientRectEx(pMsg->hWin, &Rect);
		GUI_SetColor(ColorFrame);
		GUI_FillRectEx(&Rect);

		GUI_SetColor(Color);
		Rect.x0 += 1;
		Rect.x1 -= 1;
		Rect.y1 -= 1;
		GUI_AA_FillRoundedRectEx(&Rect, 3);

		GUI_SetFont(pFont23);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetColor(ColorText);
		Rect.x0 += 10;
		GUI_DispStringInRect((char*)_aChannels[ChannelIndex].aName, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);

		break;

	default:
		BUTTON_Callback(pMsg);
		break;
	}
}

/*
* _OnChannelRelease
*/
static void _OnChannelRelease(WM_MESSAGE* pMsg)
{
	int i;
	int Id = WM_GetId(pMsg->hWinSrc);
	WM_HWIN hItem;

	for (i = 0; i < CHANNEL_NUMBERS; i++)
	{
		if (_aChannels[i].ID != Id)
		{
			WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aChannels[i].ID));
		}
	}
	if (nowChannelIndex != Id - ID_BUTTON_CH1)
	{
		GRAPH_DATA_YT_Clear(_aGraph.graphdata);
	}
	nowChannelIndex = Id - ID_BUTTON_CH1;

	for (i = 0; i < 2; i++)
	{
		WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aSwitchButtons[i].ID));
	}

	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRENT);
	EDIT_SetText(hItem, _aChannels[nowChannelIndex].ChEditData.CurrentStr);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY);
	EDIT_SetText(hItem, _aChannels[nowChannelIndex].ChEditData.FrequenctStr);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY);
	EDIT_SetText(hItem, _aChannels[nowChannelIndex].ChEditData.DutyStr);

	if (_aChannels[nowChannelIndex].OnOff) {
		_DrawGraph();
	}
}

static void _cbButtonSwitchOnOff(WM_MESSAGE* pMsg)
{
	GUI_RECT Rect;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		WM_GetClientRectEx(pMsg->hWin, &Rect);
		if (_aChannels[nowChannelIndex].OnOff)
		{
			GUI_DrawBitmap(&bmButtonON_110x40, 0, 0);
		}
		else
		{
			GUI_DrawBitmap(&bmButtonOFF_110x40, 0, 0);
		}
		break;
	default:
		BUTTON_Callback(pMsg);
		break;
	}
}

static void _cbButtonSwitchMode(WM_MESSAGE* pMsg)
{
	GUI_RECT Rect;
	GUI_COLOR ColorFrame;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		WM_GetClientRectEx(pMsg->hWin, &Rect);
		GUI_SetFont(GUI_FONT_32B_ASCII);
		GUI_SetTextMode(GUI_TM_TRANS);

		ColorFrame = GUI_WHITE;
		GUI_SetColor(ColorFrame);
		GUI_FillRectEx(&Rect);


		if (_aChannels[nowChannelIndex].Mode)
		{ //Choose text color
			GUI_SetColor(LEMON);
			GUI_DispStringInRect("Const Pattern", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			WM_HideWin(WM_GetDialogItem(_hWin, ID_EDIT_DUTY));
			WM_HideWin(WM_GetDialogItem(_hWin, ID_TEXT_DUTYLABEL));
			WM_HideWin(WM_GetDialogItem(_hWin, ID_TEXT_DUTYUNIT));

			WM_HideWin(WM_GetDialogItem(_hWin, ID_EDIT_FREQUENCY));
			WM_HideWin(WM_GetDialogItem(_hWin, ID_TEXT_FREQUENCYLABEL));
			WM_HideWin(WM_GetDialogItem(_hWin, ID_TEXT_FREQUENCYUNIT));
		}
		else
		{
			GUI_SetColor(GUI_DARKGRAY);
			GUI_DispStringInRect("Pulse Pattern", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			WM_ShowWin(WM_GetDialogItem(_hWin, ID_EDIT_DUTY));
			WM_ShowWin(WM_GetDialogItem(_hWin, ID_TEXT_DUTYLABEL));
			WM_ShowWin(WM_GetDialogItem(_hWin, ID_TEXT_DUTYUNIT));

			WM_ShowWin(WM_GetDialogItem(_hWin, ID_EDIT_FREQUENCY));
			WM_ShowWin(WM_GetDialogItem(_hWin, ID_TEXT_FREQUENCYLABEL));
			WM_ShowWin(WM_GetDialogItem(_hWin, ID_TEXT_FREQUENCYUNIT));
		}

		break;
	default:
		BUTTON_Callback(pMsg);
		break;
	}
}

static void _OnSwitchRelease(WM_MESSAGE* pMsg)
{
	WM_HWIN hItem;

	hItem = pMsg->hWinSrc;
	switch (WM_GetId(hItem))
	{
	case ID_BUTTON_ONOFF:
		_aChannels[nowChannelIndex].OnOff = !_aChannels[nowChannelIndex].OnOff;
		WM_InvalidateWindow(hItem);
		if (_aChannels[nowChannelIndex].OnOff)
		{
			_DrawGraph();
		}
		else
		{
			GRAPH_DATA_YT_Clear(_aGraph.graphdata);
		}
		break;
	case ID_BUTTON_SWITCHMODE:
		_aChannels[nowChannelIndex].Mode = !_aChannels[nowChannelIndex].Mode;
		WM_InvalidateWindow(hItem);
		if (_aChannels[nowChannelIndex].Mode) {
			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY));
			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYLABEL));
			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYUNIT));

			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY));
			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYLABEL));
			WM_HideWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYUNIT));
		}
		else
		{
			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY));
			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYLABEL));
			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYUNIT));

			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY));
			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYLABEL));
			WM_ShowWin(WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYUNIT));
		}

		_aChannels[nowChannelIndex].OnOff = 0;
		GRAPH_DATA_YT_Clear(_aGraph.graphdata);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ONOFF);
		WM_InvalidateWindow(hItem);

		break;
	default:
		break;
	}
}

static void _Str2Float(const char* NumStr, float* num) {
	int i;
	int len;
	float circle;
	float replace_float;

	replace_float = 0;
	circle = 10;

	len = strlen(NumStr);
	for (i = 0; i < len; i++)
	{
		if (NumStr[i] == '.')
		{
			circle = 0.1;
			continue;
		}
		replace_float = replace_float * circle + NumStr[i] - '0';
	}

	*num = replace_float;
}

static int _CheckEditTextLogic(WM_HWIN hWin) {
	WM_HWIN hItem;
	float   Tempfloat;
	char    acTextSrc[20];

	hItem = WM_GetDialogItem(hWin, ID_EDIT_CURRENT);
	EDIT_GetText(hItem, acTextSrc, sizeof(acTextSrc));
	_Str2Float(acTextSrc, &Tempfloat);
	if (Tempfloat > 20)
	{
		return 0;
	}

	hItem = WM_GetDialogItem(hWin, ID_EDIT_FREQUENCY);
	EDIT_GetText(hItem, acTextSrc, sizeof(acTextSrc));
	_Str2Float(acTextSrc, &Tempfloat);
	if (Tempfloat > 10000)
	{
		return 0;
	}

	hItem = WM_GetDialogItem(hWin, ID_EDIT_DUTY);
	EDIT_GetText(hItem, acTextSrc, sizeof(acTextSrc));
	_Str2Float(acTextSrc, &Tempfloat);
	if (Tempfloat > 100)
	{
		return 0;
	}

	return 1;
}

static void _DrawGraph()
{
	WM_HWIN hItem;
	int dataxsize;
	int i, j;
	float duty;
	float cur;

	hItem = WM_GetDialogItem(_hWin, ID_GRAPH);
	dataxsize = WM_GetWindowSizeX(hItem) - 53;

	GRAPH_DATA_YT_Clear(_aGraph.graphdata);									//删除给定的数据对象

	_Str2Float(_aChannels[nowChannelIndex].ChEditData.CurrentStr, &cur);

	switch (_aChannels[nowChannelIndex].Mode)
	{
	case 0: //Pulse
		_Str2Float(_aChannels[nowChannelIndex].ChEditData.DutyStr, &duty);
		for (j = 0; j < 5; j++)
		{
			for (i = 0; i < duty; i++)
			{
				GRAPH_DATA_YT_AddValue(_aGraph.graphdata, 20 * cur);
			}
			for (i = 0; i < 100 - duty; i++)
			{
				GRAPH_DATA_YT_AddValue(_aGraph.graphdata, 0);
			}
		}
		break;

	case 1: //Const
		for (i = 0; i < 500; i++)
		{
			GRAPH_DATA_YT_AddValue(_aGraph.graphdata, 20 * cur);
		}
		break;
	}
}

static WM_HWIN _CreateMessageBox(const char* sMessage, const char* sCaption, int Flags) {
	WM_HWIN  hWin;
	WM_HWIN  hItem;
	GUI_RECT Rect;
	GUI_CONST_STORAGE GUI_FONT* pFont;

	pFont = pFont23p;

	hWin = MESSAGEBOX_Create(sMessage, sCaption, Flags);
	FRAMEWIN_SetFont(hWin, pFont);
	WM_GetWindowRectEx(hWin, &Rect);
	WM_SetWindowPos(hWin, Rect.x0 - 40,
		Rect.y0 - 40,
		Rect.x1 - Rect.x0 + 1 + 50,
		Rect.y1 - Rect.y0 + 1 + 50);

	hItem = WM_GetDialogItem(hWin, GUI_ID_OK);
	BUTTON_SetFont(hItem, pFont);

	WM_GetWindowRectEx(hItem, &Rect);
	WM_SetWindowPos(hItem, Rect.x0,
		Rect.y0 + 10,
		Rect.x1 - Rect.x0 + 1 + 30,
		Rect.y1 - Rect.y0 + 1 + 5);

	hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
	TEXT_SetFont(hItem, pFont);

	WM_GetWindowRectEx(hItem, &Rect);
	WM_SetWindowPos(hItem, Rect.x0,
		Rect.y0,
		Rect.x1 - Rect.x0 + 1 + 30,
		Rect.y1 - Rect.y0 + 1 + 12);
	return hWin;
}

static void _OnNumKeyRelease(WM_MESSAGE* pMsg) {
	WM_HWIN hItem;
	int     ID;
	int     Len;
	int     i;
	char    acTextSrc[20];
	char    c;

	ID = WM_GetId(pMsg->hWinSrc);
	if (nowEditFocus == -1 && ID != ID_BUTTON_KEYENTER) {
		return;
	}
	else {
		if (nowEditFocus != -1)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, _aEdits[nowEditFocus]);
			WM_SetFocus(hItem);
		}
		if (ID >= ID_BUTTON_KEY0 && ID <= ID_BUTTON_KEY9)
		{
			c = '0' + ID - ID_BUTTON_KEY0;
			switch (_aEdits[nowEditFocus])
			{
			case ID_EDIT_CURRENT:
				if (!_aChannels[nowChannelIndex].ChEditData.CurrentHasDot)
				{
					GUI_StoreKeyMsg(c, 1);
					GUI_StoreKeyMsg(c, 0);
				}
				else if (_aChannels[nowChannelIndex].ChEditData.CurrentHasDot && _aChannels[nowChannelIndex].ChEditData.AfterCurrentDotCount < 3)
				{
					GUI_StoreKeyMsg(c, 1);
					GUI_StoreKeyMsg(c, 0);
					_aChannels[nowChannelIndex].ChEditData.AfterCurrentDotCount++;
				}
				break;
			case ID_EDIT_FREQUENCY:
				if (!_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot)
				{
					GUI_StoreKeyMsg(c, 1);
					GUI_StoreKeyMsg(c, 0);
				}
				else if (_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot && _aChannels[nowChannelIndex].ChEditData.AfterFrequencyDotCount < 3)
				{
					GUI_StoreKeyMsg(c, 1);
					GUI_StoreKeyMsg(c, 0);
					_aChannels[nowChannelIndex].ChEditData.AfterFrequencyDotCount++;
				}
				break;
			default:
				GUI_StoreKeyMsg(c, 1);
				GUI_StoreKeyMsg(c, 0);
				break;
			}
		}
		else if (ID == ID_BUTTON_KEYDOT)
		{
			if (_aEdits[nowEditFocus] == ID_EDIT_CURRENT && !_aChannels[nowChannelIndex].ChEditData.CurrentHasDot) {
				GUI_StoreKeyMsg('.', 1);
				GUI_StoreKeyMsg('.', 0);
				_aChannels[nowChannelIndex].ChEditData.CurrentHasDot = 1;
			}
			else if (_aEdits[nowEditFocus] == ID_EDIT_FREQUENCY && !_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot) {
				GUI_StoreKeyMsg('.', 1);
				GUI_StoreKeyMsg('.', 0);
				_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot = 1;
			}
		}
		else if (ID == ID_BUTTON_KEYBACK)
		{
			EDIT_GetText(hItem, acTextSrc, sizeof(acTextSrc));
			Len = strlen(acTextSrc);
			if (Len > 0) {
				c = acTextSrc[Len - 1];
				acTextSrc[Len - 1] = '\0';
				EDIT_SetText(hItem, acTextSrc);
				if (c == '.') {
					if (_aEdits[nowEditFocus] == ID_EDIT_CURRENT) {
						_aChannels[nowChannelIndex].ChEditData.CurrentHasDot = 0;
					}
					else if (_aEdits[nowEditFocus] == ID_EDIT_FREQUENCY)
					{
						_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot = 0;
					}
				}
				if (_aEdits[nowEditFocus] == ID_EDIT_CURRENT) {
					if (_aChannels[nowChannelIndex].ChEditData.AfterCurrentDotCount > 0)
						_aChannels[nowChannelIndex].ChEditData.AfterCurrentDotCount--;
				}
				else if (_aEdits[nowEditFocus] == ID_EDIT_FREQUENCY) {
					if (_aChannels[nowChannelIndex].ChEditData.AfterFrequencyDotCount > 0)
						_aChannels[nowChannelIndex].ChEditData.AfterFrequencyDotCount--;
				}
			}
		}
		else if (ID == ID_BUTTON_KEYCLR)
		{
			EDIT_SetText(hItem, "");
			if (_aEdits[nowEditFocus] == ID_EDIT_CURRENT) {
				_aChannels[nowChannelIndex].ChEditData.CurrentHasDot = 0;
				_aChannels[nowChannelIndex].ChEditData.AfterCurrentDotCount = 0;
			}
			else if (_aEdits[nowEditFocus] == ID_EDIT_FREQUENCY) {
				_aChannels[nowChannelIndex].ChEditData.FrequencyHasDot = 0;
				_aChannels[nowChannelIndex].ChEditData.AfterFrequencyDotCount = 0;
			}

		}
		else if (ID == ID_BUTTON_KEYENTER)
		{
			if (_CheckEditTextLogic(pMsg->hWin))
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRENT);
				EDIT_GetText(hItem, _aChannels[nowChannelIndex].ChEditData.CurrentStr, sizeof(_aChannels[nowChannelIndex].ChEditData.CurrentStr));
				//_Str2Float(_aChannels[nowChannelIndex].ChEditData.CurrentStr, &_aChannels[nowChannelIndex].ChEditData.CurrentData);

				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY);
				EDIT_GetText(hItem, _aChannels[nowChannelIndex].ChEditData.FrequenctStr, sizeof(_aChannels[nowChannelIndex].ChEditData.FrequenctStr));
				//_Str2Float(_aChannels[nowChannelIndex].ChEditData.FrequenctStr, &_aChannels[nowChannelIndex].ChEditData.FrequencyData);

				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY);
				EDIT_GetText(hItem, _aChannels[nowChannelIndex].ChEditData.DutyStr, sizeof(_aChannels[nowChannelIndex].ChEditData.DutyStr));
				//_Str2Float(_aChannels[nowChannelIndex].ChEditData.DutyStr, &_aChannels[nowChannelIndex].ChEditData.DutyData);
				if (_aChannels[nowChannelIndex].OnOff) {
					_DrawGraph();
				}
			}
			else
			{
				_CreateMessageBox("Illegal Value!", "Warning", 0);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRENT);
				EDIT_SetText(hItem, "1");

				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY);
				EDIT_SetText(hItem, "1000");

				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY);
				EDIT_SetText(hItem, "50");
			}
		}
	}
}


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE* pMsg) {
	WM_HWIN hItem;
	int     NotificationCode;
	int     Id;
	int     i;


	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		hItem = pMsg->hWin;
		WINDOW_SetBkColor(hItem, GUI_WHITE);

		_hWin = pMsg->hWin;

		//EDIT init
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRENT);
		EDIT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
		EDIT_SetText(hItem, "1");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FREQUENCY);
		EDIT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
		EDIT_SetText(hItem, "1000");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DUTY);
		EDIT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_24B_ASCII);
		EDIT_SetText(hItem, "50");

		//TEXT init
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, "Current Source");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CURRENTLABEL);
		TEXT_SetTextAlign(hItem, GUI_TA_CENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, "I");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CURRENTUNIT);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, " A");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYLABEL);
		TEXT_SetTextAlign(hItem, GUI_TA_CENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, "f");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FREQUENCYUNIT);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, " Hz");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYLABEL);
		TEXT_SetTextAlign(hItem, GUI_TA_CENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, "Duty");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DUTYUNIT);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		TEXT_SetBkColor(hItem, GUI_WHITE);
		TEXT_SetText(hItem, " %");

		//Graph Init
		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH);
		_aGraph.linecolor = GUI_YELLOW;
		_aGraph.backcolor = GUI_BLACK;
		GRAPH_SetBorder(hItem, 40, 10, 10, 15);
		_aGraph.graphdata = GRAPH_DATA_YT_Create(_aGraph.linecolor, 600, 0, 0);
		GRAPH_AttachData(hItem, _aGraph.graphdata);
		GRAPH_DATA_YT_SetAlign(_aGraph.graphdata, GRAPH_ALIGN_LEFT);

		GRAPH_SetGridDistX(hItem, 20);
		GRAPH_SetGridDistY(hItem, 10);
		GRAPH_SetGridVis(hItem, 1);

		_aGraph.hScaleV = GRAPH_SCALE_Create(35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 50); //绘制垂直刻度
		GRAPH_SCALE_SetTextColor(_aGraph.hScaleV, GUI_BLACK);
		GRAPH_SCALE_SetFactor(_aGraph.hScaleV, 0.05);  //设置刻度系数
		//GRAPH_SCALE_SetOff(_aGraph.hScaleV, 100);    //垂直刻度向上移动100个像素点，这样垂直刻度会有负值
		GRAPH_AttachScale(hItem, _aGraph.hScaleV);   //将刻度对象附加到图形小工具

		_aGraph.hScaleH = GRAPH_SCALE_Create(420, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);//绘制水平刻度
		GRAPH_SCALE_SetTextColor(_aGraph.hScaleH, GUI_DARKGREEN);
		GRAPH_SCALE_SetFactor(_aGraph.hScaleH, 1);
		GRAPH_AttachScale(hItem, _aGraph.hScaleH);

		//Switch Button init
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ONOFF);
		BUTTON_SetBitmapEx(hItem, 0, &bmButtonOFF_110x40, 0, 0);
		BUTTON_SetText(hItem, "");
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SWITCHMODE);
		WM_SetHasTrans(hItem);
		BUTTON_SetText(hItem, "");

		//Initialization Buttons for Channel
		for (i = 0; i < CHANNEL_NUMBERS; i++)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, _aChannels[i].ID);
			WM_SetCallback(hItem, _cbButtonChannel);
		}

		//Initialization Buttons can Switch
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ONOFF);
		WM_SetCallback(hItem, _cbButtonSwitchOnOff);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SWITCHMODE);
		WM_SetCallback(hItem, _cbButtonSwitchMode);

		//Num Button
		for (i = ID_BUTTON_KEY0; i <= ID_BUTTON_KEYCLR; i++)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, i);
			BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
		}


		//Do we have logo to init?

		break;

//	case WM_TOUCH:
//		nowEditFocus = -1;
//		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NotificationCode = pMsg->Data.v;
		switch (Id) {
		case ID_BUTTON_CH1:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnChannelRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_CH2:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnChannelRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_CH3:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnChannelRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_SETTINGS:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_CreateMessageBox("By HQM 2021/4/6/0:50", "Edition", 0);
				break;
			}
			break;
		case ID_BUTTON_ONOFF:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnSwitchRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_SWITCHMODE:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnSwitchRelease(pMsg);
				break;
			}
			break;
		case ID_EDIT_CURRENT:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				nowEditFocus = 0;
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
		case ID_EDIT_FREQUENCY:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				nowEditFocus = 1;
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
		case ID_EDIT_DUTY:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				nowEditFocus = 2;
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
		case ID_BUTTON_KEY7:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY8:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY9:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEYBACK:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY4:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY5:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY6:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEYENTER:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY1:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY2:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY3:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEY0:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEYDOT:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
			break;
		case ID_BUTTON_KEYCLR:
			switch (NotificationCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				_OnNumKeyRelease(pMsg);
				break;
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void GUIMainTask(void)
{
	GUI_Init();

	GUI_Clear();

	WM_MULTIBUF_Enable(1);

	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

//	while (1)
//	{
//		GUI_Delay(100);
//	}
}

/*************************** End of file ****************************/
