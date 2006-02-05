// -*- tab-width: 4 -*-
// MOD Loader for TRAXMOD Player
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/


#ifndef MOD_H
#define MOD_H

#include <string>

#include "pattern.h"
#include "row.h"
#include "cmd.h"

using namespace std;

class mod
{
 public:
  mod(char* sFileName);
  ~mod();

  void writeSamples(void);
  void writeMusic(void);

  const static double C2SPD[16];

  char Title[20];
  typedef struct
  {
	char Name[22];
	unsigned short int Length;
	char Finetune;
	unsigned char Volume;
	short int LoopStart;
	short int LoopLength;
  } SampleType;

  SampleType Samples[32];

  unsigned char NumOrders;
  unsigned char OrderTable[128];
  char Format[4];

  unsigned char *Patterns;

  char *SampleData[31];

  unsigned char NumPatterns;
  unsigned int NumSamples;

  Pattern* oPat;

 protected:
  int iCurrentChannel;
  int iNumChannels;

  Row ProcessRow(int iPattern, int iRow);

  Pattern ProcessPattern(int iPattern);

  int calcWait(int iPat, int iRow);

 private:

};

#endif // MOD_H
