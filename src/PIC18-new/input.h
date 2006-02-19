#ifndef INPUT_H
#define INPUT_H

#include "common.h"

typedef struct Input
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

void high_isr(void);
void inputInit(void);
Input* getInputEvent(void);
Input* getInputStatus(void);

#endif // INPUT_H
