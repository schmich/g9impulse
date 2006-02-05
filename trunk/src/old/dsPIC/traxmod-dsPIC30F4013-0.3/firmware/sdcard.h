// -*- tab-width: 4 -*-
// SD Card Support Routines
// For use on dsPIC30F4013
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
// $Log: sdcard.h,v $
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
// Revision 1.7  2005/09/26 02:35:35  edwards
// Caching SD Card data is working, but needs to be optimized to be faster.
//
// Revision 1.6  2005/09/20 21:01:46  edwards
// Reading 64 bytes from SD card each mixing loop iteration is working at
// 16KHz.
//
// Revision 1.5  2005/09/18 09:09:20  edwards
// SPI Interrupt driven SD Card command write working.
//
// Revision 1.4  2005/09/18 06:40:28  edwards
// SPI Interrupt driven SD Card read working.
//
// Revision 1.3  2005/09/16 05:01:39  edwards
// Now working with only 64 byte block length on SD Card reads.
//
// Revision 1.2  2005/09/16 04:29:44  edwards
// Now working without ICD.
//
// Revision 1.1  2005/09/15 08:29:48  edwards
// Plays raw audio data at 43Hz, 8 bits, stereo from my SD Card now, but
// only seems to work when running under the ICD. Something seriously
// wrong with timing or something somewhere.
//

#ifndef SDCARD_H
#define SDCARD_H

#include "mixer.h"

#define SD_SIZE 64

extern unsigned int SD_BlockLen;
extern volatile unsigned char* SD_Buffer;
extern volatile unsigned char* SD_BufferEnd;

extern unsigned long SD_PriReq[CHANNELS]; // Primary SD Address Requested table
extern unsigned long SD_SecReq[CHANNELS]; // Secondary SD Address Requested table

extern unsigned long SD_CacheAddr[CHANNELS*2];
extern unsigned char SD_Cache[CHANNELS*2][SD_SIZE];
extern unsigned long SD_CacheLoad;
extern unsigned long SD_NextCacheLoad;
extern unsigned int SD_NextBuffer;

extern volatile unsigned int SD_State;

#define LED PORTBbits.RB0
#define SDCARD 1

#define SD_PWR PORTBbits.RB1
#define SD_PWR_DIR TRISBbits.TRISB1
#define SD_CS PORTBbits.RB3
#define SD_CS_DIR TRISBbits.TRISB3

#define SDI PORTFbits.RF2
#define SDI_DIR TRISFbits.TRISF2
#define SCK PORTFbits.RF6
#define SCK_DIR TRISFbits.TRISF6
#define SDO PORTFbits.RF3
#define SDO_DIR TRISFbits.TRISF3

#define SD_PowerOn() SD_PWR = 1
#define SD_PowerOff() SD_PWR = 0
#define SD_Enable() SD_CS = 0 /* set low to activate SD Card chip select */
#define SD_Disable() SD_CS = 1 /* set high to deactivate SD Card chip select */

/*
// Note: 1:1 clocking does not work at 30MIPS, SD Card specifies
// 	maximum SPI clocking of 25MHz.
// primary prescaler 1:1
// secondary prescaler 1:1
*/
//#define SPIFastClock() SPI1CON = 0x007F

// primary prescaler 1:1
// secondary prescaler 1:2
#define SPIFastClock() SPI1CON = 0x007B

extern void SPIWrite(unsigned char data);
extern unsigned char SPIRead(void);
extern void InitSPI(void);

extern unsigned char SD_WriteCommand(unsigned char* cmd);
extern unsigned char InitSD();
extern unsigned char SD_ReadBlock(unsigned long addr, unsigned char *buf);
extern unsigned char SD_StartRead(unsigned long addr);
extern unsigned char SD_StartCache(unsigned long addr);
//extern unsigned char SD_CacheSample(ChannelType* pChan);

extern int SD_GetSample(unsigned long addr);
extern void SD_ChannelCache(unsigned int iChannel);
extern unsigned int SD_Prefetch(unsigned int iFlags);

extern unsigned char SD_SetBlockLen(unsigned int iBlockLen);
//extern unsigned char SD_InitRead(unsigned long addr);
//extern void SD_EndRead(void);
//extern unsigned int SD_GetSample(unsigned long lSampleLoc);
extern int SD_ReadSample(unsigned int addr);

typedef union
{
	struct
	{
		unsigned char cmd;
		unsigned long address;
		unsigned char crc;
	};

	struct
	{
		unsigned char cmd;
		unsigned char alignment; // doesn't actually contain data, just dummy
		unsigned char addr[4];
		unsigned char crc;
	};
} CommandType;

#endif // SDCARD_H
