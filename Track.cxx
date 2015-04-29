//*************************************************************************
//  Track.cxx  - 4/14/2008
//
//  by miham
//
//  This class handles Track projections in projection view
//
//*************************************************************************

#include "Track.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define  DEBUG_LEVEL 0

using namespace std;


Track::Track(CStransform *cst)
{
    transformation = cst;
    line = new TLine(0,0,0,0);
    line->SetLineColor(kBlack);
    line->SetLineWidth(1);

#if DEBUG_LEVEL >= 3
    cout<<"Trackobject created!!!"<<endl;
#endif

}

void Track::DrawMe(double x1, double y1, double x2, double y2)
{

    double XX1 = transformation->transXtoCX(x1);
    double YY1 = transformation->transYtoCY(y1);

    double XX2 = transformation->transXtoCX(x2);
    double YY2 = transformation->transYtoCY(y2);

    line->SetX1(XX1);
    line->SetX2(XX2);
    line->SetY1(YY1);
    line->SetY2(YY2);
    line->SetLineColor(kOrange+7);
    line->SetLineWidth(1);
    line->Draw();
}

void Track::Clear()
{
    line->SetX1(0);
    line->SetX2(0);
    line->SetY1(0);
    line->SetY2(0);

    line->SetLineColor(kWhite);
    line->SetLineWidth(1);
    line->Draw();
}

Track::~Track()
{

}
