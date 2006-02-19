#include "sralloc.h"
#include "memory.h"

void* alloc(uint8 size)
{
    return (void*)SRAMalloc(size);
}

void free(void* ptr)
{
    SRAMfree((unsigned char*)ptr);
}

void memoryInit(void)
{
    SRAMInitHeap();
}
