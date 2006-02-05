// -*- tab-width: 4 -*-
// dsPIC Mixer
//
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/

#ifndef MIXER_H
#define MIXER_H

// NOTE: Must also change isr.s for new value.
#define PLAYBUFSIZE 448
//#define PLAYBUFSIZE 576

#define CHANNELS 	4
#define MAXVOLUME 64
#define MAX_MIX_CHUNK 24

typedef struct
{
	unsigned int iSample;
	int iStep;
	int iStepFract;
	unsigned int iLocFract;
	int iVolLeft;
	int iVolRight;
	unsigned int iEffect;
	unsigned bActive	:1;
	unsigned bLoop  	:1;
	unsigned bBiDi		:1;
	unsigned b16		:1;
	unsigned bStereo	:1;
	unsigned bSD		:1;

	unsigned int iVolume;
	unsigned int iPan;
	unsigned int iPeriod;
	unsigned int iPortaDest;
	unsigned int iPortaSpeed;
	unsigned long SDLoc;
	unsigned long SDLoopStart;
	unsigned long SDEnd;

	unsigned int iBuffer;
	unsigned char* pBuffer;
	unsigned long lBufferAddress;
} ChannelType;

void InitMixer(void);
unsigned int DoMix(void);
unsigned int DoPlay(void);

void SetSample(ChannelType* oChan, unsigned int iSample);
int GetSample(unsigned int iPtr);

extern int iPlay[PLAYBUFSIZE];
extern ChannelType oChannel[CHANNELS];
extern volatile unsigned int iPlayLoc;
extern unsigned int iFrameCounter;
extern unsigned int iFramePeriod;
extern unsigned int iMasterVolumeLeft;
extern unsigned int iMasterVolumeRight;


#endif // MIXER_H
