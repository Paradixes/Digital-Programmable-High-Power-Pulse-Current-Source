#ifndef KEYPAD_H
#define KEYPAD_H

//Keypad
#define KEYPAD_X_BEGIN 600
#define KEYPAD_Y_BEGIN 280
#define KEYPAD_WIDTH 50
#define KEYPAD_HEIGHT 50
#define KEYPAD_XGAP 20
#define KEYPAD_YGAP 20

#define KEYPAD_X_POS(i)         KEYPAD_X_BEGIN - (KEYPAD_WIDTH + KEYPAD_XGAP) * i
#define KEYPAD_Y_POS(i)			KEYPAD_Y_BEGIN - (KEYPAD_WIDTH + KEYPAD_YGAP) * i

typedef struct
{
	int ID;
	char Name[20];
	int xPos;
	int yPos;
	int Width;
	int Height;
} KEYPAD_DATA;

#endif // !KEYPAD_H
