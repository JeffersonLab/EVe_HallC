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

ScintillatorPaddle3D::ScintillatorPaddle3D(int index, double x, double y, double z,  double length, double height, double thickness, TGeoVolume *paddle, int numPMT, int rotation)
{
  plength = s1x_paddle_length;
  //  n = numPMT;
  double r_PMT = 2.0; 

  // scintillator paddle
  Double_t StartingPoint[3]={x,y,z};
  TGeoBBox *scint_box = new TGeoBBox("scint_box",0.9*height/2.0,thickness/2.0,length/2.0,StartingPoint);
  scint = new TGeoVolume("scint",scint_box);
  scint->SetLineColor(5);

  TGeoRotation *rot1 = new TGeoRotation("rot1", 0.0,0.0,90.0,90.0,90.0,0.0);
  TGeoCombiTrans *combo = new TGeoCombiTrans(plength/2.0, 0.0, -plength/1.0, rot1); /// add on for rotated paddles

  /* This rotates 90 deg about paddle axis
  TGeoRotation *rot1 = new TGeoRotation("rot1",90.0, 90.0, 90.0,180.0,0.0,0.0);
  */
  if (rotation == 1) {
    paddle->AddNode(scint,1);
  }
  else {
    paddle->AddNode(scint,1, combo);
  }

  // side skirt scintillator
  TGeoBBox *side_box = new TGeoBBox("side_box",0.5*0.1*height/2.0,thickness/2.0,length/2.0);
  TGeoVolume *sides = new TGeoVolume("sides",side_box);
  sides->SetLineColor(kBlack);
  TGeoTranslation *trans1 = new TGeoTranslation((0.9+0.05)*height/2.0+x,0.0 + y,0.0 + z);

  // r1->SetAngles(0,0,90,90,0,0);
  // (90,0,90,90,0,0) is no rotation (by GEANT3 angles)

  TGeoCombiTrans *combo1 = new TGeoCombiTrans((0.9+0.05)*height/2.0+x + plength/2.0 ,0.0 + y,0.0 + z - plength, rot1);
  TGeoCombiTrans *combo2 = new TGeoCombiTrans(-(0.9+0.05)*height/2.0 + x + plength/2.0 ,0.0 + y,0.0 + z - plength, rot1);


  TGeoTranslation *trans2 = new TGeoTranslation(-(0.9+0.05)*height/2.0 + x,0.0 + y,0.0 + z);

  if (rotation == 1) {
    paddle->AddNode(sides, 1, trans1);
    paddle->AddNode(sides, 2, trans2);
  }
  else {
    paddle->AddNode(sides,1, combo1);
    paddle->AddNode(sides,2, combo2);
  }
  TGeoTrd2 *trapezoid1 = new TGeoTrd2("trapezoid1",height/2, thickness/4.0, thickness/2.0, thickness/4.0, length/10.0);
  TGeoVolume *pmtedge1 = new TGeoVolume("pmtedge1",trapezoid1);
  pmtedge1->SetLineColor(kBlack);

  TGeoCombiTrans *combo3 = new TGeoCombiTrans(0.0+x +plength/2.0,0.0+y,length/2.0 + length/10.0 +z - plength, rot1);
  if (rotation ==1) {
    paddle->AddNode(pmtedge1,1, new TGeoTranslation(0.0+x,0.0+y,length/2.0 + length/10.0 +z));
  }
  else {
    paddle->AddNode(pmtedge1,1,combo3);
  }

  if (numPMT==2) {
    TGeoTrd2 *trapezoid2 = new TGeoTrd2("trapezoid2",thickness/4.0, height/2.0, thickness/4.0, thickness/2.0, length/10.0);
    TGeoVolume *pmtedge2 = new TGeoVolume("pmtedge2",trapezoid2);
    pmtedge2->SetLineColor(kBlack);
    
    TGeoCombiTrans *combo4 = new TGeoCombiTrans(0.0+x + plength/2.0,0.0+y,-length/2.0 - length/10.0+z - plength, rot1);
    if (rotation ==1) {
      paddle->AddNode(pmtedge2,1, new TGeoTranslation(0.0+x,0.0+y,-length/2.0 - length/10.0+z));
    }
    else {
      paddle->AddNode(pmtedge2,1,combo4);
    }
  }
  TGeoTube *tube1 = new TGeoTube("tube1",0.0, r_PMT , 0.3*length/2.0);
  pmt1 = new TGeoVolume("pmt1",tube1);
  pmt1->SetLineColor(kBlack);
  TGeoCombiTrans *combo5 = new TGeoCombiTrans(0.0+x + plength/2.0 ,0.0+y, (length/2.0 + 2.0*length/10.0+ 0.3*length/2.0) + z - plength, rot1);

  if (rotation ==1 ) {
    paddle->AddNode(pmt1,1, new TGeoTranslation(0.0+x,0.0+y, (length/2.0 + 2.0*length/10.0+ 0.3*length/2.0) + z));
  }
  else {
    paddle->AddNode(pmt1,1,combo5);
  }
  if (numPMT==2) {
    TGeoTube *tube2 = new TGeoTube("tube2",0.0, r_PMT , 0.3*length/2.0);
    pmt2 = new TGeoVolume("pmt2",tube2);
    pmt2->SetLineColor(kBlack);
    TGeoCombiTrans *combo6 = new TGeoCombiTrans(0.0+x + plength/2.0,0.0+y,-(length/2.0 + 2.0*length/10.0+ 0.3*length/2.0)+z - plength, rot1);

    if (rotation ==1) {
      paddle->AddNode(pmt2,1, new TGeoTranslation(0.0+x,0.0+y,-(length/2.0 + 2.0*length/10.0+ 0.3*length/2.0)+z));
    }
    else {
      paddle->AddNode(pmt2,1,combo6);
    }
  }
}

ScintillatorPaddle3D::~ScintillatorPaddle3D()
{

}


/// FIXME:: This hit method needs to be re-written or a new one must be added to            create hits analogous to tdchits as in the ScintillatorPaddle planar            class. This will be propagated through to the ScintPlane3D class and            the Detector3D class.

void ScintillatorPaddle3D::hit(double left, double right, int nPMT)
{
  double min = -10000.0;
  // If scint. plane is not hit, then left or right
  // is set to 1E-35. However, if paddle was hit, then 
  // left or right is set to a value < 0.

   // Only right PMT got signal
  if (left<min && right>min)
  {
    pmt1->SetLineColor(2);
  } 
  if (nPMT==2) {     ///// R and L may need to be swapped to match planar L/R
    // Only left PMT got signal
    if (left>min  && right<min )
      {
	pmt2->SetLineColor(2);
      }
    
    // Both PMTs received a pulse.
    if (left>min && right>min)
      {
	pmt1->SetLineColor(3);
	pmt2->SetLineColor(3);
      }
  }
}

void ScintillatorPaddle3D::clear()
{
    pmt1->SetLineColor(kBlack);
    pmt2->SetLineColor(kBlack);
}
