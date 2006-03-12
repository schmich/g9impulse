#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define new(T) ((T*)alloc(sizeof(T)))
#define newArray(T, C) ((T*)alloc(sizeof(T) * (C)))

void* alloc(uint8 size);
void free(void* ptr);
void memoryInit(void);

uint16 memoryFree(void);

#endif // MEMORY_H
