#include <system.h>
#include "spu_pic.h"

//plays the sound file specified by track
void playmusic(char track)
{    
    portc = 0x0F;                                 //Load GPU sound register
    portb = track;
    porta = 0x01;                                //enable load
    delay_ms (1);                                //account for the fact that the SPU may be running slowly
    porta = 0x00;                                //load complete
    return;
}
