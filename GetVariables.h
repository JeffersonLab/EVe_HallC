//************************************************************************* 
//  GetVariables.h - Friday, June 13/14
// 
//  by Ben Davis-Purcell
// 
//  This class inputs all needed geometry variables from a text file.
//
//
//************************************************************************* 

#ifndef ROOT_getvariables
#define ROOT_getvariables

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

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
