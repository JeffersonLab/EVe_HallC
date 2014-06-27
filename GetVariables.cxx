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

GetVariables::GetVariables(string variable) 
{

  var = variable;

  // char file[256];
  // cout << "Enter filename: ";
  // cin >> file; 
  // cout << endl;

  infile("HMS.txt");

  while(!infile.eof()) {
    getline(infile,tempstr);   /// reads in from infile and puts in tempstr
    x = tempstr.find(var);   /// gets position of front of var string
    if (x!= -1) {          /// -1 means string was not found in that line
      tempstr.erase(x, x+var.length());
      intcheck = tempstr.find(".");

      if (intcheck == -1) {   // doubles must be declared with decimals
	                      // if no decimals found (==-1) then convert to int
	strcpy(varchar, tempstr.c_str()); 
	outint = atoi(varchar);
      }
      else { 
	strcpy(varchar, tempstr.c_str()); 
	outvar = atof(varchar);
      }
    }
  }

}
 
GetVariables::~GetVariables()
{
  /// Destructor
}
 
