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
// $Log: sdcard.c,v $
// Revision 1.13  2005/09/28 04:56:44  edwards
// More optimization attempts.
//
// Revision 1.11  2005/09/27 06:08:51  edwards
// Trying some various changes to the prefetch timing.
//
// Revision 1.10  2005/09/26 05:52:11  edwards
// Playing all sample data out of SD Card! Still a few glitches on tight sample
// loops though (due to SD Card block misalignment).
//
// Revision 1.9  2005/09/26 03:47:08  edwards
// Caching improvements.
//
// Revision 1.6  2005/09/18 09:09:20  edwards
// SPI Interrupt driven SD Card command write working.
//
// Revision 1.4  2005/09/16 05:15:41  edwards
// Trying to get end of card to behave gracefully.
//
// Revision 1.3  2005/09/16 05:01:39  edwards
// Now working with only 64 byte block length on SD Card reads.
//
// Revision 1.1  2005/09/15 08:29:48  edwards
// Plays raw audio data at 43Hz, 8 bits, stereo from my SD Card now, but
// only seems to work when running under the ICD. Something seriously
// wrong with timing or something somewhere.
//
// Revision 1.2  2005/09/09 04:26:23  edwards
// Comments updated.
//
// Revision 1.1  2005/09/07 07:07:18  edwards
// First successful read from SD Card on my dsPIC30F4013!

#include "p30f4013.h"
#include "sdcard.h"

unsigned int SD_BlockLen = 512;

void SPIWrite(unsigned char data)
{
	// DO NOT WAIT FOR SPITBF TO BE CLEAR HERE
	// (for some reason, it doesn't work on this side of the write data).

	// Write the data!
	SPI1BUF = data;

	// Wait until send buffer is ready for more data.
	while(SPI1STATbits.SPITBF);
}

unsigned char SPIRead(void)
{
	unsigned char data;

	__asm__ volatile("disi #0x1000"); /* disable interrupts */


	if(SPI1STATbits.SPIRBF)
	{
		// already have some data to return, don't initiate a read
		data = SPI1BUF;

		SPI1STATbits.SPIROV = 0;
		__asm__ volatile("disi #0x0000"); /* enable interrupts */

		return data;
	}

	// We don't have any data to read yet, so initiate a read
	SPI1BUF = 0xFF;  // write dummy data to initiate an SPI read
	while(SPI1STATbits.SPITBF); // wait until the data is finished reading
	data = SPI1BUF;

	__asm__ volatile("disi #0x0000"); /* enable interrupts */


	SPI1STATbits.SPIROV = 0;
	return data;
}

void InitSPI(void)
{
	SD_PowerOff();
	SD_PWR_DIR = 0;	 // output
	SD_PowerOff();

	SD_Disable();
	SD_CS_DIR = 0; // output
	SD_Disable();

	SDI_DIR = 1; // input
	SCK_DIR = 1;
	SDO_DIR = 1;

	SPI1STAT = 0x8000; // enable SPI port

	// set SPI port to slowest setting
	// master mode
	// 8 bit
	// Idle state for Clock is high level
	// Primary prescaler 64:1
	// Secondary prescaler 8:1
	SPI1CON = 0x0060;

	while(SPI1STATbits.SPIRBF)
	{
		// already have some data, read from SPI1BUF to clear it out
		asm("cp	_SPI1BUF");
//		unsigned char data = SPI1BUF;
	}

	SPI1STATbits.SPIROV = 0;
}

// SD Card SPI Commands are defined in the SanDisk SD Card Product Manual v1.9 
// section 5.2.2.1 (page 91 of PDF)
// http://www.sandisk.com/pdf/oem/ProdManualSDCardv1.9.pdf
//
// 0 - GO_IDLE_STATE		Resets the SD Card
// 1 - SEND_OP_COND 		Activates the card's initialization process
// 9 - SEND_CSD				Asks card to send card-specific data
// 10 - SEND_CID			Asks card to send card identification
// 12 - STOP_TRANSMISSION	Forces card to stop transmission during multi-block read
// 13 - SEND_STATUS			Asks card to send its status register.
// 16 - SET_BLOCKLEN		Selects block length for all subsequent block commands (default is 512)
// 17 - READ_SINGLE_BLOCK	Reads a block of the size specified by SET_BLOCKLEN
// 18 - READ_MULTIPLE_BLOCK	Continuously transfers data until interrupted by STOP_TRANSMISSION
// 24 - WRITE_BLOCK			Writes a block of the size specified by SET_BLOCKLEN
// 25 - WRITE_MULTI_BLOCK 	Continuously writes blocks of data until a stop transmission token is sent instead of start block token.

// This is the only command required to have a valid CRC
// After this command, CRC values are ignore unless explicitly enabled using CMD59
unsigned char CMD0_GO_IDLE_STATE[] = {0x00,0x00,0x00,0x00,0x00,0x95};
unsigned char CMD1_SEND_OP_COND[] = {0x01,0x00,0x00,0x00,0x00,0xFF};
unsigned char CMD55_APP_CMD[] = {55,0x00,0x00,0x00,0x00,0xFF};
unsigned char ACMD41_SD_SEND_OP_COND[] = {41,0x00,0x00,0x00,0x00,0xFF};

// R1 Response Codes (from SD Card Product Manual v1.9 section 5.2.3.1)
#define R1_IN_IDLE_STATE    (1<<0)   // The card is in idle state and running initializing process.
#define R1_ERASE_RESET      (1<<1)   // An erase sequence was cleared before executing because of an out of erase sequence command was received.
#define R1_ILLEGAL_COMMAND  (1<<2)   // An illegal command code was detected
#define R1_COM_CRC_ERROR    (1<<3)   // The CRC check of the last command failed.
#define R1_ERASE_SEQ_ERROR  (1<<4)  // An error in the sequence of erase commands occured.
#define R1_ADDRESS_ERROR    (1<<5)  // A misaligned address, which did not match the block length was used in the command.
#define R1_PARAMETER        (1<<6)  // The command's argument (e.g. address, block length) was out of the allowed range for this card.
// R1 bit (1<<7) is always 0

unsigned char SDWriteCommand(CommandType* cmd)
{
	unsigned int i;
	unsigned char response;
	unsigned char savedSD_CS = SD_CS;

	// SD Card Command Format
	// (from Section 5.2.1 of SanDisk SD Card Product Manual v1.9).
	// Frame 7 = 0
	// Frame 6 = 1
	// Command (6 bits)
	// Address (32 bits)
	// Frame 0 = 1

	SD_Enable();

	// Set the framing bits correctly (never change)
	cmd->cmd  = (cmd->cmd | (1<<6)) & ~(1<<7);
	cmd->crc |= (1<<0);
	
	// Send the 6 byte command
	SPIWrite(cmd->cmd);
	SPIWrite(cmd->addr[3]);
	SPIWrite(cmd->addr[2]);
	SPIWrite(cmd->addr[1]);
	SPIWrite(cmd->addr[0]);
	SPIWrite(cmd->crc);

	// Wait for the response
	for(i = 0; i < 10000; ++i)
	{
		response = SPIRead();
		if(response != 0xFF)
		{
			break;
		}
	}

	SD_Disable();

	// Following any command, the SD Card needs 8 clocks to finish up its work.
	// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)
	SPIWrite(0xFF); 

	SD_CS = savedSD_CS;
	return(response);
}

unsigned char SD_WriteCommand(unsigned char* cmd)
{
	unsigned int i;
	unsigned char response;
	unsigned char savedSD_CS = SD_CS;

	// SD Card Command Format
	// (from Section 5.2.1 of SanDisk SD Card Product Manual v1.9).
	// Frame 7 = 0
	// Frame 6 = 1
	// Command (6 bits)
	// Address (32 bits)
	// Frame 0 = 1

	SD_Enable();

	// Set the framing bits correctly (never change)
	cmd[0] |= (1<<6);
	cmd[0] &= ~(1<<7);
	cmd[5] |= (1<<0);
	
	// Send the 6 byte command
	for(i = 0; i < 6; ++i)
	{
		SPIWrite(*cmd);
		cmd++;
	}

	// Wait for the response
	for(i = 0; i < 10000; ++i)
	{
		response = SPIRead();
		if(response != 0xFF)
		{
			break;
		}
	}

	SD_Disable();

	// Following any command, the SD Card needs 8 clocks to finish up its work.
	// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)
	SPIWrite(0xFF); 

	SD_CS = savedSD_CS;
	return(response);
}

unsigned char InitSD()
{
	unsigned int i = 0;
	unsigned char status;
	unsigned char ResultCode = 0;

	// Turn off SD Card
	SD_Disable();
	SD_PowerOff();

	// Wait for power to really go down
	for(i = 0; i; i++);
	for(i = 0; i; i++);
	for(i = 0; i; i++);
	for(i = 0; i; i++);

	SD_CacheLoad = -1;
	SD_Buffer = SD_Cache[0];
	SD_BufferEnd = SD_Buffer + SD_SIZE;
	SD_State = 0;
	for(i = 0; i < (CHANNELS*2); ++i)
	{
		SD_CacheAddr[i] = -1;
	}

	for(i = 0; i < CHANNELS; ++i)
	{
		SD_PriReq[i] = -1;
		SD_SecReq[i] = -1;
	}

	// Turn on SD Card
	SD_PowerOn();

	// Wait for power to really come up
	for(status = 0; status < 10; ++status)
	{
		for(i = 0; i; i++);
		for(i = 0; i; i++);
		for(i = 0; i; i++);
		for(i = 0; i; i++);
	}

	// We need to give SD Card about a hundred clock cycles to boot up
	for(i = 0; i < 16; ++i)
	{
		SPIWrite(0xFF); // write dummy data to pump clock signal line
	}	

	SD_Enable();

	for(i = 0; i; i++);

	// Wait for the SD Card to go into IDLE state
	i = 0;
	do
	{
		status = SD_WriteCommand(CMD0_GO_IDLE_STATE);

		// fail and return
		if(i++ > 1000)
		{
			SD_Disable();
			return 1;
		}
	} while( status != 0x01 );

	// Wait for SD Card to initialize
	i = 0;
	do
	{
		status = SD_WriteCommand(CMD1_SEND_OP_COND);
		if(i++ > 1000)
		{
			SD_Disable();
			return 2;
		}
	} while( (status & R1_IN_IDLE_STATE) != 0 );

    // Send CMD55, required to precede all "application specific" commands
	status = SD_WriteCommand(CMD55_APP_CMD); // Do not check response here

	// Send the ACMD41 command to initialize SD Card mode (not supported by MMC cards)
	i = 0;
	status = SD_WriteCommand(ACMD41_SD_SEND_OP_COND);
	// Might return 0x04 for Invalid Command if MMC card is connected
	if( (status & R1_IN_IDLE_STATE) != 0 )
	{
		ResultCode = 3;
	}

	// Set the SPI bus to full speed now that SD Card is initialized in SPI mode
	SD_Disable();
	SPIFastClock();

	SD_SetBlockLen(SD_SIZE);

	return ResultCode;
}

unsigned char CMD16_SET_BLOCKLEN[] = {16,0x00,0x00,0x00,0x00,0xFF};
unsigned char SD_SetBlockLen(unsigned int iBlockLen)
{
	unsigned char status;

	CMD16_SET_BLOCKLEN[1] = ((iBlockLen & 0xFF000000) >> 24);
	CMD16_SET_BLOCKLEN[2] = ((iBlockLen & 0x00FF0000) >> 16);
	CMD16_SET_BLOCKLEN[3] = ((iBlockLen & 0x0000FF00) >> 8);
	CMD16_SET_BLOCKLEN[4] = ((iBlockLen & 0x000000FF));

	status = SD_WriteCommand(CMD16_SET_BLOCKLEN);
	if(status != 0 )
	{
		// ABORT: invalid response for read single command
		return 1;
	}
	
	SD_BlockLen = iBlockLen;
	return 0;
}

/*
unsigned long SD_CurrentAddress = -1;
unsigned int SD_BlockTimer = 512;
unsigned char SD_Reading = 0;
unsigned char CMD18_READ_MULTI_BLOCK[] = {18,0x00,0x00,0x00,0x00,0xFF};
unsigned char SD_InitRead(unsigned long addr)
{
	unsigned char status;

	CMD18_READ_MULTI_BLOCK[1] = ((addr & 0xFF000000) >> 24);
	CMD18_READ_MULTI_BLOCK[2] = ((addr & 0x00FF0000) >> 16);
	CMD18_READ_MULTI_BLOCK[3] = ((addr & 0x0000FF00) >> 8);
	CMD18_READ_MULTI_BLOCK[4] = ((addr & 0x000000FF));

	SD_Enable();

	// Send the read command
	status = SD_WriteCommand(CMD18_READ_MULTI_BLOCK);
	if(status != 0 )
	{
		// ABORT: invalid response for read single command
		return 1;
	}
	
	// Now wait for the "Start Block" token	(0xFE)
	// (see SanDisk SD Card Product Manual v1.9 section 5.2.4. Data Tokens)
	do
	{
		status = SPIRead();
	} while(status != 0xFE);
	
	SD_CurrentAddress = addr;
	SD_BlockTimer = SD_BlockLen;
	SD_Reading = 1;
	return 0;
}

unsigned int SD_LastSample = 0;
*/
/*
unsigned int SD_GetSample(unsigned long lSampleLoc)
{
	unsigned char status = -1;

	if(lSampleLoc < SD_CurrentAddress || SD_Reading == 0)
	{
		// Have to seek backwards, abort current multi-read and
		// re-locate.
		SD_EndRead();
		SD_InitRead(lSampleLoc);
	}		
	else if( lSampleLoc == (SD_CurrentAddress-1) )
	{
		return SD_LastSample;
	}

	do
	{
		status = SPIRead();
		SD_LastSample = status;

		SD_CurrentAddress++;	
		SD_BlockTimer--;
		if(SD_BlockTimer == 0)
		{
			// Read CRC bytes
			status = SPIRead();
			status = SPIRead();

			SD_BlockTimer = SD_BlockLen;

			// Now wait for the "Start Block" token	(0xFE)
			// (see SanDisk SD Card Product Manual v1.9 section 5.2.4. Data Tokens)
			do
			{
				status = SPIRead();
			} while(status != 0xFE);
		}

	} while(SD_CurrentAddress < lSampleLoc);
	
	if(SD_LastSample & 0x80)
	{
		// sign extend
		SD_LastSample |= 0xFF00;
	}
	return SD_LastSample;
}
*/
/*
unsigned char CMD12_STOP_TRANSMISSION[] = {12,0x00,0x00,0x00,0x00,0xFF};
void SD_EndRead(void)
{
	unsigned char status;

	if(SD_Reading == 0)
	{
		// ABORT: SD Card not in the middle of a multi-read, so don't
		// try stopping transmission.
		return;
	}

	SD_Enable();

	status = SD_WriteCommand(CMD12_STOP_TRANSMISSION);
	SD_Reading = 0;

	do
	{
		status = SPIRead();
	} while(status == 0x00);

	SD_Disable();
}
*/

// SD Card defaults to 512 byte block size
unsigned char CMD17_READ_SINGLE_BLOCK[] = {17,0x00,0x00,0x00,0x00,0xFF};
CommandType CMD17 = { { 17, 0, 0xFF } };
unsigned char SD_ReadBlock(unsigned long addr, unsigned char *buf)
{
	unsigned int i;
	unsigned char status;
	/*
	CMD17_READ_SINGLE_BLOCK[1] = ((addr & 0xFF000000) >> 24);
	CMD17_READ_SINGLE_BLOCK[2] = ((addr & 0x00FF0000) >> 16);
	CMD17_READ_SINGLE_BLOCK[3] = ((addr & 0x0000FF00) >> 8);
	CMD17_READ_SINGLE_BLOCK[4] = ((addr & 0x000000FF));
*/
	CMD17.address = addr;

	SD_Enable();

	// Send the read command
	status = SDWriteCommand(&CMD17);
	if(status != 0 )
	{
		// ABORT: invalid response for read single command
		SD_Disable();
		return 1;
	}
	
	// Now wait for the "Start Block" token	(0xFE)
	// (see SanDisk SD Card Product Manual v1.9 section 5.2.4. Data Tokens)
	do
	{
		status = SPIRead();
	} while(status != 0xFE);
	
	// Read off all the bytes in the block
	for(i = 0; i < SD_BlockLen; ++i)
	{
		status = SPIRead();
		*buf = status;
		buf++;
	}
	
	// Read CRC bytes
	status = SPIRead();
	status = SPIRead();

	SD_Disable();

	// Following a read transaction, the SD Card needs 8 clocks after the end 
	// bit of the last data block to finish up its work.
	// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)
	SPIWrite(0xFF); 

	return 0;
}

extern void Sample1(void);
extern int GetSample(unsigned int iPtr);

unsigned int iSucessfulReads = 0;
int SD_ReadSample(unsigned int addr)
{
	unsigned char status;
	unsigned int i, iTimeout, sample;
//	int sample2;

	CMD17_READ_SINGLE_BLOCK[1] = ((addr & 0xFF000000) >> 24);
	CMD17_READ_SINGLE_BLOCK[2] = ((addr & 0x00FF0000) >> 16);
	CMD17_READ_SINGLE_BLOCK[3] = ((addr & 0x0000FF00) >> 8);
	CMD17_READ_SINGLE_BLOCK[4] = ((addr & 0x000000FF));

//TryAgain:
	SD_Enable();

	// Send the read command
	status = SD_WriteCommand(CMD17_READ_SINGLE_BLOCK);
	if(status != 0)
	{
		LED = 1;
		sample = -1;
		goto ExitRead;
	}

	// Now wait for the "Start Block" token	(0xFE)
	// (see SanDisk SD Card Product Manual v1.9 section 5.2.4. Data Tokens)
	iTimeout = 0;
	do
	{
		status = SPIRead();
		if(iTimeout++ > 10000)
		{
			// ABORT: can't get the SD Card working
			LED = 1;
			sample = -2;
			goto ExitRead;
		}
	} while(status != 0xFE);

	
	// Read off all the bytes in the block
	for(i = 0; i < SD_BlockLen; ++i)
	{
		status = SPIRead();
	}
	sample = status;

	// Read CRC bytes
	status = SPIRead();
	i = 0;
	status = SPIRead();

ExitRead:
	SD_Disable();

	// Following a read transaction, the SD Card needs 8 clocks after the end 
	// bit of the last data block to finish up its work.
	// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)
	SPIWrite(0xFF); 
	SPIWrite(0xFF); 
	SPIWrite(0xFF); 
	SPIWrite(0xFF); 

	asm("se	w0, %0" : "+r"(sample)); // sign extend

	iSucessfulReads++;
	return sample;
}

int SD_GetSample(unsigned long addr)
{
	static unsigned int iBuffer = -1;
	static unsigned char* pBuffer;
	static unsigned long lAddress = -1;
	unsigned int i;
	unsigned int iSample;
	static unsigned long addrmasked;

//	addrmasked = addr & (~(SD_SIZE - 1));
	asm("mov #0xFFC0, w0\n and %1, w0, w0\n mov w0, %0\n mov w3, %0+2" 
		: "=T"(addrmasked) : "r"(addr) : "w0");

	if(addrmasked != lAddress)
	{
		// Find cache buffer that holds this address
		for(i = 0; i < CHANNELS*2; ++i)
		{
			iBuffer++;
			if(iBuffer >= (CHANNELS*2))
			{
				iBuffer = 0;
			}

			if(SD_CacheAddr[iBuffer] == addrmasked)
			{
				// found it!
				pBuffer = SD_Cache[iBuffer];
				lAddress = SD_CacheAddr[iBuffer];
				break;
			}
		} 

		if(i == (CHANNELS*2) || SD_CacheAddr[iBuffer] == SD_CacheLoad)
		{
			// this shouldn't happen
			iBuffer = 0;
			pBuffer = SD_Cache[iBuffer];
			lAddress = SD_CacheAddr[iBuffer];
			SD_Prefetch(0);
		}
	}

	// Now find the right byte
	i = addr & (SD_SIZE-1);
	iSample = pBuffer[i];

	asm("se	w0, %0" : "+r"(iSample)); // sign extend

	return iSample;	
}


/**
 * Use this function to update the cache prefetch request tables to match
 * the channel's currently needed sample data. This function does NOT initiate
 * SD Card data transfers by itself; you must call SD_Prefetch later on to
 * start SD Card data transfers.
 */
void SD_ChannelCache(unsigned int iChannel)
{
	unsigned long addr;
	ChannelType* pChan;

	if(iChannel < 0 || iChannel >= CHANNELS)
	{
		// ABORT: invalid channel number
		return;
	}

	pChan = &oChannel[iChannel];
	if(pChan->bActive == 0)
	{
		// This channel isn't playing anything, so we don't need to request
		// anything off the SD Card.
		SD_PriReq[iChannel] = -1;
		SD_SecReq[iChannel] = -1;
		return;
	}

	addr = pChan->SDLoc & (~(SD_SIZE - 1));
	SD_PriReq[iChannel] = addr;

	// Now determine if the next address is still within the sample
	addr += SD_SIZE;
	if(addr > pChan->SDEnd)
	{
		// Too far out, will the sample loop?
		if(pChan->bLoop)
		{
			// Yes, use the loop start as the next address to cache
			addr = pChan->SDLoopStart & (~(SD_SIZE - 1));
		}
		else
		{
			addr = -1;
		}
	}

	SD_SecReq[iChannel] = addr;
}

/**
 * Call this function to initiate SD Card cache prefetch data transfer if any
 * are queued and need processing.
 *
 * @return 0 if nothing needs caching. 1 if a new SD Card transfer started
 */
unsigned int SD_Prefetch(unsigned int iFlags)
{
	unsigned int i, j;
	unsigned int bCache = 0;

	if(iFlags == 2 || iFlags == 3)
	{
		if(SD_NextCacheLoad != -1)
		{
			if(SD_CacheLoad == -1)
			{
				SD_StartCache(SD_NextCacheLoad);
				SD_NextCacheLoad = -1;
			}
			else
			{
				return 0;
			}
		}
	}

	// First scan the primary address request table for an address needed.
	for(i = 0; i < CHANNELS; ++i)
	{
		if(SD_PriReq[i] == -1)
		{
			// Unused cache request slot
			continue;
		}

		if(SD_PriReq[i] == SD_NextCacheLoad)
		{
			// This cache request is already queued to be loaded right now
			return 1;
		}

		bCache = 1;
		for(j = 0; j < (CHANNELS*2); ++j)
		{
			if(SD_CacheAddr[j] == SD_PriReq[i])
			{
				// ABORT: This request is already cached.
				bCache = 0;
				break;
			}
		}

		if(bCache)
		{
			// This address request needs caching.
			SD_StartCache(SD_PriReq[i]);
			return 1;						
		}
	}

	if(iFlags == 1 || iFlags == 3)
	{
		return 0;
	}

	// Okay, all primary address requests are already cached, now see if we
	// can find a secondary address request that needs caching.
	for(i = 0; i < CHANNELS; ++i)
	{
		if(SD_SecReq[i] == -1)
		{
			// Unused cache request slot
			continue;
		}

		if(SD_SecReq[i] == SD_NextCacheLoad)
		{
			// This cache request is already queued to be loaded right now
			return 1;
		}

		bCache = 1;
		for(j = 0; j < (CHANNELS*2); ++j)
		{
			if(SD_CacheAddr[j] == SD_SecReq[i])
			{
				// ABORT: This request is already cached.
				bCache = 0;
				break;
			}
		}

		if(bCache)
		{
			// This address request needs caching.
			SD_StartCache(SD_SecReq[i]);
			return 1;						
		}
	}

	// Nothing needs caching, quit
	return 0;
}

unsigned char SD_StartCache(unsigned long addr)
{
	unsigned int i, j;
	// First we need to find an available SD_Cache that isn't being used any
	// more.
	for(i = 0; i < (CHANNELS*2); ++i)
	{
		if(SD_CacheAddr[i] == -1)
		{
			// This cache block hasn't been used yet, use it!
			break;
		}

		// See if this cache buffer is being used in the primary address
		// request table
		for(j = 0; j < CHANNELS; ++j)
		{
			if(SD_CacheAddr[i] == SD_PriReq[j])
			{
				// This address is needed, skip overwriting this block.
				goto CheckNextBuffer;
			}
		}

		// See if this cache buffer is being used in the secondary address
		// request table
		for(j = 0; j < CHANNELS; ++j)
		{
			if(SD_CacheAddr[i] == SD_SecReq[j])
			{
				// This address is needed, skip overwriting this block.
				goto CheckNextBuffer;
			}
		}

		// If we got to here, nobody is requesting the current SD address.
		break;

CheckNextBuffer:
	}

	if(i == (CHANNELS*2))
	{
		// This should never happen.
		i = 0;
	}

	if(SD_CacheLoad != -1)
	{
		// The SD Card is already busy prefetching data, so don't bother it
		// trying to start another transfer right now, but store what we
		// want it to fetch next time. This needs to be atomic.

		__asm__ volatile("disi #0x1000"); // disable interrupts
		SD_NextCacheLoad = addr;
		SD_NextBuffer = i;
		__asm__ volatile("disi #0x0000"); // enable interrupts
		return 0;
	}

	// Send the read command
	asm("cp	_SPI1BUF"); // read data and discard
	SPI1STATbits.SPIROV = 0; // interrupt handler doesn't fire if overflow set

	IFS0bits.SPI1IF = 0;
	IEC0bits.SPI1IE = 1;

	SD_Enable();

	SD_CacheLoad 	= addr;
	SD_Buffer    	= SD_Cache[i];
	SD_BufferEnd 	= SD_Buffer + SD_SIZE;
	SD_State = 4;

	SPI1BUF = 17 | (1<<6); // read single command

	SD_CacheAddr[i] = addr;

	return 0;
}

volatile unsigned int SD_Count = 0;
volatile unsigned int SD_State = 0;
volatile unsigned char* SD_Buffer;
volatile unsigned char* SD_BufferEnd;
volatile CommandType SD_Command;
volatile unsigned int SD_CRC;

unsigned long SD_PriReq[CHANNELS]; // Primary SD Address Requested table
unsigned long SD_SecReq[CHANNELS]; // Secondary SD Address Requested table

unsigned long SD_CacheAddr[CHANNELS*2];
unsigned char SD_Cache[CHANNELS*2][SD_SIZE];
unsigned long SD_CacheLoad = -1;
unsigned long SD_NextCacheLoad = -1;
unsigned int SD_NextBuffer = 0;
/*
void __attribute__((interrupt, shadow)) _AltSPI1Interrupt(void)
{
	IFS0bits.SPI1IF = 0;	// clear interrupt flag

	asm("setm %1\n mov %1, w0\n mov.b w0, [%0++]" 
		: "+r"(SD_Buffer): "T"(SPI1BUF) : "w0"); // write dummy data to start next read and store read byte

	if(SD_Buffer == SD_BufferEnd)
	{
		INTCON2bits.ALTIVT = 0;
	}
}
*/

void __attribute__((interrupt, shadow)) _SPI1Interrupt(void)
{
	IFS0bits.SPI1IF = 0;	// clear interrupt flag

	switch(SD_State)
	{
		case 9:
			// Wait for response			
			asm("setm %0" :	"=T"(SPI1BUF)); // write dummy data to start next read.
			switch(SPI1BUF)
			{
				case 0:
					// Got the right response, now wait for start data token
					SD_State = 2;
					return;

				case 0xFF:
					// Keep waiting...
					return;

				default:
					// ABORT: invalid response
					SD_Disable();
				
					SD_State = 0;
					IEC0bits.SPI1IE = 0;
		
					// Following a read transaction, the SD Card needs 8 clocks after the end 
					// bit of the last data block to finish up its work.
					// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)
					return;
			}

		case 8:
			// Sending Dummy Command Packet CRC
			asm("setm %0" :	"=T"(SPI1BUF)); // write dummy data to start next read.
			asm("cp	_SPI1BUF"); // read data and discard
			SD_State = 9;
			return;

		case 7:
			// Sending address
			SPI1BUF = SD_CacheLoad & 0xFF;
			asm("cp	_SPI1BUF"); // read data and discard
			SD_State = 8;
			return;

		case 6:
			// Sending address
			SPI1BUF = (SD_CacheLoad >> 8) & 0xFF;
			asm("cp	_SPI1BUF"); // read data and discard
			SD_State = 7;
			return;

		case 5:
			// Sending address
			SPI1BUF = (SD_CacheLoad >> 16) & 0xFF;
			asm("cp	_SPI1BUF"); // read data and discard
			SD_State = 6;
			return;

		case 4:
			// Sending address
			SPI1BUF = (SD_CacheLoad >> 24) & 0xFF;
			asm("cp	_SPI1BUF"); // read data and discard
			SD_State = 5;
			return;

		case 3:
			// Reading CRC bytes
			asm("setm %0" :	"=T"(SPI1BUF)); // write dummy data to start next read.
			asm("cp	_SPI1BUF"); // read data and discard
			SD_Count--;
			// Following a read transaction, the SD Card needs 8 clocks after the end 
			// bit of the last data block to finish up its work.
			// (from SanDisk SD Card Product Manual v1.9 section 5.1.8)

			if(SD_Count == 0)
			{
				// All done!
				SD_Disable();
	
				// Do we have another load queued?
				if(SD_NextCacheLoad == -1)
				{
					// Nope, quit loading data from the SD Card
					SD_CacheLoad = -1;
					SD_State = 0;
					IEC0bits.SPI1IE = 0;
				}
				else
				{
					// Start the next transfer right now!
					asm("cp	_SPI1BUF"); // read data and discard
					SPI1STATbits.SPIROV = 0; // interrupt handler doesn't fire if overflow set
				
					SD_Enable();
				
					SD_CacheLoad 	= SD_NextCacheLoad;
					SD_Buffer    	= SD_Cache[SD_NextBuffer];
					SD_BufferEnd 	= SD_Buffer + SD_SIZE;
					SD_State = 4;
				
					SPI1BUF = 17 | (1<<6); // read single command
				
					SD_CacheAddr[SD_NextBuffer] = SD_CacheLoad;
					SD_NextCacheLoad = -1; // flag that we are done with this one
				}
			}

			return;

		case 2:
			// Now wait for the "Start Block" token	(0xFE)
			// (see SanDisk SD Card Product Manual v1.9 section 5.2.4. Data Tokens)
			asm("setm %0" :	"=T"(SPI1BUF)); // write dummy data to start next read.
			if(SPI1BUF == 0xFE)
			{
				// Found the start block, switch to alternative ISR
				INTCON2bits.ALTIVT = 1;

				SD_State = 3;	// this is the next state we'll go into after
				SD_Count = 2;	// we've read the SD Card block.
			}
			return;

		case 0:
		default:
			// Idle state, ignore data and quit.
			asm("cp	_SPI1BUF"); // read data and discard
			return;
	}
}

unsigned char SD_StartRead(unsigned long addr)
{
	while(SD_State != 0);

	SD_Command.address = addr;

	// Send the read command

	asm("cp	_SPI1BUF"); // read data and discard
	SPI1STATbits.SPIROV = 0; // interrupt handler doesn't fire if overflow set

	IFS0bits.SPI1IF = 0;
	IEC0bits.SPI1IE = 1;

	SD_Enable();

	SD_State = 4;
	SPI1BUF = 17 | (1<<6); // read command

	return 0;
}
