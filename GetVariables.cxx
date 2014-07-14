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

using namespace std;

GetVariables::GetVariables(std::string filename)
{
  file = filename;
  infile.open(filename.c_str());
  
  // while(!infile.eof()) {
  //   getline(infile,tempstr);   /// reads in from infile and puts in tempstr
  //   x = tempstr.find(var);   /// gets position of front of var string
  //   if (x!= -1) {          /// -1 means string was not found in that line
  //     tempstr.erase(x, x+var.length());
  //     //   intcheck = tempstr.find('.');

  //     //   if (intcheck == -1) {   // doubles must be declared with decimals
  // 	                      // if no decimals found (==-1) then convert to int
  // 	//strcpy(varchar, tempstr.c_str()); 
  // 	outint = atoi(tempstr.c_str());
  //     }
  //     else { 
  // 	//	strcpy(varchar, tempstr.c_str()); 
  // 	//outvar = atof(varchar);
  // 	outvar = atof(tempstr.c_str());
  //     }
  //   }
  // }
  
}
 
GetVariables::~GetVariables()
{
  /// Destructor
}
 
// void GetVariables::GetFile(std::string filename)
// {
//   file = filename;
  
//   infile.open(file);
//   if (infile.is_open()) {
//     cout << "File is open" endl;
//   }
// }

int GetVariables::GetInt(std::string variable_int)
{
  intvar = variable_int;
  while(!infile.eof()) {
    getline(infile,tempstr);   /// reads in from infile and puts in tempstr
    x = tempstr.find(intvar);   /// gets position of front of var string
    if (x!= -1) {          /// -1 means string was not found in that line
      tempstr.erase(x, x+intvar.length());
   
    }
    outint = atoi(tempstr.c_str());
  }
  return outint;
}


double GetVariables::GetDouble(std::string variable_dbl)
{
  dblvar = variable_dbl;
  while(!infile.eof()) {
    getline(infile,tempstr);   /// reads in from infile and puts in tempstr
    x = tempstr.find(dblvar);   /// gets position of front of var string
    if (x!= -1) {          /// -1 means string was not found in that line
      tempstr.erase(x, x+dblvar.length());
      
    }
    outvar = atof(tempstr.c_str());
  }
  return outvar;
}
