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
#include "TString.h"

class GetVariables
{

public:
    GetVariables(std::string filename);
    virtual ~GetVariables();
    int GetInt(std::string variable_int);
    double GetDouble(std::string variable_dbl);
    double outvar;
    int outint;
    std::string file;

protected:
    ifstream infile;

    std::string intvar;
    std::string dblvar;
    std::string tempstr;
    int x;

};

#endif
