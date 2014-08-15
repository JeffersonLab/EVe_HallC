
//************************************************************************* 
//  MWDChamber.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view of the MWDC.
//
//
//*************************************************************************   

#include "MWDChamber.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0


using namespace std;

MWDChamber::MWDChamber(char *name, int n, double Height, CStransform *trans, int t)
{
   type = t;

   cst = trans;

   double x =  cst->transXtoCX(0.0) - cst->transLtoCL(Height)/8;
   double y =  cst->transYtoCY(0.0) - cst->transLtoCL(Height)/2;


   double a = cst->transLtoCL(Height); 
   double b = cst->transLtoCL(Height);

   fa = a;
   fb = b;

   // n = actual number of wires
   // Num = number of wires that we draw

   if ((n/5.0 - int(n/5.0))>0) Num =  (int) (n/5.0) + 1; // must be rounded up
   else Num = (int) (n/5.0);


  
   double LL = 0.25;
   double HH = 1.0;


   Double_t xbox[5] = {a*0.0-0.05*a+x, a*0.0-0.05*a+x, a*LL+0.05*a+x, a*LL+0.05*a+x, a*0.0-0.05*a+x};
   Double_t ybox[5] = {b*0.0-0.05*b+y, b*HH+0.05*b+y, b*HH+0.05*b+y, b*0.0-0.05*b+y, b*0.0-0.05*b+y};
   box = new TPolyLine(5,xbox,ybox);
   box->SetFillColor(38);
   box->SetLineColor(1);
   box->SetLineWidth(1);
   box->Draw("f");
   box->Draw();


   Double_t xbox2[5] = {a*0.0+x, a*0.0+x, a*LL+x, a*LL+x, a*0.0+x};
   Double_t ybox2[5] = {b*0.0+y,b*HH+y, b*HH+y, b*0.0+y, b*0.0+y};
   box2 = new TPolyLine(5,xbox2,ybox2);
   box2->SetFillColor(0);
   box2->SetLineColor(1);
   box2->SetLineWidth(1);
   box2->Draw("f");
   box2->Draw();
  
   // The first chamber with 2 levels of wires

  
   if (type==0)
   {

     // The 1st plane (x1)
     for (int i = 0; i<Num; i++)
     {
	double fac = (HH)*(i)/(Num);
        x1_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        x1_wires[i]->SetLineColor(kGray);
        x1_wires[i]->Draw(); 
	
     }
     // 2nd plane (x-prime)
     for (int i = 0; i<Num; i++)
     {
        double fac =  (HH)/Num/2.0 + (HH)*(i)/(Num);
        xp_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        xp_wires[i]->SetLineColor(kGray);
        xp_wires[i]->Draw(); 
	
    }
     // U plane
     for (int i = 0; i<Num; i++)
     {
	double cn =- (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  u_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  u_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
	}
	else
	{
	  u_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
	}
     }

     // V plane
     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/Num/2 - (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  v_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          v_wires[i]->SetLineColor(kGray);
          v_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  v_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          v_wires[i]->SetLineColor(kGray);
          v_wires[i]->Draw();   
	}
	else
	{
	  v_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          v_wires[i]->SetLineColor(kGray);
          v_wires[i]->Draw();   
	}

     }


     // 1st Y plane (y1)

     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);
	//double cn = -(HH*2.0/sqrt(3.))/(Num)*7.0/2.0 + (HH*2.0/sqrt(3.))/(Num)*(i);
	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  y1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  y1_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();  	  
	}
	else
	{
	  y1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();   
	}
     }


     // 2nd Y plane (y-prime)

     for (int i = 0; i<Num; i++)
     {
	double cn =-(HH*2.0/sqrt(3.))/(Num)/2 -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);


	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  yp_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          yp_wires[i]->SetLineColor(kGray);
          yp_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  yp_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          yp_wires[i]->SetLineColor(kGray);
          yp_wires[i]->Draw();  	  
	}
	else
	{
	  yp_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          yp_wires[i]->SetLineColor(kGray);
          yp_wires[i]->Draw();   
	}
     }

     // sedaj naredimo se semaforcek ?????

     x1_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8+y, a*0.03, b*0.03 );
     x1_circ->SetFillColor(kGray);
     x1_circ->Draw();

     TLatex *x1_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77+y, "X");
     x1_title->SetTextSize(0.02);
     x1_title->Draw(); 
 
     xp_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.03+y, a*0.03, b*0.03 );
     xp_circ->SetFillColor(kGray);
     xp_circ->Draw();

     TLatex *xp_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.03+y, "X'");
     xp_title->SetTextSize(0.02);
     xp_title->Draw(); 

     u_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.06+y, a*0.03, b*0.03 );
     u_circ->SetFillColor(kGray);
     u_circ->Draw();

     TLatex *u_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.06+y, "U");
     u_title->SetTextSize(0.02);
     u_title->Draw(); 

     v_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.09+y, a*0.03, b*0.03 );
     v_circ->SetFillColor(kGray);
     v_circ->Draw();

     TLatex *v_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.09+y, "V");
     v_title->SetTextSize(0.02);
     v_title->Draw(); 

     y1_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.12+y, a*0.03, b*0.03 );
     y1_circ->SetFillColor(kGray);
     y1_circ->Draw();

     TLatex *y1_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.12+y, "Y");
     y1_title->SetTextSize(0.02);
     y1_title->Draw(); 

     yp_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.15+y, a*0.03, b*0.03 );
     yp_circ->SetFillColor(kGray);
     yp_circ->Draw();

     TLatex *yp_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.15+y, "Y'");
     yp_title->SetTextSize(0.02);
     yp_title->Draw(); 

   }
   else if (type==1) /// SECON TYPE
   {

     // X1 plane
     for (int i = 0; i<Num; i++)
     {
	double fac = (HH)*(i)/(Num);
        x1_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        x1_wires[i]->SetLineColor(kGray);
        x1_wires[i]->Draw(); 
	
     }

     // U plane
     for (int i = 0; i<Num; i++)
     {
	double cn =- (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  u_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  u_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
	}
	else
	{
	  u_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          u_wires[i]->SetLineColor(kGray);
          u_wires[i]->Draw();   
	}
     }

  
     // Y1 plane

     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);
	//double cn = -(HH*2.0/sqrt(3.))/(Num)*7.0/2.0 + (HH*2.0/sqrt(3.))/(Num)*(i);
	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  y1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  y1_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();  	  
	}
	else
	{
	  y1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          y1_wires[i]->SetLineColor(kGray);
          y1_wires[i]->Draw();   
	}
     }




     y1_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8+y, a*0.03, b*0.03 );
     y1_circ->SetFillColor(kGray);
     y1_circ->Draw();

     TLatex *y1_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77+y, "Y");
     y1_title->SetTextSize(0.02);
     y1_title->Draw(); 

     u_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.03+y, a*0.03, b*0.03 );
     u_circ->SetFillColor(kGray);
     u_circ->Draw();


     TLatex *u_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.03+y, "U");
     u_title->SetTextSize(0.02);
     u_title->Draw(); 

    
     TLatex *x1_title = new TLatex(a*((LL+0.02+0.05) + 0.015+0.05) +x,b*0.77 - 0.06+y, "X");
     x1_title->SetTextSize(0.02);
     x1_title->Draw(); 

     x1_circ = new TEllipse(a*(LL+0.02+0.05+0.02)+x,b*0.8-0.06+y, a*0.03, b*0.03 );
     x1_circ->SetFillColor(kGray);
     x1_circ->Draw();

 
   }

   title = new TLatex(x,b*1.06+y, name);
   title->SetTextSize(0.03);
   title->Draw(); 

   for(int i = 0; i<MAX_TRACK_NUM; i++) track_circ[i] = new TEllipse(0,0,0);
  
   cout<<"MWDChamber is created!"<<endl;
}


MWDChamber::~MWDChamber()
{

}

void MWDChamber::x1WireHit(int i)
{
  int quotient = (int) i/5; /// 1 wire drawn for every 5 "real" wires
  if (quotient<Num)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****u1WireHit: "<<quotient<<endl;
#endif
    x1_wires[quotient]->SetLineColor(kGreen);
    x1_wires[quotient]->SetLineWidth(3);
    x1_circ->SetFillColor(kGreen);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber::xpWireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****u2WireHit: "<<quotient<<endl;
#endif
    xp_wires[quotient]->SetLineColor(kGreen+3);
    xp_wires[quotient]->SetLineWidth(3);
    xp_circ->SetFillColor(kGreen+3);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber::uWireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num )
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****v1WireHit: "<<quotient<<endl;
#endif
    u_wires[quotient]->SetLineColor(kAzure+8);
    u_wires[quotient]->SetLineWidth(3);
    u_circ->SetFillColor(kAzure+8);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber::vWireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****v2WireHit: "<<quotient<<endl;
#endif
    v_wires[quotient]->SetLineColor(kBlue);
    v_wires[quotient]->SetLineWidth(3);
    v_circ->SetFillColor(kBlue);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  } 
}


void MWDChamber::y1WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****x1WireHit: "<<quotient<<endl;
#endif
    y1_wires[quotient]->SetLineColor(kRed);
    y1_wires[quotient]->SetLineWidth(3);
    y1_circ->SetFillColor(kRed);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
  
}


void MWDChamber::ypWireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****x2WireHit: "<<quotient<<endl;
#endif
    yp_wires[quotient]->SetLineColor(kRed+2);
    yp_wires[quotient]->SetLineWidth(3);
    yp_circ->SetFillColor(kRed+2);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber::clear()
{
  for (int i = 0; i<Num; i++)
  {
    u_wires[i]->SetLineColor(kGray);
    u_wires[i]->SetLineWidth(1);
    if (type==0) v_wires[i]->SetLineColor(kGray);
    if (type==0) v_wires[i]->SetLineWidth(1);
    y1_wires[i]->SetLineColor(kGray);
    y1_wires[i]->SetLineWidth(1);
    if (type==0) yp_wires[i]->SetLineColor(kGray);
    if (type==0) yp_wires[i]->SetLineWidth(1);
    x1_wires[i]->SetLineColor(kGray);
    x1_wires[i]->SetLineWidth(1);
    if (type==0) xp_wires[i]->SetLineColor(kGray);
    if (type==0) xp_wires[i]->SetLineWidth(1);
 
    u_circ->SetFillColor(kGray);
    if (type==0) v_circ->SetFillColor(kGray);
    y1_circ->SetFillColor(kGray);
    if (type==0) yp_circ->SetFillColor(kGray);
    x1_circ->SetFillColor(kGray);
    if (type==0) xp_circ->SetFillColor(kGray);
  }

	for (int i = 0; i<MAX_TRACK_NUM; i++)
	{
          track_circ[i]->SetX1(0.0);
	  track_circ[i]->SetY1(0.0);
	  track_circ[i]->SetR1(0.0);
	  track_circ[i]->SetR2(0.0);
	  track_circ[i]->SetFillColor(kWhite);
     	  track_circ[i]->Draw();
	}
}

void MWDChamber::Track(double x, double y, int i)
{
//#if DEBUG_LEVEL >= 3	
//	cout<<"HURA: TRACK: (x,y): "<<x<<", "<<y<<endl;
//#endif

	// First we need to transform meters to pixels
	double CX =  cst->transXtoCX(-y);
   	double CY =  cst->transYtoCY(-x);	

	track_circ[i]->SetX1(CX);
	track_circ[i]->SetY1(CY);
	track_circ[i]->SetR1(0.008);
	track_circ[i]->SetR2(0.008);
	//track_circ[i]->SetLineColor(kOrange+8);
	track_circ[i]->SetLineColor(1+i);
	track_circ[i]->SetLineWidth(2);
	//track_circ[i]->SetFillColor(kYellow);
	//track_circ[i]->SetFillColor(kOrange+0-i);
     	track_circ[i]->Draw();
	
  
}

