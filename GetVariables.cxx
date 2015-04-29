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
    if(! infile.is_open()) {
        cerr << "Unable to open: " << filename << endl;
        exit;
    }
}

GetVariables::~GetVariables()
{
    /// Destructor
}


int GetVariables::GetInt(std::string variable_int)
{
    intvar = variable_int;

    infile.seekg(0);

    while(!infile.eof()) {

        getline(infile,tempstr); /// reads in from infile and puts in tempstr

        x = tempstr.find(variable_int);   /// gets position of front of var string

        if (x!= -1) {          /// -1 means string was not found in that line
            tempstr.erase(x, x+variable_int.length());

            outint = atoi(tempstr.c_str());
            return outint;
        }
    }
    cerr<< "Cant find : " << variable_int << "\n";
    return -1;
}


double GetVariables::GetDouble(std::string variable_dbl)
{
    dblvar = variable_dbl;

    infile.seekg(0);

    while(!infile.eof()) {
        getline(infile,tempstr);   /// reads in from infile and puts in tempstr
        x = tempstr.find(dblvar);   /// gets position of front of var string
        if (x!= -1) {          /// -1 means string was not found in that line
            tempstr.erase(x, x+dblvar.length());
            outvar = atof(tempstr.c_str());
            return outvar;
        }

    }
    cerr<< "Cant find : " << variable_dbl << "\n";
    return -1;
}
