// -*- tab-width: 4 -*-
// Pattern Container Object
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/


#ifndef PATTERN_H
#define PATTERN_H

#include "cmd.h"
#include "row.h"

class Pattern
{
 public:
  Pattern();
  ~Pattern();

  Row& operator[](int i);

  int length(void);
  void getData(unsigned char* pBuffer);

 protected:
  Row oRows[64];

  int NumRows;

  int iCurrentRow;
  int iCurrentCmd;
  int iCurrentByte;
  bool bEOF;

 private:

};

#endif // PATTERN_H
