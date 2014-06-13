
//************************************************************************* 
//  Scintillator.cxx  - Monday, June 9/14
// 
//  by Ben Davis-Purcell
// 
// This class is used to create the planar view of a single scintillator paddle.
//
//
//*************************************************************************  

#include "Scintillator.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0

using namespace std;




Scintillator::Scintillator(int index, double xpos, double ypos, double pmtx, double pmty, double length, int pmtnum)
{
   // x and y coordinates of the lower left edge of the object. 
   // pmtx and pmty determine the vertical and horizontal size of the photomultiplier
   // !! Coordinates of the vertices are always between 0 and 1. 

   sx0 = xpos- pmtx*0.21875;
   sy0 = ypos;
   spmtx = pmtx;
   spmty = pmty;
   paddle_length = length;
   PMTn = pmtnum;

   // first make left photomultiplier
   Double_t xL[7] = {pmtx*0.0 + sx0 ,pmtx*0.09375 + sx0 ,pmtx*0.21875 + sx0 , pmtx*0.21875 + sx0 , pmtx*0.09375 + sx0 , pmtx*0.0 + sx0 , pmtx*0.0 + sx0 };
   Double_t yL[7] = {pmty*0.175 + ypos, pmty*0.175 + ypos, pmty*0.25 + ypos, pmty*0.0 + ypos, pmty*0.075 + ypos, pmty*0.075 + ypos, pmty*0.175 + ypos};
   plineL = new TPolyLine(7,xL,yL);
   plineL->SetFillColor(38);
   plineL->SetLineColor(1);
   plineL->SetLineWidth(1);
   plineL->Draw("f");
   plineL->Draw();

   // Then right photomultiplier
   if (PMTn == 2) {
   Double_t xR[7] = {pmtx*0.78125 + sx0 ,pmtx*0.90625 + sx0 ,pmtx*1.0 + sx0 ,pmtx*1.0 + sx0 ,pmtx*0.90625 + sx0 , pmtx*0.78125 + sx0 , pmtx*0.78125 + sx0 };
   Double_t yR[7] = {pmty*0.25 + ypos , pmty*0.175 + ypos, pmty*0.175 + ypos, pmty*0.075 + ypos, pmty*0.075 + ypos, pmty*0.0 + ypos, pmty*0.25 + ypos};

   plineR = new TPolyLine(7,xR,yR);
   plineR->SetFillColor(38);
   plineR->SetLineColor(1);
   plineR->SetLineWidth(1);
   plineR->Draw("f");
   plineR->Draw();
   }

   // Now do the scintillation material

   Double_t xscint[5] = {pmtx*0.21875 + sx0, pmtx*0.78125 + sx0, pmtx*0.78125 + sx0, pmtx*0.21875 + sx0, pmtx*0.21875 + sx0};
   Double_t yscint[5] = {pmty*0.0 + ypos, pmty*0.0 + ypos, pmty*0.25 + ypos,pmty*0.25 + ypos, pmty*0.0 + ypos};

   scintillator = new TPolyLine(5,xscint,yscint);
   scintillator->SetFillColor(38);
   scintillator->SetLineColor(1);
   scintillator->SetLineWidth(1);
   scintillator->Draw("f");
   scintillator->Draw();

   yindicator = new TArrow(spmtx*0.5+sx0+spmtx*0.5625*ypos/paddle_length, spmty*0.0+sy0, spmtx*0.5+sx0+spmtx*0.5625*ypos/paddle_length, spmty*0.25+sy0, 0.01, "<|>");

   TString Buff;
   Buff="";
   Buff+=(index+1);
   const char *ime = Buff.Data();

#if DEBUG_LEVEL >= 3	
   cout<<"Ime je: "<<ime<<endl;
#endif

   index_text = new TLatex(pmtx*1.02+sx0,pmty*0.02+sy0, ime);
   index_text->SetTextSize(0.03);
   index_text->Draw();      

#if DEBUG_LEVEL >= 3	
   cout<<"Scintillation paddle is created!"<<endl;
#endif

}

Scintillator::~Scintillator()
{

}


void Scintillator::hit(double left, double right, double ypos)
{
  double min = -10000.0;
  // If the chamber was not hit the program returns 1e-35
  // If the chamber was a legitimate hit, the program returns a number 
  // that is less than 0.
  // The program can also return something that is >0
  // but this is not treated as a legit event.
 
  // Hit was just right photomultiplier
  if (left<min && right>min)
  {
    plineR->SetFillColor(2);
  } 

  // Hit was only left photomultiplier
  if (left>min  && right<min )
  {
    plineL->SetFillColor(2);
  }

  // Hit 2 photomultipliers
  if (left>min && right>min)
  {
    plineR->SetFillColor(3);
    plineL->SetFillColor(3);
  }

  if ((left>min) || (right>min))
  {	
    yindicator = new TArrow(spmtx*0.5+sx0+spmtx*0.5625*ypos/paddle_length, spmty*0.0+sy0, spmtx*0.5+sx0+spmtx*0.5625*ypos/paddle_length, spmty*0.25+sy0, 0.008, "<|>");
    yindicator->Draw();
  }

}

void Scintillator::clear()
{
    plineR->SetFillColor(38);
    plineL->SetFillColor(38);
    delete yindicator;
    yindicator = new TArrow(0.0,0.0,0.0,0.0, 0.01, "<|>");
}
