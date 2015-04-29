#include "WireChamber.h"
#include "WirePlane2D.h"
#include "TMath.h"
#include "GetVariables.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>


#include <vector>
#include <string>
#include <map>

using namespace std;

WireChamber:: WireChamber(char* chamberName,CStransform *trans)
{
    cst=trans;

    string PN[6]= {"x", "y", "u", "v", "yp", "xp"};
    vector<string> planeNames(&PN[0],&PN[0]+6);

    GetVariables *HMSvars = new GetVariables("HMS.txt");
    double Height = HMSvars->GetDouble(Form("%s.Height =",chamberName));
    double Width = HMSvars->GetDouble(Form("%s.Width =",chamberName));

    double x =  cst->transXtoCX(0.0) - cst->transLtoCL(Width)/2;
    double y =  cst->transYtoCY(0.0) - cst->transLtoCL(Height)/2;

    double a = cst->transLtoCL(Width);
    double b = cst->transLtoCL(Height);

    //Draw the out frame of chambers

    Double_t xbox[5] = {a*0.0-0.05*b+x, a*0.0-0.05*b+x, a+0.05*b+x, a+0.05*b+x, a*0.0-0.05*b+x};
    Double_t ybox[5] = {b*0.0-0.05*b+y, b+0.05*b+y, b+0.05*b+y, b*0.0-0.05*b+y, b*0.0-0.05*b+y};
    box1 = new TPolyLine(5,xbox,ybox);
    box1->SetFillColor(38);
    box1->SetLineColor(1);
    box1->SetLineWidth(1);
    box1->Draw("f");
    box1->Draw();


    Double_t xbox2[5] = {a*0.0+x, a*0.0+x, a+x, a+x, a*0.0+x};
    Double_t ybox2[5] = {b*0.0+y,b+y, b+y, b*0.0+y, b*0.0+y};
    box2 = new TPolyLine(5,xbox2,ybox2);
    box2->SetFillColor(0);
    box2->SetLineColor(1);
    box2->SetLineWidth(1);
    box2->Draw("f");
    box2->Draw();

    //Draw all wire planes
    GetVariables *hmsVars = new GetVariables("HMS.txt");
    for(unsigned int i=0; i<planeNames.size(); i++) {
        wirePlanes.insert (
            std::pair<string, WirePlane2D>(planeNames[i], WirePlane2D(
                                               planeNames[i],
                                               Height, Width,
                                               hmsVars->GetDouble(Form("%s.%s.WireAngle =", chamberName, planeNames[i].c_str())),
                                               hmsVars->GetInt(Form("%s.%s.NumWires =",  chamberName, planeNames[i].c_str())),
                                               i+2 ,/* I'm using 'i' to set the wire color (last arg) so each
             * plane is assigned a different color */
                                               cst ,
                                               hmsVars->GetDouble(Form("%s.%s.Offset =",  chamberName, planeNames[i].c_str())),
                                               i
                                           )));
    }

    title = new TLatex(x,b*1.06+y, chamberName);
    title->SetTextSize(0.03);
    title->Draw();

    for(int i = 0; i<MAX_TRACK_NUM; i++) track_circ[i] = new TEllipse(0,0,0);

    cout<<"WireChamber is created!"<<endl;
}


WireChamber::~WireChamber() {}

void WireChamber::WireHit(string planeName, int wireNum)
{

    wirePlanes.find(planeName)->second.WireHit(wireNum);
}

void WireChamber::clear()
{
    std::map<string,WirePlane2D>:: iterator itr;

    for(itr=wirePlanes.begin(); itr!=wirePlanes.end(); itr++) {
        itr->second.clear();
    }

    for (int i = 0; i<MAX_TRACK_NUM; i++) {
        track_circ[i]->SetX1(0.0);
        track_circ[i]->SetY1(0.0);
        track_circ[i]->SetR1(0.0);
        track_circ[i]->SetR2(0.0);
        track_circ[i]->SetFillColor(kWhite);
        track_circ[i]->Draw();
    }
}

void WireChamber::Track(double x, double y, int i)
{
// First we need to transform meters to pixels
    double CX =  cst->transXtoCX(x);
    double CY =  cst->transYtoCY(y);

    track_circ[i]->SetX1(CX);
    track_circ[i]->SetY1(CY);
    track_circ[i]->SetR1(0.008);
    track_circ[i]->SetR2(0.008);
    track_circ[i]->SetLineColor(1+i);
    track_circ[i]->SetLineWidth(2);
    track_circ[i]->Draw();
}
