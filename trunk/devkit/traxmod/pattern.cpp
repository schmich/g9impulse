// -*- tab-width: 4 -*-
// Pattern Container Object
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/

#include "stdafx.h"
#include "pattern.h"

Pattern::Pattern()
{
  NumRows = 64;

  iCurrentRow = 0;
  iCurrentCmd = 0;
  iCurrentByte = 0;
  bEOF = true;
}

Pattern::~Pattern()
{
}

Row& Pattern::operator[](int i)
{
  return oRows[i];
}

int Pattern::length(void)
{
  int iLength = 0;

  for(int i = 0; i < NumRows; ++i)
  {
    iLength += oRows[i].length();    
  }

  return iLength;
}

void Pattern::getData(unsigned char* pBuffer)
{
  int i = 0;

  for(int j = 0; j < NumRows; ++j)
  {
	int iRowLength = oRows[j].length();
	if(iRowLength > 0)
	  {
		oRows[j].getData(pBuffer);
		pBuffer += iRowLength;
	  }
  }
}