// -*- tab-width: 4 -*-
// Row Container Object
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/

#include "stdafx.h"
#include <typeinfo>
#include "row.h"

ostream& operator<<(ostream& out, Row& oRow)
{
  for(cmdList::iterator it = oRow.vRow.begin(); it != oRow.vRow.end(); ++it)
	{
	  out << *it;
	}

  for(cmdList::iterator it = oRow.vEnd.begin(); it != oRow.vEnd.end(); ++it)
	{
	  out << *it;
	}  

  return out;
}

Row::Row()
{
  bNextChannel = false;
  iCurrentChannel = 0;
}

Row::~Row()
{
}

cmd& Row::operator[](int i)
{
  if(i >= vRow.size())
	{
	  return vEnd[i - vRow.size()];
	}
  else
	{
	  return vRow[i];
	}
}

void Row::add(cmd oCmd)
{
  if(oCmd.Channel != cmd::DontCare && oCmd.Channel != iCurrentChannel)
  {
    // We need to signal a new channel number before adding this
	// command
	if(iCurrentChannel == (oCmd.Channel - 1) &&
	   oCmd.IncrementChannel == false)
	  {
		// Just do a one channel increment bit flip.
		oCmd.nextChannel();
		iCurrentChannel++;
	  }
	else
	  {
		iCurrentChannel = oCmd.Channel;
		add(cmdSetChannel(iCurrentChannel));
	  }
  }

  vRow.push_back(oCmd);
}

void Row::addFront(cmd oCmd)
{
  if(oCmd.Channel != cmd::DontCare && oCmd.Channel != 0)
  {
    // We need to signal a new channel number before adding this
	// command
	if(oCmd.Channel == 1 && oCmd.IncrementChannel == false)
	  {
		// Just do a one channel increment bit flip.
		oCmd.nextChannel();
	  }
	else
	  {
		addFront(cmdSetChannel(oCmd.Channel));
		vRow.insert(vRow.begin()++, 1, oCmd);
		return;
	  }
  }

  vRow.insert(vRow.begin(), 1, oCmd);
}

void Row::addFront(cmdWait oCmd)
{
  if(vRow[0].Command == cmd::SetChannel)
	{
	  cmdSetChannel* oSetChannel = (cmdSetChannel *) &vRow[0];

	  int iSetChannel = oSetChannel->getChannel();
	  switch(iSetChannel)
	  {
	    case 0:
		  // This set channel command is going to be unnecessary, so
		  // we can overwrite it with the cmdWait.
		  vRow[0] = oCmd;
		  return;
		
	    default:
		  // Otherwise, we have to do something else
		  if(oCmd.Wait < 4 && iSetChannel < 4)
			{
			  // We can substitute a cmdWaitSetChannel command instead
			  vRow[0] = cmdWaitSetChannel(oCmd.Wait, iSetChannel);
			  return;
			}
		  break;
	  }
	}
  else
	{
	  if(oCmd.Wait < 4)
		{
		  // We can substitute the smaller cmdWaitSetChannel
		  addFront(cmdWaitSetChannel(oCmd.Wait, 0));
		  return;
		}
	}

  addFront((cmd) oCmd);
}

void Row::addRear(cmd oCmd)
{
  vEnd.push_back(oCmd);
}

void Row::nextChannel(void)
{
  bNextChannel = true;
}

bool Row::IsEmpty(void)
{
  return (vRow.empty() && vEnd.empty());
}

bool Row::patternBreak(void)
{
  // Search for a pattern break command in this row.
  for(cmdList::iterator it = vRow.begin(); it != vRow.end(); ++it)
	{
	  if(it->Command == cmd::PatternBreak)
		{
		  return true;
		}
	}

  for(cmdList::iterator it = vEnd.begin(); it != vEnd.end(); ++it)
	{
	  if(it->Command == cmd::PatternBreak)
		{
		  return true;
		}
	}

  return false;
}

int Row::length(void)
{
  int iLength = 0;
  for(cmdList::iterator it = vRow.begin(); it != vRow.end(); ++it)
  {
    iLength += it->Length;
  }

  for(cmdList::iterator it = vEnd.begin(); it != vEnd.end(); ++it)
  {
    iLength += it->Length;
  }

  return iLength;
}

void Row::getData(unsigned char* pBuffer)
{
  int i = 0;

  for(cmdList::iterator it = vRow.begin(); it != vRow.end(); ++it)
	{
	  for(int j = 0; j < it->Length; ++j)
		{
		  pBuffer[i++] = it->Data[j];
		}
	}

  for(cmdList::iterator it = vEnd.begin(); it != vEnd.end(); ++it)
	{
	  for(int j = 0; j < it->Length; ++j)
		{
		  pBuffer[i++] = it->Data[j];
		}
	}
}