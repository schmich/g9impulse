#include "input.h"

#define INPUT_RIGHT_BUTTON  0
#define INPUT_LEFT_BUTTON   1
#define INPUT_DOWN_BUTTON   2
#define INPUT_UP_BUTTON     3
#define INPUT_START_BUTTON  4
#define INPUT_SELECT_BUTTON 5
#define INPUT_B_BUTTON      6
#define INPUT_A_BUTTON      7

#define INPUT_PRESSED_EVENT_FLAG 0b10000000
#define INPUT_KEY_MASK 0b01111111

Input theInputEvent;
Input theInputStatus;
Input theEmptyInput;

bool anInputEvent = false;

void clearInput(Input* inp)
{
    inp->buttonAPressed = false;
    inp->buttonBPressed = false;
    inp->startPressed   = false;
    inp->selectPressed  = false;
    inp->upPressed      = false;
    inp->downPressed    = false;
    inp->leftPressed    = false;
    inp->rightPressed   = false;
    inp->anyInput       = false;
}

#pragma interrupt high_isr
void high_isr(void)
{
    char tempEvent;
    char key;
    bool pressed;

    if (PIR1 & 0b00100000)
    {
        tempEvent = RCREG;

        clearInput(&theInputEvent);

        key = tempEvent & INPUT_KEY_MASK;
        pressed = tempEvent & INPUT_PRESSED_EVENT_FLAG;

        switch (key)
        {
            case INPUT_A_BUTTON:
                theInputStatus.buttonAPressed = pressed;
                theInputEvent.buttonAPressed = pressed;
                break;

            case INPUT_B_BUTTON:
                theInputStatus.buttonBPressed = pressed;
                theInputEvent.buttonBPressed = pressed;
                break;

            case INPUT_START_BUTTON:
                theInputStatus.startPressed = pressed;
                theInputEvent.startPressed = pressed;
                break;

            case INPUT_SELECT_BUTTON:
                theInputStatus.selectPressed = pressed;
                theInputEvent.selectPressed = pressed;
                break;

            case INPUT_UP_BUTTON:
                theInputStatus.upPressed = pressed;
                theInputEvent.upPressed = pressed;
                break;

            case INPUT_DOWN_BUTTON:
                theInputStatus.downPressed = pressed;
                theInputEvent.downPressed = pressed;
                break;

            case INPUT_LEFT_BUTTON:
                theInputStatus.leftPressed = pressed;
                theInputEvent.leftPressed = pressed;
                break;

            case INPUT_RIGHT_BUTTON:
                theInputStatus.rightPressed = pressed;
                theInputEvent.rightPressed = pressed;
                break;
        }

        theInputStatus.anyInput = theInputStatus.anyInput || pressed;
        theInputEvent.anyInput = pressed;

        anInputEvent = true;
    }
}

#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
	_asm GOTO high_isr _endasm
}
#pragma code

Input* getInputEvent(void)
{
    if (anInputEvent)
    {
        anInputEvent = false;
        return &theInputEvent;
    }
    else
    {
        return &theEmptyInput;
    }
}    

Input* getInputStatus(void)
{
    return &theInputStatus;
}

//initialize serial port for continuous receive
void inputInit(void)
{
	TRISCbits.TRISC7 = 1;
	TRISCbits.TRISC6 = 1;
	SPBRG = 80;
	TXSTAbits.BRGH = 0;
	TXSTAbits.SYNC = 0;
	BAUDCONbits.BRG16 = 0;
	RCSTAbits.SPEN = 1;
	TXSTAbits.TXEN = 1;
	RCSTAbits.CREN = 1;
	PIE1bits.RCIE = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;

    clearInput(&theEmptyInput);
    clearInput(&theInputStatus);
    clearInput(&theInputEvent);
}
