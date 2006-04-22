#ifndef STORAGE_H
#define STORAGE_H

#include "common.h"

//
// fromWhere/toWhere must be < 1024,
// since that's all that is accessible
//

char readStorageByte(uint16 fromWhere);
void writeStorageByte(char data, uint16 toWhere);

//
// readStorage does not NULL terminate
//
void readStorage(char* buffer, uint16 count, uint16 fromWhere);
void writeStorage(char* buffer, uint16 count, uint16 toWhere);

#endif // STORAGE_H
