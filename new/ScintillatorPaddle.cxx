///////////////////////////////////////
/*  ScintillatorPaddle.cxx  6/16/14
    
    Ben Davis-Purcell

    This class creates the planar view of a single scintillation paddle
    including PMTs

*/
///////////////////////////////////////

#include "ScintillatorPaddle.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

using namespace std;

ScintillatorPaddle::ScintillatorPaddle(int index, double x, double y, double a, double b, double length, int PMTn) 
{

  // x and y are the coords of the lower left edge of the object
  // a is the vertical size of the PMT
  // b is the horizontal size of the PMT
  // length is the length (horizontal) of the scintillator
  // num is the number of PMTs on the scintillator (1 or 2)

  x0 = x - a*0.21875;
  y0 = y;
  pa = a;
  pb = b;
  paddle_length = length;
  n = PMTn;
  n = 2;

  // Left PMT
  Double_t xL[7] = {a*0.0 + x0 ,a*0.09375 + x0 ,a*0.21875 + x0 , a*0.21875 + x0 , a*0.09375 + x0 , a*0.0 + x0 , a*0.0 + x0 };
  Double_t yL[7] = {b*0.175 + y, b*0.175 + y, b*0.25 + y, b*0.0 + y, b*0.075 + y, b*0.075 + y, b*0.175 + y};
  plineL = new TPolyLine(7,xL,yL);
  plineL->SetFillColor(38);
  plineL->SetLineColor(1);
  plineL->SetLineWidth(1);
  plineL->Draw("f");
  plineL->Draw();

  // Right PMT

  if (n == 2) {
    Double_t xR[7] = {a*0.78125 + x0 ,a*0.90625 + x0 ,a*1.0 + x0 ,a*1.0 + x0 ,a*0.90625 + x0 , a*0.78125 + x0 , a*0.78125 + x0 };
    Double_t yR[7] = {b*0.25 + y , b*0.175 + y, b*0.175 + y, b*0.075 + y, b*0.075 + y, b*0.0 + y, b*0.25 + y};
  
    plineR = new TPolyLine(7,xR,yR);
    plineR->SetFillColor(38);
    plineR->SetLineColor(1);
    plineR->SetLineWidth(1);
    plineR->Draw("f");
    plineR->Draw();
  }

  // Scintillation Material
 
  Double_t xscint[5] = {a*0.21875 + x0, a*0.78125 + x0, a*0.78125 + x0, a*0.21875 + x0, a*0.21875 + x0};
  Double_t yscint[5] = {b*0.0 + y, b*0.0 + y, b*0.25 + y,b*0.25 + y, b*0.0 + y};

  scint = new TPolyLine(5,xscint,yscint);
  scint->SetFillColor(38);
  scint->SetLineColor(1);
  scint->SetLineWidth(1);
  scint->Draw("f");
  scint->Draw();

  yindicator = new TArrow(pa*0.5+x0+pa*0.5625*y/paddle_length, pb*0.0+y0, pa*0.5+x0+pa*0.5625*y/paddle_length, pb*0.25+y0, 0.01, "<|>");

  TString Buff;
  Buff="";
  Buff+=(index+1);
  const char *name = Buff.Data();

  index_text = new TLatex(a*1.02+x0,b*0.02+y0, name);
  index_text->SetTextSize(0.03);
  index_text->Draw();   

}

ScintillatorPaddle::~ScintillatorPaddle()
{
  //Destructor
}


void ScintillatorPaddle::hit(double left, double right, double y)
{
  double min = -10000.0;
  // If the paddle was not hit the program returns 1e-35
  // If the paddle was hit, the program returns a number 
  // that is less than 0.
  // The program can also return something that is >0
  // but this is not treated as a legitimate event.
 
  // Only left PMT was hit
  if (left>min  && right<min )
    {
      plineL->SetFillColor(2);
    }

  if (n == 2) {
    // Only right PMT was hit
    if (left<min && right>min)
      {
	plineR->SetFillColor(2);
      } 

    // Both PMTs were hit
    if (left>min && right>min)
      {
	plineR->SetFillColor(3);
	plineL->SetFillColor(3);
      }
  }

  if ((left>min) || (right>min))
    {	
      yindicator = new TArrow(pa*0.5+x0+pa*0.5625*y/paddle_length, pb*0.0+y0, pa*0.5+x0+pa*0.5625*y/paddle_length, pb*0.25+y0, 0.008, "<|>");
      yindicator->Draw();
    }

}

void ScintillatorPaddle::clear()
{
  plineR->SetFillColor(38);
  plineL->SetFillColor(38);
  delete yindicator;
  yindicator = new TArrow(0.0,0.0,0.0,0.0, 0.01, "<|>");
}
