#include "sralloc.h"
#include "memory.h"
#include "gpu.h"

#ifdef _DEBUG
//
// lifted from sralloc.c
//
typedef union _SALLOC
{
	unsigned char byte;
	struct _BITS
	{
		unsigned count:7;
		unsigned alloc:1;	
	}bits;
}SALLOC;

uint16 theBytesFree = 0x9FF;
#endif

void* alloc(uint8 size)
{
#ifdef _DEBUG
    void* p = (void*)SRAMalloc(size);
    if (!p)
        setFieldColor(0b11000000);

    if (theBytesFree < (uint16)size)
        theBytesFree = 0;
    else
        theBytesFree -= (uint16)size;

    return p;
#else
    return (void*)SRAMalloc(size);
#endif
}

void free(void* ptr)
{
#ifdef _DEBUG
    unsigned freed = (*(SALLOC*)(ptr - 1)).bits.count - 1;
    theBytesFree += (uint16)freed;

    SRAMfree((unsigned char*)ptr);
#else
    SRAMfree((unsigned char*)ptr);
#endif
}

void memoryInit(void)
{
    SRAMInitHeap();
}

uint16 memoryFree(void)
{
    return theBytesFree;
}
