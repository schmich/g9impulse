// -*- tab-width: 4 -*-
// Loader for TRAXMOD Player
// Copyright (c) 2005, K9spud LLC.
// http://www.k9spud.com/
#include "stdafx.h"
#include <iostream>

#include "mod.h"

int main(int argc, char *argv[])
{
  char sFileName[1000] = "input/mountain.mod";
  
  cout << "loading..." << endl;
  
  if(argc > 1)
	sprintf(sFileName, "%s", argv[1]);
	
  mod oMod(sFileName);
}
