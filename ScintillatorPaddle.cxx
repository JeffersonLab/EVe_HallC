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

void rotranspaddle (double angle, double* xL, double* yL, double* xR, double* yR, double* xscint, double* yscint,double sx0,double sy0);
void rotransline(double ang, double* x, double* y, double sx0, double sy0,int size);

ScintillatorPaddle::ScintillatorPaddle(int index, double x, double y, double a, double b, double cx0,double cy0, int PMTn, double ang) 
{

  // x and y are the coords of the lower left edge of the object
  // a is the horizontal size of the PMT
  // b is the vertical size of the PMT
  // length is the length (horizontal) of the scintillator
  // PMTn is the number of PMTs on the scintillator paddle (1 or 2)
  // rotation is currently a T/F for a vertical or horizontal paddle. 
  // rotation == 1 for horizontal paddles, 0 (or other) for vertical paddles

  sx0 = x;
  sy0 = y;
  sa = a;
  sb = b;
  // paddle_length = length;
  n = PMTn;
  angle=ang;
  
  cx0=cx0-a*0.21875;
  cy0=cy0;
  /// Scaling of a/sa, b/sb needs to be fixed, written in terms of 
  /// transformed paddle height as in corresponding comment in ScintPlane


  /*if (rot == 1){    //// horizontal paddle
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
    
    /// This draws an arrow for the hit position along the paddle
    /// Only used if y hit position is known
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
    // x and y coordinates reversed for vertical paddle
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
    
    /// This draws an arrow for the hit position along the paddle
    /// Only used if y hit position is known
    yindicator = new TArrow( sb*0.0+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, sb*0.25+sy0, sa*0.5+sx0+sa*0.5625*y/paddle_length, 0.01, "<|>");

    TString Buff;
    Buff="";
    Buff+=(index+1);
    const char *name = Buff.Data();
    
    index_text = new TLatex(-b*0.23+sy0,a*1.02+sx0, name);
    index_text->SetTextSize(0.03);
    index_text->Draw();  
    */
   

    double xL[7] = {a*0.0 + cx0 ,a*0.09375 + cx0 ,a*0.21875 + cx0 , a*0.21875 +cx0 , a*0.09375 + cx0 , a*0.0 + cx0 , a*0.0 + cx0 };
    double yL[7] = {b*0.175 + cy0, b*0.175 + cy0, b*0.25 + cy0, b*0.0 + cy0, b*0.075 + cy0, b*0.075 + cy0, b*0.175 + cy0};

    double xR[7] = {a*0.78125 + cx0 ,a*0.90625 + cx0 ,a*1.0 + cx0 ,a*1.0 + cx0 ,a*0.90625 + cx0 , a*0.78125 + cx0 , a*0.78125 + cx0 };
    double yR[7] = {b*0.25 + cy0 , b*0.175 + cy0, b*0.175 + cy0, b*0.075 + cy0, b*0.075 + cy0, b*0.0 + cy0, b*0.25 + cy0};

    double xscint[5] = {a*0.21875 + cx0, a*0.78125 + cx0, a*0.78125 + cx0, a*0.21875 + cx0, a*0.21875 + cx0};
    double yscint[5] = {b*0.0 + cy0, b*0.0 + cy0, b*0.25 + cy0,b*0.25 + cy0, b*0.0 + cy0};
    double* xl=xL;
    double* yl=yL;
    double* xr=xR;
    double* yr=yR;
    double* xs=xscint;
    double* ys=yscint;
     
    double xT=a*1.0+cx0;
    double yT=b*0.125+cy0;
    double* tx=& xT;
    double* ty=& yT;
    
    rotranspaddle (angle, xl, yl, xr, yr, xs, ys, sx0,sy0);
    rotransline(angle, tx, ty,sx0,sy0,1);
 
    plineL = new TPolyLine(7,xL,yL);
    plineL->SetFillColor(38);
    plineL->SetLineColor(1);
    plineL->SetLineWidth(1);
    plineL->Draw("f");
    plineL->Draw();

    plineR = new TPolyLine(7,xR,yR);
    plineR->SetFillColor(38);
    plineR->SetLineColor(1);
    plineR->SetLineWidth(1);
    plineR->Draw("f");
    plineR->Draw();

    scint = new TPolyLine(5,xscint,yscint);
    scint->SetFillColor(38);
    scint->SetLineColor(1);
    scint->SetLineWidth(1);
    scint->Draw("f");
    scint->Draw();
    
    TString Buff;
    Buff="";
    Buff+=(index+1);
    const char *name = Buff.Data();
    
    index_text = new TLatex((Double_t)xT,(Double_t)yT, name);
    index_text->SetTextSize(0.02);
    index_text->Draw();
}


void rotranspaddle(double angle, double * xL, double * yL, double * xR, double * yR, double * xscint, double * yscint,double sx0,double sy0)
{
  rotransline(angle, xL, yL,sx0,sy0,7);
  rotransline(angle, xR, yR,sx0,sy0,7);
  rotransline(angle, xscint, yscint,sx0,sy0,5);
}

void rotransline(double ang, double * x, double * y,double sx0, double sy0, int size)
{
	ang*=(3.1415926/180);
	for (int i=0; i< size; i++)
	
	{  double tempx=x[i]*cos(ang)-y[i]*sin(ang);
	   double tempy=x[i]*sin(ang)+y[i]*cos(ang);
	   x[i]=tempx;
	   y[i]=tempy;}
        for (int i=0; i<size; i++)
	  {  x[i]=x[i]+sx0;
	    y[i]=y[i]+sy0;}

}

  

ScintillatorPaddle::~ScintillatorPaddle()
{
  //Destructor
}


/// OLD hit method - currently not in use but here for reference
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


// NEW hit method (3 functions) -- must be similarly implemented in 3D paddles
void ScintillatorPaddle::HitLeft()
{
  plineL->SetFillColor(2);

}


void ScintillatorPaddle::HitRight()
{
  plineR->SetFillColor(2);
  
}

void ScintillatorPaddle::HitBoth()
{
  plineR->SetFillColor(3);
  plineL->SetFillColor(3);
}


void ScintillatorPaddle::clear()
{
  plineR->SetFillColor(38);
  plineL->SetFillColor(38);
  delete yindicator;
  yindicator = new TArrow(0.0,0.0,0.0,0.0, 0.01, "<|>");
}
