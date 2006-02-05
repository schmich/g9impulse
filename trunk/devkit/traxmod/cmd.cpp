// -*- tab-width: 4 -*-
// Mixer Engine Commands
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/

#include "stdafx.h"
#include <string.h>
#include "cmd.h"

cmd::cmd(int iChannel)
{
  init();

  Channel = iChannel;
}

cmd::cmd(const cmd& oCmd)
{
  copy(oCmd);
}

cmd::~cmd()
{
  if(Data != NULL)
	{
	  delete Data;
	  init();
	}
}

void cmd::operator=(const cmd& oCmd)
{
  copy(oCmd);
}

void cmd::copy(const cmd& oCmd)
{
  if(oCmd.Data != NULL && oCmd.Length != 0)
	{
	  Data = new unsigned char[oCmd.Length];
	  memcpy(Data, oCmd.Data, oCmd.Length);
	  Length = oCmd.Length;
	  Command = oCmd.Command;
	  Channel = oCmd.Channel;
	  IncrementChannel = oCmd.IncrementChannel;
	}
  else
	{
	  init();
	}
}

void cmd::init(void)
{
  Channel = DontCare;
  Data = NULL;
  Length = 0;
  IncrementChannel = false;
  Command = Unknown;
}

unsigned char* cmd::getData(void)
{
  if(Data == NULL)
	return NULL;
  
  if(IncrementChannel)
	{
	  Data[0] |= 0x80;
	}
  
  return Data;
}

void cmd::nextChannel(void)
{
  IncrementChannel = true;
  if(Data != NULL)
  {
    Data[0] |= 0x80;
  }
}

ostream& operator<<(ostream& out, const cmd& oCmd)
{
  if(oCmd.Length == 0 || oCmd.Data == NULL)
	{
	  out << "(null)";
	}
  
  else
	{
	  for(int i = 0; i < oCmd.Length; ++i)
		{
		  out << hex << setw(2) << (unsigned int) oCmd.Data[i];
		}
	}

  out << " ";
  
  return out;
}

// ----------------------------------------------------------------------------

cmdNote::cmdNote(int iPeriod, int iSample, int iVolume, int iChannel)
{
  Length = 4;
  Command = cmd::Note;
  Data = new unsigned char[Length];
  
  Data[0] = 1 << 4 | (iPeriod & 0x0F00) >> 8;
  Data[1] = iPeriod & 0xFF;
  Data[2] = iSample & 0xFF;
  Data[3] = iVolume & 0xFF;
  
  Period = iPeriod;
  Sample = iSample;
  Volume = iVolume;
  Channel = iChannel;
}

cmdNote::cmdNote(const cmdNote& oCmd)
{
  copy(oCmd);
  Period = oCmd.Period;
  Volume = oCmd.Volume;
  Sample = oCmd.Sample;
}

// ----------------------------------------------------------------------------

cmdEffect::cmdEffect(int iEffect, int iChannel)
{
  Length = 2;
  Command = cmd::Effect;
  Data = new unsigned char[Length];
  
  Data[0] = 2 << 4 | (iEffect & 0x0F00) >> 8;
  Data[1] = iEffect & 0xFF;

  Effect = iEffect;
  Channel = iChannel;
}

cmdEffect::cmdEffect(const cmdEffect& oCmd)
{
  copy(oCmd);
  Effect = oCmd.Effect;
}

// ----------------------------------------------------------------------------

cmdSetChannel::cmdSetChannel(int iChannel)
{
  if(iChannel <= 0xF)
	{
	  Length = 1;
	  Command = cmd::SetChannel;
	  Data = new unsigned char[Length];
	  
	  Data[0] = 3 << 4 | (iChannel & 0x0F);
	}
  else
	{
	  Length = 2;
	  Command = cmd::SetChannel;
	  Data = new unsigned char[Length];
	  
	  Data[0] = 2 << 4;
	  Data[1] = iChannel & 0xFF;
	}

  Channel = iChannel;
}

int cmdSetChannel::getChannel(void)
{
  if(Length == 1)
	{
	  return(Data[0] & 0x0F);
	}
  else if(Length == 2)
	{
	  return(Data[1] & 0xFF);
	}
}

// ----------------------------------------------------------------------------

cmdWait::cmdWait(int iWaitRows)
{
  Length = 2;
  Command = cmd::Wait;
  Data = new unsigned char[Length];
  
  Data[0] = 0x2E;
  Data[1] = 0xE0 | (iWaitRows & 0x0F);

  Wait = iWaitRows;
}


cmdWait::cmdWait(const cmdWait& oCmd)
{
  copy(oCmd);
  Wait = oCmd.Wait;
}

// ----------------------------------------------------------------------------

cmdWaitSetChannel::cmdWaitSetChannel(int iWaitRows, int iChannel)
{
  if(iChannel > 3)
  {
    Length = 2;
    Command = cmd::WaitSetChannelLong;
    Data = new unsigned char[Length];
  
    Data[0] = 0xB << 4 | ((iWaitRows & 0x3) << 2) | (iChannel & 0x3);
    Data[1] = 0x3 << 4 | ((iChannel >> 2) & 0x0F);
  }
  else
  {
    Length = 1;
    Command = cmd::WaitSetChannel;
    Data = new unsigned char[Length];
  
    Data[0] = 0xB << 4 | ((iWaitRows & 0x3) << 2) | (iChannel & 0x3);
  }

  Wait = iWaitRows;
  Channel = iChannel;
}


cmdWaitSetChannel::cmdWaitSetChannel(const cmdWaitSetChannel& oCmd)
{
  copy(oCmd);
  Wait = oCmd.Wait;
}

// ----------------------------------------------------------------------------

cmdPatternBreak::cmdPatternBreak(int iBreakToRow)
{
  Length = 2;
  Command = cmd::PatternBreak;
  Data = new unsigned char[Length];
  
  Data[0] = 2 << 4 | 0x0D;
  Data[1] = iBreakToRow & 0xFF;

  BreakToRow = iBreakToRow;
}

// ----------------------------------------------------------------------------

cmdPortaNote::cmdPortaNote(int iDestination, int iSpeed, int iSample, int iChannel)
{
  Length = 4;
  Command = cmd::Effect;
  Data = new unsigned char[Length];
  
  Data[0] = 4 << 4 | (iDestination & 0x0F00) >> 8;
  Data[1] = iDestination & 0xFF;
  Data[2] = iSpeed & 0xFF;
  Data[3] = iSample & 0xFF;

  Destination = iDestination;
  Speed = iSpeed;
  Sample = iSample;
  Channel = iChannel;
}

cmdPortaNote::cmdPortaNote(const cmdPortaNote& oCmd)
{
  copy(oCmd);
  Destination = oCmd.Destination;
  Speed = oCmd.Speed;
}