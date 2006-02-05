// -*- tab-width: 4 -*-
// Mixer Engine Commands
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/


#ifndef ROW_H
#define ROW_H

#include <fstream>
#include <iostream>
#include <vector>
#include "cmd.h"

using namespace std;

class Row
{
 public:
  Row();
  ~Row();
  
  void add(cmd oCmd);
  void addFront(cmd oCmd);
  void addFront(cmdWait oCmd);
  void addRear(cmd oCmd);

  cmd& operator[](int i);
  
  void nextChannel(void);

  bool IsEmpty(void);
  bool patternBreak();

  int length(void);
  void getData(unsigned char *pBuffer);

  friend ostream& operator<<(ostream& out, Row& oRow);

 protected:
  cmdList vRow;
  cmdList vEnd;
  bool bNextChannel;

  int iCurrentChannel;

 private:

};

ostream& operator<<(ostream& out, Row& oRow);

#endif // ROW_H
