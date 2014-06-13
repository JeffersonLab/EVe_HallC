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

#include <vector>
#include <fstream>

class GetVariables {

 public:
  GetVariables(ifstream file);
  virtual ~GetVariables();

 protected:
  ifstream infile;
}

#endif
