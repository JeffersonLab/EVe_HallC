
//************************************************************************* 
//  Scintilator.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view one scintilation paddle.
//
//
//*************************************************************************  

#include "Scintilator.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0

using namespace std;




Scintilator::Scintilator(int index, double x, double y, double a, double b, double length)
{
   // x and y coordinates of the lower left edge of the object. 
   // a and b determine the vertical and horizontal size of the photomultiplier
   // !! Coordinates of the vertices are always between 0 and 1. 

   sx0 = x- a*0.21875;
   sy0 = y;
   sa = a;
   sb = b;
   paddle_length = length;

   // first make left photomultiplier
   Double_t xL[7] = {a*0.0 + sx0 ,a*0.09375 + sx0 ,a*0.21875 + sx0 , a*0.21875 + sx0 , a*0.09375 + sx0 , a*0.0 + sx0 , a*0.0 + sx0 };
   Double_t yL[7] = {b*0.175 + y, b*0.175 + y, b*0.25 + y, b*0.0 + y, b*0.075 + y, b*0.075 + y, b*0.175 + y};
   plineL = new TPolyLine(7,xL,yL);
   plineL->SetFillColor(38);
   plineL->SetLineColor(1);
   plineL->SetLineWidth(1);
   plineL->Draw("f");
   plineL->Draw();

   // Then right photomultiplier

   Double_t xR[7] = {a*0.78125 + sx0 ,a*0.90625 + sx0 ,a*1.0 + sx0 ,a*1.0 + sx0 ,a*0.90625 + sx0 , a*0.78125 + sx0 , a*0.78125 + sx0 };
   Double_t yR[7] = {b*0.25 + y , b*0.175 + y, b*0.175 + y, b*0.075 + y, b*0.075 + y, b*0.0 + y, b*0.25 + y};

   plineR = new TPolyLine(7,xR,yR);
   plineR->SetFillColor(38);
   plineR->SetLineColor(1);
   plineR->SetLineWidth(1);
   plineR->Draw("f");
   plineR->Draw();

   // Now do the scintillation material

   Double_t xscint[5] = {a*0.21875 + sx0, a*0.78125 + sx0, a*0.78125 + sx0, a*0.21875 + sx0, a*0.21875 + sx0};
   Double_t yscint[5] = {b*0.0 + y, b*0.0 + y, b*0.25 + y,b*0.25 + y, b*0.0 + y};

   scintilator = new TPolyLine(5,xscint,yscint);
   scintilator->SetFillColor(38);
   scintilator->SetLineColor(1);
   scintilator->SetLineWidth(1);
   scintilator->Draw("f");
   scintilator->Draw();

   yindicator = new TArrow(sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, 0.01, "<|>");

   TString Buff;
   Buff="";
   Buff+=(index+1);
   const char *ime = Buff.Data();

#if DEBUG_LEVEL >= 3	
   cout<<"Ime je: "<<ime<<endl;
#endif

   index_text = new TLatex(a*1.02+sx0,b*0.02+sy0, ime);
   index_text->SetTextSize(0.03);
   index_text->Draw();      

#if DEBUG_LEVEL >= 3	
   cout<<"Scintilation paddle is created!"<<endl;
#endif

}

Scintilator::~Scintilator()
{

}


void Scintilator::hit(double left, double right, double y)
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
    yindicator = new TArrow(sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, 0.008, "<|>");
    yindicator->Draw();
  }

}

void Scintilator::clear()
{
    plineR->SetFillColor(38);
    plineL->SetFillColor(38);
    delete yindicator;
    yindicator = new TArrow(0.0,0.0,0.0,0.0, 0.01, "<|>");
}
