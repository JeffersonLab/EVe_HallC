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

ScintillatorPaddle::ScintillatorPaddle(int index, double x, double y, double a, double b, double length, int PMTn, int rotation) 
{

  // x and y are the coords of the lower left edge of the object
  // a is the vertical size of the PMT
  // b is the horizontal size of the PMT
  // length is the length (horizontal) of the scintillator
  // PMTn is the number of PMTs on the scintillator (1 or 2)
  // rotation is currently a T/F for a vertical or horizontal paddle. 
  // rotation == 1 for horizontal paddles, 0 (or other) for vertical paddles

  sx0 = x - a*0.21875;
  sy0 = y;
  sa = a;
  sb = b;
  paddle_length = length;
  n = PMTn;
  rot = rotation;

  if (rot == 1){    //// horizontal paddle
  // Left PMT
    Double_t xL[7] = {a*0.0 + sx0 ,a*0.09375 + sx0 ,a*0.21875 + sx0 , a*0.21875 +sx0 , a*0.09375 + sx0 , a*0.0 + sx0 , a*0.0 + sx0 };
    Double_t yL[7] = {b*0.175 + y, b*0.175 + y, b*0.25 + y, b*0.0 + y, b*0.075 + y, b*0.075 + y, b*0.175 + y};
    plineL = new TPolyLine(7,xL,yL);
    plineL->SetFillColor(38);
    plineL->SetLineColor(1);
    plineL->SetLineWidth(1);
    plineL->Draw("f");
    plineL->Draw();
    
    // Right PMT
    
    if (n == 2) {
      Double_t xR[7] = {a*0.78125 + sx0 ,a*0.90625 + sx0 ,a*1.0 + sx0 ,a*1.0 + sx0 ,a*0.90625 + sx0 , a*0.78125 + sx0 , a*0.78125 + sx0 };
      Double_t yR[7] = {b*0.25 + y , b*0.175 + y, b*0.175 + y, b*0.075 + y, b*0.075 + y, b*0.0 + y, b*0.25 + y};
      
      plineR = new TPolyLine(7,xR,yR);
      plineR->SetFillColor(38);
      plineR->SetLineColor(1);
      plineR->SetLineWidth(1);
      plineR->Draw("f");
      plineR->Draw();
    }
    
  // Scintillation Material
    
    Double_t xscint[5] = {a*0.21875 + sx0, a*0.78125 + sx0, a*0.78125 + sx0, a*0.21875 + sx0, a*0.21875 + sx0};
    Double_t yscint[5] = {b*0.0 + y, b*0.0 + y, b*0.25 + y,b*0.25 + y, b*0.0 + y};
    
    scint = new TPolyLine(5,xscint,yscint);
    scint->SetFillColor(38);
    scint->SetLineColor(1);
    scint->SetLineWidth(1);
    scint->Draw("f");
    scint->Draw();
    
    yindicator = new TArrow(sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, 0.01, "<|>");
    
    TString Buff;
    Buff="";
    Buff+=(index+1);
    const char *name = Buff.Data();
    
    index_text = new TLatex(a*1.02+sx0,b*0.02+sy0, name);
    index_text->SetTextSize(0.03);
    index_text->Draw();   
  }
  
  
  /// vertical paddle
  else {
    // Left PMT
    Double_t xL[7] = {b*0.175 + y, b*0.175 + y, b*0.25 + y, b*0.0 + y, b*0.075 + y, b*0.075 + y, b*0.175 + y};
    Double_t yL[7] = {a*0.0 + sx0 ,a*0.09375 + sx0 ,a*0.21875 + sx0 , a*0.21875 +sx0 , a*0.09375 + sx0 , a*0.0 + sx0 , a*0.0 + sx0 };
    plineL = new TPolyLine(7,xL,yL);
    plineL->SetFillColor(38);
    plineL->SetLineColor(1);
    plineL->SetLineWidth(1);
    plineL->Draw("f");
    plineL->Draw();
    
    // Right PMT
    
    if (n == 2) {
      Double_t xR[7] = {b*0.25 + y , b*0.175 + y, b*0.175 + y, b*0.075 + y, b*0.075 + y, b*0.0 + y, b*0.25 + y};
      Double_t yR[7] = {a*0.78125 + sx0 ,a*0.90625 + sx0 ,a*1.0 + sx0 ,a*1.0 + sx0 ,a*0.90625 + sx0 , a*0.78125 + sx0 , a*0.78125 + sx0 }; 
      
      plineR = new TPolyLine(7,xR,yR);
      plineR->SetFillColor(38);
      plineR->SetLineColor(1);
      plineR->SetLineWidth(1);
      plineR->Draw("f");
      plineR->Draw();
    }

  // Scintillation Material
 
    Double_t xscint[5] = {b*0.0 + y, b*0.0 + y, b*0.25 + y,b*0.25 + y, b*0.0 + y};
    
    Double_t yscint[5] = {a*0.21875 + sx0, a*0.78125 + sx0, a*0.78125 + sx0, a*0.21875 + sx0, a*0.21875 + sx0};
    
    scint = new TPolyLine(5,xscint,yscint);
    scint->SetFillColor(38);
    scint->SetLineColor(1);
    scint->SetLineWidth(1);
    scint->Draw("f");
    scint->Draw();
    
    yindicator = new TArrow( sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, 0.01, "<|>");

    TString Buff;
    Buff="";
    Buff+=(index+1);
    const char *name = Buff.Data();
    
    index_text = new TLatex(b*0.02+sy0,a*1.02+sx0, name);
    index_text->SetTextSize(0.03);
    index_text->Draw();   
  }
  
  
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
      yindicator = new TArrow(sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, 0.008, "<|>");
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
