#include <system.h>
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

Input theInput;
Input theEmptyInput;

bool theNewInputFlag = false;

void interrupt()
{
    char tempEvent;
    char key;
    bool pressed;

    if (pir1 & 0b00100000)
    {
        tempEvent = rcreg;

        key = tempEvent & INPUT_KEY_MASK;
        pressed = tempEvent & INPUT_PRESSED_EVENT_FLAG;

        switch (key)
        {
            case INPUT_A_BUTTON:
                theInput.buttonAPressed = pressed;
                break;

            case INPUT_B_BUTTON:
                theInput.buttonBPressed = pressed;
                break;

            case INPUT_START_BUTTON:
                theInput.startPressed = pressed;
                break;

            case INPUT_SELECT_BUTTON:
                theInput.selectPressed = pressed;
                break;

            case INPUT_UP_BUTTON:
                theInput.upPressed = pressed;
                break;

            case INPUT_DOWN_BUTTON:
                theInput.downPressed = pressed;
                break;

            case INPUT_LEFT_BUTTON:
                theInput.leftPressed = pressed;
                break;

            case INPUT_RIGHT_BUTTON:
                theInput.rightPressed = pressed;
                break;
        }

        theInput.anyInput = theInput.anyInput || pressed;

        theNewInputFlag = true;
    }
}

Input* getInputEvent()
{
    if (theNewInputFlag)
    {
        theNewInputFlag = false;
        return &theInput;
    }
    else
    {
        return &theEmptyInput;
    }
}    

Input* getInputStatus()
{
    return &theInput;
}

//initialize serial port for continuous receive
void inputInit()
{
    set_bit(trisc, 7);
    set_bit(trisc, 6);
    spbrg = 80;
    clear_bit(txsta, BRGH); 
    clear_bit(baudcon, BRG16);
    clear_bit(txsta, SYNC);
    set_bit(rcsta, SPEN);
    set_bit(txsta, TXEN);
    set_bit(rcsta, CREN);
    //enable interrupts
    set_bit(pie1, RCIE);
    set_bit(intcon, PEIE);
    set_bit(intcon, GIE);

    // initialize empty input to empty!
    theEmptyInput.buttonAPressed = false;
    theEmptyInput.buttonBPressed = false;
    theEmptyInput.startPressed   = false;
    theEmptyInput.selectPressed  = false;
    theEmptyInput.upPressed      = false;
    theEmptyInput.downPressed    = false;
    theEmptyInput.leftPressed    = false;
    theEmptyInput.rightPressed   = false;
    theEmptyInput.anyInput       = false;

    // initialize input to empty!
    theInput.buttonAPressed = false;
    theInput.buttonBPressed = false;
    theInput.startPressed   = false;
    theInput.selectPressed  = false;
    theInput.upPressed      = false;
    theInput.downPressed    = false;
    theInput.leftPressed    = false;
    theInput.rightPressed   = false;
    theInput.anyInput       = false;
}
