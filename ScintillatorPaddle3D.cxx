///////////////////////////////////////
/*  ScintillatorPaddle3D.cxx  7/16/14
    
    Ben Davis-Purcell

    Class that creates a single 3D scintillation paddle with attached PMTs

*/
///////////////////////////////////////

 

#include "ScintillatorPaddle3D.h"
#include "EVe_DB.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

using namespace std;

ScintillatorPaddle3D::ScintillatorPaddle3D(char* PlaneName, int index, int n,
                                           double length, double height, double thickness,
                                           TGeoVolume *paddle, int numPMT /*int rotation */)
{

    double r_PMT = 2.0;// Change to use HMS.txt
    double L=length;
    double H=height;
    double T=thickness;
    
    //(x,y,z) is the coordinate of the center of this single paddle

    double x= 0;
    double y= -((-1)* H*((double)n-1)/2.0+(double)index*H);
    double z= 0;
    
    //Drawing a single Scintillator Paddle in z direction till the end of this constructor
    //Draw scintillator Paddle and side skirt paddle first

    TGeoBBox *scintb = new TGeoBBox(Form("%s.%d.ScintPaddle",PlaneName,index),T/2.0, H/2.0, L/2.0);
    TGeoTranslation *transcint = new TGeoTranslation(x,y,z);
    scint = new TGeoVolume (Form("%s.%d.Paddle",PlaneName,index), scintb);
    scint ->SetLineColor(kBlack);
    paddle -> AddNode(scint,1,transcint);

    //Draw PMT on each side
    TGeoTrd2 *pmtedge = new TGeoTrd2("Edge",T/2.0,T/4.0,H/2.0,T/4.0,L/10.0);
    TGeoTube *pmttube = new TGeoTube("Tube",0.0, r_PMT , 0.3*length/2.0);

    TGeoTranslation *t1 = new TGeoTranslation("t1",0,0,-0.15*L);
    t1->RegisterYourself();
    TGeoTranslation *t2 = new TGeoTranslation("t2",0,0,0.1*L);
    t2->RegisterYourself();

    TGeoCompositeShape *pmt = new TGeoCompositeShape("pmt","(Edge:t1)+(Tube:t2)");
    pmt1= new TGeoVolume(Form("%s.%d.PMT1",PlaneName,index),pmt);
    pmt1->SetLineColor(kBlack);

    TGeoTranslation *pmttrans= new TGeoTranslation("pmttrans",x,y,z+0.65*L);

    paddle->AddNode(pmt1,1,pmttrans);

     //Draw lower PMT if n_PMT=2
     if(numPMT==2)
     {
         TGeoTranslation pmt2t(x,y,z-0.65*L);
         TGeoRotation pmt2r;
         pmt2r.SetAngles(90,180,90,90,180,0);
         TGeoCombiTrans *pmt2CT= new TGeoCombiTrans(pmt2t,pmt2r);
         pmt2= new TGeoVolume(Form("%s.%d.PMT2",PlaneName,index),pmt);
         pmt2->SetLineColor(kBlack);
         paddle ->AddNode(pmt2,1,pmt2CT);
     }
}

ScintillatorPaddle3D::~ScintillatorPaddle3D()
{
    //Destructor
}

//Below using the same hit method used in planar view ScintPlane and Paddle Classes

void ScintillatorPaddle3D::HitL()
{
  pmt1->SetLineColor(kRed);
}

void ScintillatorPaddle3D::HitR()
{
  pmt2->SetLineColor(kRed);
}

void ScintillatorPaddle3D::HitB()
{
  pmt1->SetLineColor(kGreen);
  pmt2->SetLineColor(kGreen);
}

void ScintillatorPaddle3D::clear()
{
    pmt1->SetLineColor(kBlack);
    pmt2->SetLineColor(kBlack);
}
