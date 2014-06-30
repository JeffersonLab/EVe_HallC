//************************************************************************* 
//  GetVariables.h - Friday, June 13/14
// 
//  by Ben Davis-Purcell
// 
//  This class inputs all needed geometry variables from a text file.
//
//
//************************************************************************* 

#ifndef ROOT_GetVariables
#define ROOT_GetVariables

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <utility>
#include "TString.h"

class GetVariables {

 public:
  GetVariables(string variable);
  virtual ~GetVariables();
  double outvar;
  int outint;

 protected:
  ifstream infile;
  string var;
  string tempstr;
  string intcheck;
  int x;
  char varchar[100];   /// conversion of string to char to convert to double
  
};

#endif
