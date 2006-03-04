#include "sralloc.h"
#include "memory.h"
#include "gpu.h"

void* alloc(uint8 size)
{
#ifndef _DEBUG
    return (void*)SRAMalloc(size);
#else
    void* p = (void*)SRAMalloc(size);
    if (!p)
        setFieldColor(0b11000000);

    return p;
#endif
}

void free(void* ptr)
{
    SRAMfree((unsigned char*)ptr);
}

void memoryInit(void)
{
    SRAMInitHeap();
}
