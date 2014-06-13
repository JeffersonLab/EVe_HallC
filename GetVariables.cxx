//************************************************************************* 
//  GetVariables.cxx - Friday, June 13/14
// 
//  by Ben Davis-Purcell
// 
//  This class inputs all needed geometry variables from a text file.
//
//
//*************************************************************************

#include "GetVariables.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

GetVariables::GetVariables(ifstream infile) 
{

  char values[256];
  vector<double> variable;
  file = infile;
  file("textfile.txt");
  while(!file.eof()) {
    file.ignore(256,'=');
    file.getline(values,256);
    variable.push_back(atof (values));
  }
  
}
 
GetVariables::~GetVariables()
{
  /// Destructor
}
 
