///////////////////////////////////////
/*  ScintillatorPaddle3D.cxx  7/16/14
    
    Ben Davis-Purcell

    Class that creates a single 3D scintillation paddle with attached PMTs

*/
///////////////////////////////////////

 

#include "ScintillatorPaddle3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

using namespace std;

ScintillatorPaddle3D::ScintillatorPaddle3D(char* PlaneName, int index, int n,
                                           double length, double height, double thickness,
                                           TGeoVolume *paddle, int numPMT)
{

    double r_PMT = 2.0;// Change to use HMS.txt
    double l_PMT = 8.0;// Change to use HMS.txt
    double L=length;
    double H=height;
    double T=thickness;

    //(x,y,z) is the coordinate of the center of this single paddle

    double x= 0;
    double y= -((-1)* H*((double)n-1)/2.0+(double)index*H);
    double z= 0;

    //Drawing a single Scintillator Paddle in z direction till the end of this constructor
    //Draw scintillator Paddle and side skirt paddle first
    TGeoBBox *scintb = new TGeoBBox(Form("%s.%d.ScintPaddle",PlaneName,index),0.9*T/2.0, 0.9*H/2.0, L/2.0);
    TGeoTranslation *transcint = new TGeoTranslation(x,y,z);
    scint = new TGeoVolume (Form("%s.%d.Paddle",PlaneName,index), scintb);
    scint ->SetLineColor(kBlack);
    paddle -> AddNode(scint,1,transcint);

    //Draw PMT on each side
    TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",1,PlaneName,index),
        0.0, r_PMT , l_PMT);
    pmt1= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,1),pmt);
    pmt1->SetLineColor(kBlack);
    TGeoTranslation *pmttrans1= new TGeoTranslation("pmttrans1",x,y,z+(0.5*L+l_PMT));
    paddle->AddNode(pmt1,1,pmttrans1);

     //Draw lower PMT if n_PMT=2
     if(numPMT==2)
     {
         TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",2,PlaneName,index),
             0.0, r_PMT , l_PMT);
         pmt2= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,2),pmt);
         pmt2->SetLineColor(kBlack);
         TGeoTranslation *pmttrans2 = new TGeoTranslation("pmttrans2",x,y,z-(0.5*L+l_PMT));
         paddle->AddNode(pmt2,1,pmttrans2);
     }
}

ScintillatorPaddle3D::~ScintillatorPaddle3D()
{
    //Destructor
}

//Below using the same hit method used in planar view ScintPlane and Paddle Classes

void ScintillatorPaddle3D::HitPaddle()
{
  scint ->SetLineColor(kGreen);
}

void ScintillatorPaddle3D::HitL()
{
  pmt1->SetLineColor(kGreen);
}

void ScintillatorPaddle3D::HitR()
{
  //FIXME: what about single PMT case (no pmt2)?
  pmt2->SetLineColor(kGreen);
}

void ScintillatorPaddle3D::clear()
{
  //FIXME: what about single PMT case (no pmt2)?
  pmt1->SetLineColor(kBlack);
  pmt2->SetLineColor(kBlack);
  scint ->SetLineColor(kBlack);
}
