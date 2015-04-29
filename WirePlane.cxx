//*************************************************************************
//  WirePlane.cxx  - 4/14/2008
//
//  by miham
//
//  This class creates the projection view for given wire plane in given
//  wire chamber.
//
//
//
//*************************************************************************

#include "WirePlane.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 3

using namespace std;


WirePlane::WirePlane(char *name, int n, double x, double y, double l, double a, double b, int f)
{
    planename = name;
    WireNum = n;
    rescale_me = 1.0/1000.0*b;
    x0 = x;
    y0 = y;
    type = f;
    length = l;

    double fac = length/(WireNum);
    for (int i = 0; i<WireNum; i++) {
        wire[i] = new TEllipse(x0+a*fac*(i), y0, a*0.001, b*0.001);
        wire[i]->SetFillColor(kBlack);
        wire[i]->Draw();
        // Draw the wires, initializing them all to 0 (no hits)
        time[i] = new TLine(x0+a*fac*(i), y0+0.0, x0+a*fac*(i), y0-0.0);
        time[i] -> SetLineColor(kBlack);
        time[i] -> Draw();
    }

    if (type==1) { // Back Planes - X' , V , Y'
        TLatex *title = new TLatex(x0+a*fac*(WireNum),y - b*(0.008-0.01), name);
        title->SetTextSize(b*0.02);
        title->Draw();
    } else if (type==-1) { // Front Planes - X, U, Y
        TLatex *title = new TLatex(x0+a*fac*(WireNum),y - b*(0.008+0.01), name);
        title->SetTextSize(b*0.02);
        title->Draw();
    } else {
        TLatex *title = new TLatex(x0+a*fac*(WireNum),y - b*0.008, name);
        title->SetTextSize(b*0.02);
        title->Draw();
    }

    cout<<"Wire plane created!"<<endl;
}

WirePlane::~WirePlane()
{
}

void WirePlane::SetWire(int i, double t)
{
    cout<<planename<<"Index is: "<<i<<", T is: "<<t<<endl;
    if (i < WireNum) {
        if (type==1) { // Back Planes - X' , V , Y'
            time[i]->SetY1(rescale_me*t+y0);
            time[i]->SetLineColor(kGreen+1);
        } else if (type==-1) { // Front Planes - X, U, Y
            time[i]->SetY2(-1.0*rescale_me*t+y0);
            time[i]->SetLineColor(kRed);
        } else {
            time[i]->SetY1(rescale_me*t+y0);
            time[i]->SetY2(-1.0*rescale_me*t+y0);
            time[i]->SetLineColor(kBlue);
        }
    }

}

void WirePlane::clear()
{
    for(int i = 0; i<WireNum; i++) {
        time[i]->SetY1(0.0+y0);
        time[i]->SetY2(0.0+y0);
        time[i]->SetLineColor(kBlack);
    }
}

void WirePlane::Hit(double *signal)
{
    for (int i = 0; i<WireNum; i++) {
        SetWire(i, signal[i]);
    }
}
