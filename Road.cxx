//************************************************************************* 
//  Road.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class handles roads in projection view 
//
// 
//
//************************************************************************* 
 
#include "Road.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0

using namespace std;

Road::Road(CStransform *cst)
{
       transformation = cst;
       for (int i = 0; i<5; i++)
       {
	 ZZ[i] = 0;
	 XX[i] = 0;
       }

        pline = new TPolyLine(5,XX,ZZ);
        pline->SetLineColor(kBlack);
        pline->SetLineWidth(1);

#if DEBUG_LEVEL >= 3
  	cout<<"Road object created!!!"<<endl;
#endif

}

void Road::DrawMe(double *x, double *z)
{
       for (int i = 0; i<4; i++)
       {
	 ZZ[i] = transformation->transYtoCY(z[i]);
	 XX[i] = transformation->transXtoCX(x[i]);
       }
       ZZ[4] = ZZ[0];
       XX[4] = XX[0];

        pline->SetPolyLine(5,XX,ZZ);
	pline->SetLineWidth(1);
        pline->SetLineColor(kPink+8);
	pline->SetFillColor(kPink+8);
	pline->SetFillStyle( 3002 );
        pline->Draw("f");
        pline->Draw();
        
}

void Road::Clear()
{
       for (int i = 0; i<5; i++)
       {
	 ZZ[i] = 0.0;
	 XX[i] = 0.0;
       }

        pline->SetPolyLine(5,XX,ZZ);
        pline->SetLineColor(kWhite);
        pline->SetLineWidth(1);
        pline->Draw();
}

Road::~Road()
{

}     
