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

ScintillatorPaddle::ScintillatorPaddle(int index, double x, double y, double a, double b, double cx0,double cy0, int PMTn, double PMTlength, double ang) 
{

  // x and y are the coords of the lower left edge of the object
  // a is the horizontal size of the PMT
  // b is the vertical size of the PMT
  // length is the length (horizontal) of the scintillator
  // PMTn is the number of PMTs on the scintillator paddle (1 or 2)
  sx0 = x;
  sy0 = y;
  sa = a;
  sb = b;
  // paddle_length = length;
  n = PMTn;
  angle=ang;
  
  cx0=cx0-PMTlength;
  cy0=cy0;
  pl=PMTlength;

  /// transformed paddle height as in corresponding comment in ScintPlane
     
  //consider sypaddle length is different from sxpaddle length
    double xL[7] = {pl*0.0 + cx0 ,pl*0.42857 + cx0 ,pl + cx0 , pl +cx0 , pl*0.42857 + cx0 , pl*0.0 + cx0 , pl*0.0 + cx0 };
    double yL[7] = {b*0.175 + cy0, b*0.175 + cy0, b*0.25 + cy0, b*0.0 + cy0, b*0.075 + cy0, b*0.075 + cy0, b*0.175 + cy0};

    double xR[7] = {a+pl + cx0 ,a+pl*(2-0.42857) + cx0 ,a+2*pl + cx0 ,a+2*pl + cx0 ,a+pl*(2-0.42857) + cx0 , a+pl + cx0 , a+pl + cx0 };
    double yR[7] = {b*0.25 + cy0 , b*0.175 + cy0, b*0.175 + cy0, b*0.075 + cy0, b*0.075 + cy0, b*0.0 + cy0, b*0.25 + cy0};

    double xscint[5] = {pl + cx0, a+pl + cx0, a+pl + cx0, pl + cx0, pl + cx0};
    double yscint[5] = {b*0.0 + cy0, b*0.0 + cy0, b*0.25 + cy0,b*0.25 + cy0, b*0.0 + cy0};


    double* xl=xL;
    double* yl=yL;
    double* xr=xR;
    double* yr=yR;
    double* xs=xscint;
    double* ys=yscint;
     
    double xT=a*1.0+2*pl+cx0;
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
  //delete yindicator;
  //yindicator = new TArrow(0.0,0.0,0.0,0.0, 0.01, "<|>");
}
