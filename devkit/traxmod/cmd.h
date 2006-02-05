// -*- tab-width: 4 -*-
// Mixer Engine Commands
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/


#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

class cmd
{
 public:
  cmd(int iChannel = DontCare);
  cmd(const cmd& oCmd);
  ~cmd();

  void operator=(const cmd& oCmd);

  unsigned char* getData(void);
  void nextChannel(void);

  friend ostream& operator<<(ostream& out, const cmd& oCmd);

  enum ChannelType { DontCare = -1 };

  int Channel;
  int Length;
  unsigned char* Data;

  enum 
  { 
    Note, Wait, SetChannel, WaitSetChannel, WaitSetChannelLong, 
    Effect, PatternBreak, Unknown 
  } Command;

  bool IncrementChannel;

 protected:

  void init(void);
  void copy(const cmd& oCmd);

 private:

};

ostream& operator<<(ostream& out, const cmd& oCmd);

typedef vector<cmd> cmdList;

class cmdNote : public cmd
{
 public:
	cmdNote(int iPeriod, int iSample, int iVolume, int iChannel = DontCare);
	cmdNote(const cmdNote& oCmd);

	int Period;
	int Volume;
	int Sample;
};

class cmdEffect : public cmd
{
 public:
	cmdEffect(int iEffect, int iChannel = DontCare);
	cmdEffect(const cmdEffect& oCmd);

	int Effect;
};


class cmdSetChannel : public cmd
{
 public:
  cmdSetChannel(int iChannel);

  int getChannel(void);
};

class cmdWait : public cmd
{
 public:
  cmdWait(int iWaitRows);
  cmdWait(const cmdWait& oCmd);

  int Wait;
};

class cmdWaitSetChannel : public cmd
{
 public:
  cmdWaitSetChannel(int iWait, int iChannel);
  cmdWaitSetChannel(const cmdWaitSetChannel& oCmd);

  int Wait;
};

class cmdPatternBreak : public cmd
{
 public:
  cmdPatternBreak(int iBreakToRow);

  int BreakToRow;
};

class cmdPortaNote : public cmd
{
 public:
	cmdPortaNote(int iDestination, int iSpeed, int iSample, int iChannel = DontCare);
	cmdPortaNote(const cmdPortaNote& oCmd);

	int Destination;
	int Speed;
	int Sample;
};

#endif // CMD_H
