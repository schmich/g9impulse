#ifndef INPUT_H
#define INPUT_H

#include "common.h"

typedef struct _Input
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
} Input;


void inputInit();
Input* getInputEvent();
Input* getInputStatus();

#endif
