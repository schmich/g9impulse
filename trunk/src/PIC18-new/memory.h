#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void* alloc(uint8 size);
void free(void* ptr);
void memoryInit(void);

#endif // MEMORY_H
