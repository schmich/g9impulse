#ifndef INPUT_H
#define INPUT_H

#include "common.h"

typedef struct _InputEvent
{
	bool buttonAPressed;
	bool buttonBPressed;
	
	bool startPressed;
	bool selectPressed;
	
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	
	bool anyInput;
} InputEvent;


void serialInit();
InputEvent* getInputEvent();

#endif
