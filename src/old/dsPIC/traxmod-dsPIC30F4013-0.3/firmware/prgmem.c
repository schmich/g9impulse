// -*- tab-width: 4 -*-
// Program Memory Data Retrieval
// Copyright (c) 2005, K9spud LLC.

unsigned char getByte(unsigned int iPtr)
{
	unsigned int iResult;
	unsigned int iRealPtr = iPtr - (iPtr / 3);
	unsigned int iByte = iPtr % 3;

	if(iByte == 0)
	{
		asm("tblrdl [%0], %1" : "=r"(iResult) : "r"(iRealPtr));
		iResult = iResult & 0xFF;
	}
	else if(iByte == 1)
	{
		asm("tblrdl [%0], %1" : "=r"(iResult) : "r"(iRealPtr - 1));
		iResult = iResult >> 8;
	}
	else
	{
		asm("tblrdh [%0], %1" : "=r"(iResult) : "r"(iRealPtr - 2));
	}

	return iResult;
}
