#include "storage.h"

#define MEMORY_MAX 1023

//
// EEPROM control constants
//
#define RD      0b00000001
#define WR      0b00000010
#define WREN    0b00000100
#define WRERR   0b00001000
#define FREE    0b00010000
#define CFGS    0b01000000
#define EPGD    0b10000000

//
// interrupt constants
//
#define GIE   0b10000000

void readStorage(char* buffer, uint16 count, uint16 fromWhere)
{
    uint16 pos;
    uint16 addr;

    if (fromWhere + count > MEMORY_MAX)
        return;

    //
    // slow but safe
    //
    for (pos = 0, addr = fromWhere; pos < count; ++pos, ++addr)
        buffer[pos] = readStorageByte(addr);
}

void writeStorage(char* buffer, uint16 count, uint16 toWhere)
{
    uint16 pos;
    uint16 addr;

    if (toWhere + count > MEMORY_MAX)
        return;

    //
    // slow but safe
    //
    for (pos = 0, addr = toWhere; pos < count; ++pos, ++addr)
        writeStorageByte(buffer[pos], addr);
}

char readStorageByte(uint16 fromWhere)
{
    if (fromWhere > MEMORY_MAX)
        return 0;

    EEADR = fromWhere & 0xFF;           // set low memory address bits
    EEADRH = (fromWhere >> 8) & 0xFF;   // set high memory address bits

    EECON1 &= ~(EPGD | CFGS);           // point to data memory, access EEPROM
    EECON1 |= RD;                       // perform read

    while (EECON1 & RD)
        { /* wait for read to complete*/ }

    return EEDATA;
}

void writeStorageByte(char data, uint16 toWhere)
{
    if (toWhere > MEMORY_MAX)
        return;

    EEADR = toWhere & 0xFF;             // set low memory address bits
    EEADRH = (toWhere >> 8) & 0xFF;     // set high memory address bits

    EEDATA = data;                      // load data to be written

    EECON1 &= ~(EPGD | CFGS);           // point to data memory, access EEPROM
    EECON1 |= WREN;                     // enable writes
    
    INTCON &= ~GIE;                     // disable interrupts

    EECON2 = 0x55;                      // necessary
    EECON2 = 0xAA;                      // sequence

    EECON1 |= WR;                       // set write bit to begin writes

    INTCON |= GIE;                      // enable interrupts

    EECON1 &= ~WREN;                    // disable writes

    while (EECON1 & WR)
        { /* wait for write to complete*/ }
}
