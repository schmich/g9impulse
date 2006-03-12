#include "spu.h"
#include "delay.h"

// plays the sound file specified by track
void playMusicRaw(uint8 track)
{    
    PORTC = 0x0F;   // load GPU sound register
    PORTB = track;
    PORTA = 0x01;   // enable load
    delay_ms(1);    // account for the fact that the SPU may be running slowly
    PORTA = 0x00;   // load complete
}

void playMusic(uint8 track)
{
    stopMusic();
    delay_ms(20);
    playMusicRaw(track);
}

void stopMusic(void)
{
    playMusicRaw(255);
}
