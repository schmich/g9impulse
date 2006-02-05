// -*- tab-width: 4 -*-
// Digital Sound Mixer
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/traxmod/
//
// $Log: mixer.c,v $
// Revision 1.14  2005/09/28 04:56:44  edwards
// More optimization attempts.
//
// Revision 1.13  2005/09/27 21:59:28  edwards
// Speed optimizations.
//
// Revision 1.12  2005/09/27 06:08:51  edwards
// Trying some various changes to the prefetch timing.
//
// Revision 1.8  2005/09/20 21:01:46  edwards
// Reading 64 bytes from SD card each mixing loop iteration is working at
// 16KHz.
//
// Revision 1.7  2005/09/18 09:09:20  edwards
// SPI Interrupt driven SD Card command write working.
//
// Revision 1.6  2005/09/18 06:40:27  edwards
// SPI Interrupt driven SD Card read working.
//
// Revision 1.5  2005/09/16 05:15:41  edwards
// Trying to get end of card to behave gracefully.
//
// Revision 1.4  2005/09/16 05:01:39  edwards
// Now working with only 64 byte block length on SD Card reads.
//
// Revision 1.3  2005/09/16 04:29:44  edwards
// Now working without ICD.
//
// Revision 1.2  2005/09/15 08:29:48  edwards
// Plays raw audio data at 43Hz, 8 bits, stereo from my SD Card now, but
// only seems to work when running under the ICD. Something seriously
// wrong with timing or something somewhere.
//
// Revision 1.1  2005/09/11 10:35:40  edwards
// Ported code to play on dsPIC30F4013.
//
// Revision 1.4  2005/08/05 08:27:23  edwards
// no message
//
// Revision 1.3  2005/08/04 09:20:16  edwards
// Made some improvements to the sound quality and implemented a few more
// effects.
//
// Revision 1.2  2005/08/01 04:52:13  edwards
// Cleaning up code for publication.
//
// Revision 1.1  2005/07/31 20:49:40  edwards
// no message
//

#include <p30f4013.h>

#include "sdcard.h"

#include "mixer.h"

unsigned int iMixLoc = 0;
unsigned int iFrameCounter = 0;
unsigned int iFramePeriod = 0;

ChannelType oChannel[CHANNELS];

/*
int xSD_GetSample(ChannelType *pChan, unsigned long addr)
{
	unsigned int i;
	unsigned int iSample, j;
	static unsigned long addrmasked;

//	addrmasked = addr & (~(SD_SIZE - 1));
	asm("mov #0xFFC0, w0\n and %1, w0, w0\n mov w0, %0\n mov w3, %0+2" 
		: "=T"(addrmasked) : "r"(addr) : "w0");

	if(addrmasked != pChan->lBufferAddress)
	{
		// Find cache buffer that holds this address
		for(i = 0, j = 0; i < CHANNELS*2; ++i, ++j)
		{
			if(SD_CacheAddr[j] == addrmasked)
			{
				// found it!
				pChan->pBuffer = SD_Cache[j];
				pChan->lBufferAddress = SD_CacheAddr[j];
				pChan->iBuffer = j;
				break;
			}
		} 

		if(i == (CHANNELS*2) || SD_CacheAddr[j] == SD_CacheLoad)
		{
			// this shouldn't happen
			pChan->iBuffer = 0;
			pChan->pBuffer = SD_Cache[0];
			pChan->lBufferAddress = SD_CacheAddr[0];
			SD_Prefetch(0);
		}
	}

	iSample = *pChan->pBuffer;
	asm("se	w0, %0" : "+r"(iSample)); // sign extend
	return iSample;	
}
*/

void MixChannel(ChannelType *pChan, unsigned int iChannel, unsigned int iBufferLoc, unsigned int iLength)
{
	unsigned int i, iVolLeft, iVolRight, bLoop;
	unsigned int iSampleStep, iSampleStepC;
	unsigned int iSampleLocFract, iSampleStepFract;
	unsigned long SDLoc;
	unsigned long SDLoopStart, SDEnd;
	int iTmpLFT, iTmpRGT;
	int* pBuffer = &(iPlay[iBufferLoc]);

	iVolLeft = pChan->iVolLeft;
	iVolRight = pChan->iVolRight;
	SDLoc = pChan->SDLoc;
	iSampleStep = pChan->iStep;
	iSampleLocFract = pChan->iLocFract;
	iSampleStepFract = pChan->iStepFract;
	bLoop = pChan->bLoop;
	SDLoopStart = pChan->SDLoopStart;
	SDEnd = pChan->SDEnd;

	asm("mul.ss %2, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %0\n mul.ss %3, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %1" :
		"=r"(iTmpLFT), "=r"(iTmpRGT) : 
		"r"(iVolLeft), "r"(iVolRight), "r"(SD_GetSample(SDLoc)) :
		"w2", "w3");

	for(i = 0; i < iLength; ++i)
	{
 		// *pBuffer += iTmpLFT;
		// pBuffer++;
		// *pBuffer += iTmpRGT;
		// pBuffer++;
		// iSampleLocFract = iSampleLocFract + iSampleStepFract
	 	// iSampleStepC = iSampleStep + CARRY(iSampleLocFract + iSampleStepFract)

		asm("add %3, [%0], [%0++]\n add %5, %1, %1\n addc %7, %6, %2\n add %4, [%0], [%0++]" 
			: "+r"(pBuffer), "+r"(iSampleLocFract), "=r"(iSampleStepC) 
			: "r"(iTmpLFT), "r"(iTmpRGT), "r"(iSampleStepFract), "r"(iSampleStep), "r"(0));

/*		asm("add %1, [%0], [%0++]\n add %2, [%0], [%0++]" :
			"+r"(pBuffer) : "r"(iTmpLFT), "r"(iTmpRGT));

		asm("add %2, %0, %0\naddc %4, %3, %1" : 
			"+r"(iSampleLocFract), "=r"(iSampleStepC) : 
			"r"(iSampleStepFract), "r"(iSampleStep), "r"(0));
*/
		// Did the sample offset change at all?
		if(iSampleStepC)
		{
			SDLoc += iSampleStepC;
//			pChan->pBuffer += iSampleStepC;

			if(SDLoc >= SDEnd)
			{
				if(!bLoop)
				{
					// Sample is done playing.
					pChan->bActive = 0;
					break;
				}
				else
				{
					SDLoc = SDLoopStart;
//					pChan->lBufferAddress = -1;
				}
			}

			// Load the new sample
//			iSample = GetSample(iSampleLoc);
//			iTmpLFT = (iVolLeft * iSample) >> 16;
//			iTmpRGT = (iVolRight * iSample) >> 16; 
			asm("mul.ss %2, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %0\n mul.ss %3, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %1" :
				"=r"(iTmpLFT), "=r"(iTmpRGT) : 
				"r"(iVolLeft), "r"(iVolRight), "r"(SD_GetSample(SDLoc)) :
				"w2", "w3");
		}

/*		if(pBuffer >= iPlay + PLAYBUFSIZE)
		{
			pBuffer = iPlay;
		}
*/
	}

	pChan->SDLoc = SDLoc;
	pChan->iLocFract = iSampleLocFract;
}


void WriteChannel(ChannelType *pChan, unsigned int iChannel, unsigned int iBufferLoc, unsigned int iLength)
{
	unsigned int i, iVolLeft, iVolRight, bLoop;
	unsigned int iSampleStep, iSampleStepC;
	unsigned int iSampleLocFract, iSampleStepFract;
	unsigned long SDLoc;
	unsigned long SDLoopStart, SDEnd;
	int iTmpLFT, iTmpRGT;
	int* pBuffer = &(iPlay[iBufferLoc]);

	iVolLeft = pChan->iVolLeft;
	iVolRight = pChan->iVolRight;
	SDLoc = pChan->SDLoc;
	iSampleStep = pChan->iStep;
	iSampleLocFract = pChan->iLocFract;
	iSampleStepFract = pChan->iStepFract;
	bLoop = pChan->bLoop;
	SDLoopStart = pChan->SDLoopStart;
	SDEnd = pChan->SDEnd;

	asm("mul.ss %2, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %0\n mul.ss %3, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %1" :
		"=r"(iTmpLFT), "=r"(iTmpRGT) : 
		"r"(iVolLeft), "r"(iVolRight), "r"(SD_GetSample(SDLoc)) :
		"w2", "w3");
	for(i = 0; i < iLength; ++i)
	{
		asm("add %3, [%0], [%0++]\n add %5, %1, %1\n addc %7, %6, %2\n add %4, [%0], [%0++]" 
			: "+r"(pBuffer), "+r"(iSampleLocFract), "=r"(iSampleStepC) 
			: "r"(iTmpLFT), "r"(iTmpRGT), "r"(iSampleStepFract), "r"(iSampleStep), "r"(0));

		// Did the sample offset change at all?
		if(iSampleStepC)
		{
			SDLoc += iSampleStepC;
//			pChan->pBuffer += iSampleStepC;
			if(SDLoc >= SDEnd)
			{
				if(!bLoop)
				{
					// Sample is done playing.
					pChan->bActive = 0;
					break;
				}
				else
				{
					SDLoc = SDLoopStart;
//					pChan->lBufferAddress = -1;
				}
			}

			// Load the new sample
			asm("mul.ss %2, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %0\n mul.ss %3, %4, w2\n rrc w3, w3\n rrc w2, w2\n rrc w3, w3\n rrc w2, %1" :
				"=r"(iTmpLFT), "=r"(iTmpRGT) : 
				"r"(iVolLeft), "r"(iVolRight), "r"(SD_GetSample(SDLoc)) :
				"w2", "w3");
//			iTmpLFT = (iVolLeft * iSample) >> 16; 
//			iTmpRGT = (iVolRight * iSample) >> 16; 
		}

/*		if(pBuffer >= iPlay + PLAYBUFSIZE)
		{
			pBuffer = iPlay;
		}
*/
	}

	pChan->SDLoc = SDLoc;
	pChan->iLocFract = iSampleLocFract;
}

#define SCALEBITS 2
void PostProcess(unsigned int iLoc, unsigned int iLength)
{
	int* p = &iPlay[iLoc];
	unsigned int i;

	for(i = 0; i < (iLength << 1); ++i)
	{
		unsigned int iValue = *p;

/*		//if(((iValue & 0x8000) == 0) &&
		//	(iValue > (127 << (8 - SCALEBIT))))
*/
		if((iValue & 0x8000) == 0)
		{
			if((iValue & 0x6000))
			{
				*p = 0x1FF;
			}
			else
			{
				*p = ((iValue << SCALEBITS) + 0x8000) >> 7;
			}
		}
		else
		{
			if((iValue & 0x6000) == 0x6000)
			{
				*p = ((iValue << SCALEBITS) + 0x8000) >> 7;
			}
			else
			{
				*p = 0;
			}
		}

		p++;
	}
}

#ifdef RAW_PLAYER

unsigned int iSDLoc = SD_SIZE;
unsigned long SD_Addr = 0;
unsigned int DoPlay(void)
{
	unsigned int i = iPlayLoc;
	unsigned int iLength;
	unsigned char status = 0;

	if(SD_BlockLen != SD_SIZE)
	{
		SD_SetBlockLen(SD_SIZE);
	}

	if(iSDLoc >= SD_SIZE)
	{
		// Need to load a new block from the SD Card
		status = SD_StartRead(SD_Addr);
		if(status != 0)
		{
			// Maybe we hit the end of the SD Card? reset address and try again.
			SD_Addr = 0;
			status = InitSD();
			status = SD_SetBlockLen(SD_SIZE);
			status = SD_StartRead(SD_Addr);
		}
		SD_Addr += SD_SIZE;
		iSDLoc = 0;
	}

	if(iMixLoc == i)
	{
		return 0;		// ABORT: mixing buffer is already full
	}

	if(iMixLoc > i)
	{
		iLength = ((PLAYBUFSIZE - iMixLoc) + i);
	}
	else
	{
		iLength = (i - iMixLoc);
	}

	if((iMixLoc + iLength) > PLAYBUFSIZE)
	{
		// Lets make sure the mixing loop doesn't have to
		// cross the end of the play buffer boundary
		iLength = (PLAYBUFSIZE - iMixLoc);
	}
/*
	else if(iLength < (PLAYBUFSIZE / 4))
	{
		// Don't bother doing anything if we don't
		// have enough space to do much.
		return 0;
	}
*/

	if(iLength > (SD_SIZE - iSDLoc))
	{
		iLength = SD_SIZE - iSDLoc;
	}
	
	if(SD_Location < (iSDLoc + iLength))
	{
		// Not finished reading SD Card, so can't copy that far ahead yet.
		iLength = SD_Location;
	}

	for(i = 0; i < iLength; ++i)
	{
		iPlay[iMixLoc++] = ((int)SD_Buffer[iSDLoc++]) << 1;

		if(iMixLoc >= PLAYBUFSIZE)
		{
			iMixLoc = 0;
		}
	
		if(iSDLoc >= SD_SIZE)
		{
			break;
		}
	}

	return iLength;
}
#endif

unsigned int DoMix(void)
{
	unsigned int i;
	unsigned int iLength;
	unsigned int bFirstPass = 1;
	unsigned int bFrameProcess = 0;

	i = iPlayLoc;
	if(iMixLoc == i)
	{
		return 0;		// ABORT: mixing buffer is already full
	}

	if(iMixLoc > i)
	{
		iLength = ((PLAYBUFSIZE - iMixLoc) + i) >> 1;
	}
	else
	{
		iLength = (i - iMixLoc) >> 1;
	}

	if(iMixLoc + (iLength * 2) > PLAYBUFSIZE)
	{
		// Lets make sure the mixing loop doesn't have to
		// cross the end of the play buffer boundary
		iLength = (PLAYBUFSIZE - iMixLoc) >> 1;
	}
	else if(iLength < (PLAYBUFSIZE / 4) && SD_CacheLoad != -1)
	{
		// Don't bother doing anything if we don't
		// have enough space to do much and the SD Card is busy reading...
		return 0;
	}

	if(iLength > MAX_MIX_CHUNK)
	{
		iLength = MAX_MIX_CHUNK;
	}

	if(iLength >= iFrameCounter)
	{
		bFrameProcess = 1;

		iLength = iFrameCounter;
		iFrameCounter = iFramePeriod;
	}
	else
	{
		iFrameCounter -= iLength;
	}

	for(i = 0; i < CHANNELS; ++i)
	{
		ChannelType *pChannel = &oChannel[i];
		if(!pChannel->bActive)
		{
			continue;
		}

		if(bFirstPass)
		{
			WriteChannel(pChannel, i, iMixLoc, iLength);
			bFirstPass = 0;
		}
		else
		{
			MixChannel(pChannel, i, iMixLoc, iLength);
		}
	}

	PostProcess(iMixLoc, iLength);

	if(bFirstPass)
	{
		// None of channels are active, so we must manually 
		// clear the mix buffer of old sample data.

		for(i = 0; i < iLength; ++i)
		{
			iPlay[iMixLoc++] = 0;
			iPlay[iMixLoc++] = 0;
	
			if(iMixLoc >= PLAYBUFSIZE)
			{
				iMixLoc = 0;
			}
		}
	}
	else
	{
		iMixLoc = iMixLoc + (iLength * 2);
		if(iMixLoc == PLAYBUFSIZE)
		{
			iMixLoc = 0;
		}
/*
		if(iMixLoc >= PLAYBUFSIZE)
		{
			iMixLoc -= PLAYBUFSIZE;
		}*/
	}

	return bFrameProcess;
}

/*
int GetSample(unsigned int iPtr)
{
	unsigned int iTmp;
	unsigned int iRealPtr = iPtr - (iPtr / 3);
	unsigned int iByte = iPtr % 3;

	if(iByte == 0)
	{
		asm("tblrdl [%0], %1" : "=r"(iTmp) : "r"(iRealPtr));
		iTmp &= 0xFF;
	}
	else if(iByte == 1)
	{
		asm("tblrdl [%0], %1" : "=r"(iTmp) : "r"(iRealPtr - 1));
		iTmp >>= 8;
	}
	else
	{
		asm("tblrdh [%0], %1" : "=r"(iTmp) : "r"(iRealPtr - 2));
	}

	if(iTmp & 0x80)
	{
		// sign extend
		iTmp |= 0xFF00;
	}
	return iTmp;
}
*/
/*
int GetInterpolatedSample(unsigned int iPtr, unsigned int iPtrFract)
{
	int iVal1; //, iVal2;
	unsigned int iTmp;
	unsigned int iRealPtr = iPtr - (iPtr / 3);
	unsigned int iByte = iPtr % 3;

	if(iByte == 0)
	{
		asm("tblrdl [%0], %1" : "=r"(iTmp) : "r"(iRealPtr));
		iVal1 = (iTmp & 0xFF) - 128;
//		iVal2 = (iTmp >> 8) - 128;
	}
	else if(iByte == 1)
	{
		asm("tblrdl [%0], %1" : "=r"(iTmp) : "r"(iRealPtr - 1));
		iVal1 = (iTmp >> 8) - 128;
//		asm("tblrdh [%0], %1" : "=r"(iTmp) : "r"(iRealPtr - 1));
//		iVal2 = iTmp - 128;
	}
	else
	{
		asm("tblrdh [%0], %1" : "=r"(iTmp) : "r"(iRealPtr - 2));
		iVal1 = iTmp - 128;
//		asm("tblrdl [%0], %1" : "=r"(iTmp) : "r"(iRealPtr));
//		iVal2 = (iTmp & 0xFF) - 128;
	}

//	iVal2 = ((long long) iVal2 * (unsigned long long)iPtrFract) >> 16;
//	iVal1 = iVal1 * (1 - iPtrFract / 65535);
//	iVal1 = iVal1 - (((long long)iVal1 * (unsigned long long)iPtrFract) >> 16);
//	iVal1 += iVal2;
	return iVal1;
}
*/

/**
 * Initializes the mixing system.
 *
 * @author edwards
 * @date 7/10/2005
 */
void InitMixer(void)
{
	unsigned int i;

	// First, clear out the play buffer so that it contains silence.
	for(i = 0; i < PLAYBUFSIZE; ++i)
	{
		iPlay[i] = 0x80;
	}

	// Now set all of the channels as non-active.
	for(i = 0; i < CHANNELS; ++i)
	{
		oChannel[i].iLocFract = 0;
		oChannel[i].iStep = 0;
		oChannel[i].iStepFract = 0;
		oChannel[i].iEffect = 0;
		oChannel[i].bActive = 0;
		oChannel[i].bLoop = 0;
		oChannel[i].bBiDi = 0;
		oChannel[i].b16 = 0;
		oChannel[i].bStereo = 0;
		oChannel[i].iBuffer = -1;
		oChannel[i].pBuffer = -1;
		oChannel[i].lBufferAddress = -1;

		if(i == 0 || i == 3)
		{
			oChannel[i].iPan = 0x10;
		}
		else
		{
			oChannel[i].iPan = 0x70;
		}
		SetVolume(&oChannel[i], 64);
	}
}
