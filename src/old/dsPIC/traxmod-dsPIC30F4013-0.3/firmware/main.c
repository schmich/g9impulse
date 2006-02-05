// -*- tab-width: 4 -*-
// TraxMod Player
// For use on dsPIC30F4013 at 7.5MHz * 16PLL and 5VDC
//
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/traxmod/
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// $Log: main.c,v $
// Revision 1.11  2005/09/28 04:56:44  edwards
// More optimization attempts.
//
// Revision 1.10  2005/09/27 21:59:28  edwards
// Speed optimizations.
//
// Revision 1.9  2005/09/27 06:08:51  edwards
// Trying some various changes to the prefetch timing.
//
// Revision 1.8  2005/09/26 05:52:11  edwards
// Playing all sample data out of SD Card! Still a few glitches on tight sample
// loops though (due to SD Card block misalignment).
//
// Revision 1.5  2005/09/20 21:01:46  edwards
// Reading 64 bytes from SD card each mixing loop iteration is working at
// 16KHz.
//
// Revision 1.4  2005/09/16 05:01:39  edwards
// Now working with only 64 byte block length on SD Card reads.
//
// Revision 1.2  2005/09/15 08:29:48  edwards
// Plays raw audio data at 43Hz, 8 bits, stereo from my SD Card now, but
// only seems to work when running under the ICD. Something seriously
// wrong with timing or something somewhere.
//
// Revision 1.1  2005/09/11 10:35:40  edwards
// Ported code to play on dsPIC30F4013.
//
// Revision 1.18  2005/08/05 08:27:23  edwards
// no message
//
// Revision 1.17  2005/08/04 09:20:16  edwards
// Made some improvements to the sound quality and implemented a few more
// effects.
//
// Revision 1.16  2005/08/01 04:52:13  edwards
// Cleaning up code for publication.
//
// Revision 1.15  2005/07/31 20:49:40  edwards
// no message
//
// Revision 1.14  2005/07/29 08:05:24  edwards
// Working TOYBOX.MOD player!
//
// Revision 1.13  2005/07/18 08:13:17  edwards
// Trying to improve the frequency calculations.
//
// Revision 1.12  2005/07/17 11:22:40  edwards
// First music playback engine now working!
//
// Revision 1.11  2005/07/13 03:53:50  edwards
// no message
//
// Revision 1.10  2005/07/12 03:16:20  edwards
// Speed improvements.
//
// Revision 1.9  2005/07/11 08:15:18  edwards
// Mixer is begining to work.
//
// Revision 1.8  2005/07/09 20:42:22  edwards
// Writing initial mixing routine.
//
// Revision 1.7  2005/07/07 14:35:00  edwards
// no message
//
// Revision 1.6  2005/07/07 04:42:48  edwards
// Started setting up code for playing a mix buffer of multiple channels.
//
// Revision 1.5  2005/07/05 06:22:58  edwards
// Minor clean ups.
//
// Revision 1.4  2005/07/04 18:37:42  edwards
// First working playback of sample instrument on the dsPIC.
//
// Revision 1.3  2005/07/03 07:45:01  edwards
// no message
//
// Revision 1.2  2005/06/22 07:35:18  edwards
// no message
//
// Revision 1.1  2005/06/20 07:25:18  edwards
// no message
//
// Revision 1.2  2005/05/30 17:26:27  edwards
// no message

#include <p30f4013.h>

#include "sdcard.h"
#include "mixer.h"
#include "orderdata.h"

// Uncomment the following to enable raw wave audio playing directly off the 
// SD Card. Otherwise defaults to MOD player instead.
// #define RAW_PLAYER

#define IFREQ		30000000

//#define WPLAYFREQ	44100

//#define T1PERIOD	IFREQ / WPLAYFREQ

//#define PLAYFREQ	44053
//#define T1PERIOD	681

//#define PLAYFREQ	30000
//#define T1PERIOD	1000

#define PLAYFREQ	24000
#define T1PERIOD	1250

//#define PLAYFREQ	22059
//#define T1PERIOD	1360

//#define PLAYFREQ	16000
//#define T1PERIOD	1875

//#define PLAYFREQ	12000
//#define T1PERIOD	2500

//#define T1PERIOD	343
//IFREQ / ((unsigned long long)T1PERIOD)

_FOSC(CSW_FSCM_OFF & FRC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(PBOR_OFF & BORV_20 & PWRT_64 & MCLR_EN);
_FGS(CODE_PROT_OFF);

extern void VisualInitialization(void);
void FrameInterrupt(void);

int iPlay[PLAYBUFSIZE];
int* pPlay = iPlay;
volatile unsigned int iPlayLoc = 0;
unsigned int iTempoCount = 6;
unsigned int iTempoPeriod = 6;
unsigned int iRow = 0;
unsigned int iPattern = 0;
unsigned int iFrame = 0;

unsigned int iMasterVolume = 32;

extern void __attribute__((interrupt, shadow)) _T1Interrupt(void);

/*
void PlaySample(unsigned int iChannel, unsigned int iSample, unsigned int iFreq)
{
	ChannelType* pChan = &oChannel[iChannel];

	SetSample(pChan, iSample);

	pChan->iLocFract = 0;
	pChan->iStep = iFreq / PLAYFREQ;
	pChan->iStepFract = ((long long) iFreq << 16) / PLAYFREQ;
	pChan->iVolumeLeft = 64;
	pChan->iVolumeRight = 64;
	pChan->bBiDi = 0;
	pChan->b16 = 0;
	pChan->bStereo = 0;
}
*/

#define AMIGASPD 3579545.25
void SetPeriod(ChannelType* pChan, unsigned int iPeriod)
{
	if(iPeriod)
	{
		pChan->iPeriod = iPeriod;
		pChan->iLocFract = 0;
		pChan->iStep = (unsigned int)(((AMIGASPD * 128) / PLAYFREQ) / iPeriod) >> 7;
		pChan->iStepFract = (long long)((AMIGASPD * 65536) / PLAYFREQ) / iPeriod;
	}
}

void SetVolume(ChannelType* pChan, int iNewVolume)
{
	if(iNewVolume < 0)
	{
		iNewVolume = 0;
	}
	else if(iNewVolume > MAXVOLUME)
	{
		iNewVolume = MAXVOLUME;
	}
	pChan->iVolume = iNewVolume;

	if(pChan->iPan >= 0 && pChan->iPan <= 0x80)
	{
		pChan->iVolLeft  = 
			( (((0x80 - pChan->iPan) * iMasterVolume) >> 7) * iNewVolume) >> 5;
		pChan->iVolRight = 
			( (((pChan->iPan) * iMasterVolume) >> 7) * iNewVolume) >> 5;
	}
	else if(pChan->iPan == 0xA4)
	{
		pChan->iVolLeft  = 
			( ((64 * iMasterVolume) >> 7) * iNewVolume) >> 5;
		pChan->iVolRight = 
			( ((64 * iMasterVolume) >> 7) * iNewVolume) >> 5;
		pChan->iVolRight = 0 - pChan->iVolRight;
	}
}

void SetBPM(unsigned int iNewBPM)
{
	iFramePeriod = (PLAYFREQ / iNewBPM * 5) >> 1;
	iFrameCounter = iFramePeriod;
}

unsigned int iPatPtr;
unsigned int iCurOrder = 0;
unsigned int iCurRow = 0;
unsigned int iCurChan = 0;
ChannelType* pChan = &oChannel[0];
void SetOrder(unsigned int iNewOrder)
{
	iCurOrder = iNewOrder;
	if(iCurOrder >= NUMORDERS)
	{
		iCurOrder = 0;
	}
	
	iPatPtr = getPattern(iCurOrder);
	iCurRow = 0;
	
	SD_ChannelCache(iCurChan);
	SD_Prefetch(0);

	iCurChan = 0;
	pChan = &oChannel[0];
	
	iTempoCount = iTempoPeriod;
}

/*
          note:  C    C#   D    D#   E    F    F#   G    G#   A    A#   B
        period: 1712,1616,1524,1440,1356,1280,1208,1140,1076,1016,0960,0907

        middle octave is 4.

                         8363 * 16 * ( period(NOTE) >> octave(NOTE) )
        note_st3period = --------------------------------------------
                         middle_c_finetunevalue(INSTRUMENT)

        note_amigaperiod = note_st3period / 4

        note_herz=14317056 / note_st3period
*/
void DoMusic(void)
{
	unsigned int iEndOfRow = 0;
	unsigned int i;

	iCurRow++;
	do
	{
		unsigned char byte1 = getByte(iPatPtr++);
		unsigned char byte2;

		if(byte1 & 0x80)
		{
			if((byte1 & 0xF0) == 0xB0)
			{
				// Wait & SetChannel
				iEndOfRow = 1;
				SD_ChannelCache(iCurChan);
				SD_Prefetch(2);
				iCurChan = byte1 & 0x3;
				pChan = &oChannel[iCurChan];
				iTempoCount = iTempoPeriod + iTempoPeriod * ((byte1 >> 2) & 0x3);
				continue;
			}

			// Otherwise, just increment channel
			SD_ChannelCache(iCurChan);
			SD_Prefetch(2);
			iCurChan++;
			pChan = &oChannel[iCurChan];
			byte1 = byte1 & 0x7F;
		}

		switch(byte1 >> 4)
		{
			case 0x1:
				// Set Period, Set Sample, and Set Volume
				i = ((byte1 & 0x0F) << 8) | getByte(iPatPtr++); 
				SetSample(pChan, getByte(iPatPtr++));
				SetPeriod(pChan, i);
				SetVolume(pChan, getByte(iPatPtr++));
				break;

			case 0x3:
				// Set Channel
				SD_ChannelCache(iCurChan);
				SD_Prefetch(2);
				iCurChan = byte1 & 0x0F;
				pChan = &oChannel[iCurChan];
				break;

			case 0x4:
				// Porta to Note
				pChan->iPortaDest = ((byte1 & 0x0F) << 8) | getByte(iPatPtr++); 
				byte2 = getByte(iPatPtr++);
				if(byte2 != 0)
				{
					pChan->iEffect = 0x300 | byte2;
					pChan->iPortaSpeed = byte2;
				}
				else
				{
					pChan->iEffect = 0x300 | (pChan->iEffect & 0xFF);
				}
				SetSample(pChan, getByte(iPatPtr++));
				break;

			case 0x2:
				// Do Effect				
				switch(byte1 & 0x0F)
				{
					case 0x8:
						// Set Panning
						byte2 = getByte(iPatPtr++);
						pChan->iPan = byte2;
						SetVolume(pChan, pChan->iVolume);
						break;

					case 0x9:
						// Set Sample Offset
						SetSample(pChan, pChan->iSample);
						pChan->SDLoc += getByte(iPatPtr++) << 8;
						if(pChan->SDLoc > pChan->SDEnd)
						{	
							pChan->SDLoc = pChan->SDEnd;
							pChan->bActive = 0;
						}
						break;
						
					case 0xB:
						// Jump to Order						
						byte2 = getByte(iPatPtr++);
						SetOrder(((byte2 & 0xF0) * 16) + (byte2 & 0x0F));
						iEndOfRow = 1;
						break;
						
					case 0xC:
						// Set volume
						SetVolume(pChan, getByte(iPatPtr++));
						break;

					case 0xD:
						// Pattern break
						// TODO this is supposed to be able to break to a specific
						// row number.
						SetOrder(iCurOrder+1);
						iEndOfRow = 1;
						break;

					case 0xE:
						byte2 = getByte(iPatPtr++);
						switch(byte2 >> 4)
						{
							case 0x1:
								// Fine Porta Up
								SetPeriod(pChan, pChan->iPeriod - (byte2 & 0x0F));
								break;

							case 0x2:
								// Fine Porta Down
								SetPeriod(pChan, pChan->iPeriod + (byte2 & 0x0F));
								break;

							case 0x8:
								// Set Pan
								pChan->iPan = (byte2 & 0x0F) << 3;
								SetVolume(pChan, pChan->iVolume);
								break;

							case 0xA:
								// Fine Volume Slide Up
								SetVolume(pChan, pChan->iVolume + (byte2 & 0x0F));
								break;

							case 0xB:
								// Fine Volume Slide Down
								SetVolume(pChan, pChan->iVolume - (byte2 & 0x0F));
								break;


							default:
								if((byte2 && 0x0F) != 0)
								{
									pChan->iEffect = ((byte1 & 0x0F) << 8) | byte2;
								}
								else
								{
									pChan->iEffect = ((byte1 & 0x0F)<< 8) | (byte2 & 0xF0) | (pChan->iEffect & 0x0F);
								}
								break;
						}
						break;
		
					case 0xF:
						// Set tempo/bpm
						byte2 = getByte(iPatPtr++);
						i = (byte2 >> 4) * 16 + (byte2 & 0x0F);
						if(i > 32)
						{
							SetBPM(i);
						}
						else
						{
							iTempoPeriod = i;
						}
						break;

					default:
						byte2 = getByte(iPatPtr++);
						if(byte2 != 0)
						{
							pChan->iEffect = ((byte1 & 0x0F) << 8) | byte2;
						}
						else
						{
							pChan->iEffect = ((byte1 & 0x0F)<< 8) | (pChan->iEffect & 0xFF);
						}
						break;
				}
				break;
		}
	} while(iEndOfRow == 0);

	iFrame = 0;
}

void DoEffects(void)
{
	iFrame++;

	int i;
	for(i = 0; i < CHANNELS; ++i)
	{
		ChannelType *pChan = &oChannel[i];
		if(!pChan->bActive)
		{
			continue;
		}

		switch(pChan->iEffect >> 8)
		{
			case 0:
				continue;
			case 0x1:
				// Porta Up
				pChan->iPeriod -= pChan->iEffect & 0xFF;
				if(pChan->iPeriod <= 56)
				{
					pChan->iPeriod = 56;
				}			
				SetPeriod(pChan, pChan->iPeriod);
				break;

			case 0x2:
				// Porta Down
				pChan->iPeriod += pChan->iEffect & 0xFF;
				SetPeriod(pChan, pChan->iPeriod);
				break;

			case 0x3:
				// Porta to Note
				if(pChan->iPeriod == pChan->iPortaDest)
					continue;
	
				if(pChan->iPeriod > pChan->iPortaDest)
				{
					pChan->iPeriod -= pChan->iPortaSpeed;
					if(pChan->iPeriod < pChan->iPortaDest)
					{
						pChan->iPeriod = pChan->iPortaDest;
					}
				}
				else
				{
					pChan->iPeriod += pChan->iPortaSpeed;
					if(pChan->iPeriod > pChan->iPortaDest)
					{
						pChan->iPeriod = pChan->iPortaDest;
					}
				}

				SetPeriod(pChan, pChan->iPeriod);
				break;

			case 0x5:
				// Porta to Note
				if(pChan->iPeriod == pChan->iPortaDest)
					continue;
	
				if(pChan->iPeriod > pChan->iPortaDest)
				{
					pChan->iPeriod -= pChan->iPortaSpeed;
					if(pChan->iPeriod < pChan->iPortaDest)
					{
						pChan->iPeriod = pChan->iPortaDest;
					}
				}
				else
				{
					pChan->iPeriod += pChan->iPortaSpeed;
					if(pChan->iPeriod > pChan->iPortaDest)
					{
						pChan->iPeriod = pChan->iPortaDest;
					}
				}

				SetPeriod(pChan, pChan->iPeriod);
				// Porta + Volume Slide

				// Fall through...

			case 0x6:
				// Vibrato + Volume Slide

				// Fall through...

			case 0xA:
				// Volume slide
				if(pChan->iEffect & 0x00F)
				{
					// slide down
					SetVolume(pChan, pChan->iVolume - (pChan->iEffect & 0x00F));
				}
				if(pChan->iEffect & 0x0F0)
				{
					// slide up
					SetVolume(pChan, pChan->iVolume + ((pChan->iEffect >> 4) & 0x00F));
				}
				break;

			case 0xE:
				// Extended Effects
				switch((pChan->iEffect >> 4) & 0x0F)
				{
					case 0x9:
						// Retrigger
						if((iFrame % (pChan->iEffect & 0x0F)) == 0)
						{
							SetSample(pChan, pChan->iSample);
						}
						break;

					case 0xC:
						// Note Cut
						if(iFrame == (pChan->iEffect & 0x0F))
						{
							pChan->bActive = 0;
							pChan->iVolume = 0;
						}
						break;
				}

			default:
				break;
		}
	}
}

//#define LED PORTBbits.RB0
#define LED_DIR TRISBbits.TRISB0

volatile unsigned int iPlayPtr;
int main (void)
{
	unsigned char status;
	unsigned int bFrameProcess;


	LED_DIR = 0; // output pin
	LED = 1;

	VisualInitialization();
	ADPCFG = 0xFFFF;

	InitSPI();
	do
	{
		status = InitSD();
	} while(status != 0);

	InitMixer();
	iPatPtr = getPattern(0);
	SetBPM(125);
	FrameInterrupt(); // process first row of music
	SD_Prefetch(0);
	
/*
	U1MODEbits.ALTIO = 1;
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB1 = 0;
	TRISBbits.TRISB6 = 1;
	TRISC = 0x3FFF;
*/

	OC3CON = 0x0006;
	OC4CON = 0x0006;
	T1CON = 0x8004; // Timers1: B15=TON B13=TSIDL B5=TGATE B5:4=TCKPS1:0 B2=TSYNC B1=TCS
	T2CON = 0xA000; // Timers2,4:   B15=TON B13=TSIDL B5=TGATE B5:4=TCKPS1:0 B3=T32 B1=TCS
	IEC0bits.T1IE = 1;	// timer 1 interrupt enable


	TMR2 = 0;
	PR2 = 0x1FF;			// Set PWM frequency

	TMR1 = 0;
	PR1 = T1PERIOD;			// Set playback frequency

	for(;;)
	{
		unsigned int i;
		unsigned int j;

//		LED = 1;
		for(i = 0; i < 1024; ++i)
		{
#ifdef RAW_PLAYER
			DoPlay();
#else
			bFrameProcess = DoMix();
			if(bFrameProcess)
			{
				FrameInterrupt();
			}
#endif
			for(i = 0; i < CHANNELS; ++i)
			{
				if(oChannel[i].bActive)
				{
					SD_ChannelCache(i);
				}
			}		
			SD_Prefetch(0);

			if(SD_State == 0)
			{
				LED = 0;
			}
			else
			{
				LED = 1;
			}
		}

//		LED = 0;
		for(j = 0; j < 15; ++j)
		{
			for(i = 0; i < 65535; ++i)
			{
#ifdef RAW_PLAYER
				DoPlay();
#else
				bFrameProcess = DoMix();
				if(bFrameProcess)
				{
					FrameInterrupt();
				}
#endif
				for(i = 0; i < CHANNELS; ++i)
				{
					if(oChannel[i].bActive)
					{
						SD_ChannelCache(i);
					}
				}		
				SD_Prefetch(0);
			
				if(SD_State == 0)
				{
					LED = 0;
				}
				else
				{
					LED = 1;
				}
			}
		}
	}

	return 0;
}


/**
 * Timer 1 Interrupt Service Routine
 *
 * This function grabs the next sample from the play buffer and
 * puts it in the PWM registers. It should be called at the sample
 * frequency.
 *
 *
 * PWM Frequency = 1 / ((PR2 + 1) * 4 * Tosc * T2PRESCALER)
 * PWM Bits 	 = log(Fosc / Fpwm) / log(2)
 * 11 bits = log(120000000 / Fpwm) / log(2)
 * 11 * log(2) = log(120000000 / Fpwm)
 * 10 ^ (11 * log(2)) = 120000000 / Fpwm
 *
 * 11 bits
 * Fpwm = 120000000 / 10 ^ (11 * log(2)) = 58.59375KHz
 *
 * 10 bits
 * Fpwm = 120000000 / 10 ^ (10 * log(2)) = 117187.5Hz
 * @author edwards
 */
/*
void __attribute__((interrupt, shadow)) _T1Interrupt(void)
{
	// Clear interrupt flag so we can get an interrupt again later...
	IFS0bits.T1IF = 0;
	
	asm("mov [%0++], w0\n mov [%0++], w1\n mov w0, %1\n mov w1, %2" :
		"+r"(pPlay), "=g"(OC1RS), "=g"(OC2RS) : : "w0", "w1");
	iPlayLoc += 2;
//	OC1RS = iPlay[iPlayLoc++]; // Left
//	OC2RS = iPlay[iPlayLoc++]; // Right

	if(iPlayLoc >= PLAYBUFSIZE)
	{
		iPlayLoc = 0;
		pPlay = iPlay;
	}
}
*/
/**
 * This is not really invoked by any hardware interrupt, but it is called
 * by the mixer everytime a frame has been finished mixing. All processing
 * between frames must then be initiated by this routine.
 *
 * @author edwards
 * @date 7/30/2005
 */
void FrameInterrupt(void)
{
	iTempoCount--;
	if(iTempoCount == 0)
	{
		int i;
		iTempoCount = iTempoPeriod;

		// Clear effects
		for(i = 0; i < CHANNELS; ++i)
		{
			ChannelType *pChannel = &oChannel[i];
			pChannel->iEffect = pChannel->iEffect &  0xFF;
		}
		DoMusic();
	}
	else
	{
		DoEffects();
	}
}
