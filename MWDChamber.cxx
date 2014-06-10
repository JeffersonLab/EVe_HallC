
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

MWDChamber::MWDChamber(char *ime, int n, double Hight, CStransform *trans, int t)
{
   type = t;

   cst = trans;

   double x =  cst->transXtoCX(0.0) - cst->transLtoCL(Hight)/8;
   double y =  cst->transYtoCY(0.0) - cst->transLtoCL(Hight)/2;

#if DEBUG_LEVEL >= 3	
   cout<<"x "<<x<<" y "<<y<<endl;	
#endif

   double a = cst->transLtoCL(Hight); 
   double b = cst->transLtoCL(Hight);

   fa = a;
   fb = b;

   // n = actual number of wires
   // Num = number ow wires that we draw

   if ((n/5.0 - int(n/5.0))>0) Num =  (int) (n/5.0) + 1; // must be rounded up
   else Num = (int) (n/5.0);

#if DEBUG_LEVEL >= 3	
   cout<<"IMAMO n = "<<n<<", Num = "<<Num<<endl;
#endif
  
   double LL = 0.25;
   double HH = 1.0;


   Double_t xokvir[5] = {a*0.0-0.05*a+x, a*0.0-0.05*a+x, a*LL+0.05*a+x, a*LL+0.05*a+x, a*0.0-0.05*a+x};
   Double_t yokvir[5] = {b*0.0-0.05*b+y, b*HH+0.05*b+y, b*HH+0.05*b+y, b*0.0-0.05*b+y, b*0.0-0.05*b+y};
   okvir = new TPolyLine(5,xokvir,yokvir);
   okvir->SetFillColor(38);
   okvir->SetLineColor(1);
   okvir->SetLineWidth(1);
   okvir->Draw("f");
   okvir->Draw();


   Double_t xokvir2[5] = {a*0.0+x, a*0.0+x, a*LL+x, a*LL+x, a*0.0+x};
   Double_t yokvir2[5] = {b*0.0+y,b*HH+y, b*HH+y, b*0.0+y, b*0.0+y};
   okvir2 = new TPolyLine(5,xokvir2,yokvir2);
   okvir2->SetFillColor(0);
   okvir2->SetLineColor(1);
   okvir2->SetLineWidth(1);
   okvir2->Draw("f");
   okvir2->Draw();
  
   // Najprej komora s po dvema nivojema zic

  
   if (type==0)
   {

     // sedaj naredimo prvo ravnino x-drotkov
     for (int i = 0; i<Num; i++)
     {
	double fac = (HH)*(i)/(Num);
        x1_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        x1_wires[i]->SetLineColor(kGray);
        x1_wires[i]->Draw(); 
	
     }
     // sedaj naredimo drugo ravnino x-drotkov
     for (int i = 0; i<Num; i++)
     {
        double fac =  (HH)/Num/2.0 + (HH)*(i)/(Num);
        x2_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        x2_wires[i]->SetLineColor(kGray);
        x2_wires[i]->Draw(); 
	
    }
     // sedaj naredimo prvo ravnino v-drotkov
     for (int i = 0; i<Num; i++)
     {
	double cn =- (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  v1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  v1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
	}
	else
	{
	  v1_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
	}
     }

     // sedaj naredimo drugo ravnino v-drotkov
     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/Num/2 - (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  v2_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          v2_wires[i]->SetLineColor(kGray);
          v2_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  v2_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          v2_wires[i]->SetLineColor(kGray);
          v2_wires[i]->Draw();   
	}
	else
	{
	  v2_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          v2_wires[i]->SetLineColor(kGray);
          v2_wires[i]->Draw();   
	}

     }


     // sedaj naredimo prvo ravnino u-drotkov

     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);
	//double cn = -(HH*2.0/sqrt(3.))/(Num)*7.0/2.0 + (HH*2.0/sqrt(3.))/(Num)*(i);
	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  u1_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();  	  
	}
	else
	{
	  u1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();   
	}
     }


     // sedaj naredimo drugo ravnino v-drotkov

     for (int i = 0; i<Num; i++)
     {
	double cn =-(HH*2.0/sqrt(3.))/(Num)/2 -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);


	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  u2_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          u2_wires[i]->SetLineColor(kGray);
          u2_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  u2_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          u2_wires[i]->SetLineColor(kGray);
          u2_wires[i]->Draw();  	  
	}
	else
	{
	  u2_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          u2_wires[i]->SetLineColor(kGray);
          u2_wires[i]->Draw();   
	}
     }

     // sedaj naredimo se semaforcek

     u1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8+y, a*0.01, b*0.01 );
     u1_circ->SetFillColor(kGray);
     u1_circ->Draw();

     TLatex *u1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79+y, "U1");
     u1_title->SetTextSize(0.02);
     u1_title->Draw(); 
 
     u2_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.03+y, a*0.01, b*0.01 );
     u2_circ->SetFillColor(kGray);
     u2_circ->Draw();

     TLatex *u2_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.03+y, "U2");
     u2_title->SetTextSize(0.02);
     u2_title->Draw(); 

     v1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.06+y, a*0.01, b*0.01 );
     v1_circ->SetFillColor(kGray);
     v1_circ->Draw();

     TLatex *v1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.06+y, "V1");
     v1_title->SetTextSize(0.02);
     v1_title->Draw(); 

     v2_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.09+y, a*0.01, b*0.01 );
     v2_circ->SetFillColor(kGray);
     v2_circ->Draw();

     TLatex *v2_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.09+y, "V2");
     v2_title->SetTextSize(0.02);
     v2_title->Draw(); 

     x1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.12+y, a*0.01, b*0.01 );
     x1_circ->SetFillColor(kGray);
     x1_circ->Draw();

     TLatex *x1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.12+y, "X1");
     x1_title->SetTextSize(0.02);
     x1_title->Draw(); 

     x2_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.15+y, a*0.01, b*0.01 );
     x2_circ->SetFillColor(kGray);
     x2_circ->Draw();

     TLatex *x2_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.15+y, "X2");
     x2_title->SetTextSize(0.02);
     x2_title->Draw(); 

   }
   else if (type==1) /// SECON TYPE
   {

     // sedaj naredimo prvo ravnino x-drotkov
     for (int i = 0; i<Num; i++)
     {
	double fac = (HH)*(i)/(Num);
        x1_wires[i] = new TLine(a*0.0+x, b*(HH-fac)+y, a*LL+x, b*(HH-fac)+y);
        x1_wires[i]->SetLineColor(kGray);
        x1_wires[i]->Draw(); 
	
     }

     // sedaj naredimo prvo ravnino v-drotkov
     for (int i = 0; i<Num; i++)
     {
	double cn =- (HH*2.0/sqrt(3.))/(Num)*(i) + HH;

        if (cn <= HH - sqrt(3.)/3.0*LL && cn>=0.0 )
        {
	  v1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(sqrt(3.)/3.0*LL+ cn)+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
        }
        else if (cn>HH - sqrt(3.)/3.0*LL)
	{
	  v1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(HH-cn)*3.0/sqrt(3.)+x, b*HH+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
	}
	else
	{
	  v1_wires[i] = new TLine(a*(3.0/sqrt(3.)*(-cn))+x, HH*0.0+y, a*LL+x, b*(LL -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y);
          v1_wires[i]->SetLineColor(kGray);
          v1_wires[i]->Draw();   
	}
     }

  
     // sedaj naredimo prvo ravnino u-drotkov

     for (int i = 0; i<Num; i++)
     {
	double cn = -(HH*2.0/sqrt(3.))/(Num)*(i) + HH+(LL*sqrt(3.)/3.0);
	//double cn = -(HH*2.0/sqrt(3.))/(Num)*7.0/2.0 + (HH*2.0/sqrt(3.))/(Num)*(i);
	if (cn < HH && cn >LL*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*LL+x, b*(-LL*sqrt(3.)/3.0+ cn)+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();   
	}
	else if (cn>=HH)
	{
	  u1_wires[i] = new TLine(a*(cn-HH)*3.0/sqrt(3.0)+x, HH*b +y, a*LL+x, b*(-sqrt(3.)/3.0*LL+ cn)+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();  	  
	}
	else
	{
	  u1_wires[i] = new TLine(a*0.0+x, b*cn+y, a*(cn*3.0/sqrt(3.0))+x, b*0.0+y);
          u1_wires[i]->SetLineColor(kGray);
          u1_wires[i]->Draw();   
	}
     }



    // sedaj naredimo se semaforcek

     u1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8+y, a*0.01, b*0.01 );
     u1_circ->SetFillColor(kGray);
     u1_circ->Draw();

     TLatex *u1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79+y, "U");
     u1_title->SetTextSize(0.02);
     u1_title->Draw(); 

     v1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.03+y, a*0.01, b*0.01 );
     v1_circ->SetFillColor(kGray);
     v1_circ->Draw();


     TLatex *v1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.03+y, "V");
     v1_title->SetTextSize(0.02);
     v1_title->Draw(); 

    
     TLatex *x1_title = new TLatex(a*((LL+0.02+0.05) + 0.015) +x,b*0.79 - 0.06+y, "X");
     x1_title->SetTextSize(0.02);
     x1_title->Draw(); 

     x1_circ = new TEllipse(a*(LL+0.02+0.05)+x,b*0.8-0.06+y, a*0.01, b*0.01 );
     x1_circ->SetFillColor(kGray);
     x1_circ->Draw();

 
   }

   title = new TLatex(x,b*1.06+y, ime);
   title->SetTextSize(0.03);
   title->Draw(); 

   for(int i = 0; i<MAX_TRACK_NUM; i++) track_circ[i] = new TEllipse(0,0,0);
  
   cout<<"MWDChamber is created!"<<endl;
}


MWDChamber::~MWDChamber()
{

}

void MWDChamber::u1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****u1WireHit: "<<kvocient<<endl;
#endif
    u1_wires[kvocient]->SetLineColor(kGreen);
    u1_wires[kvocient]->SetLineWidth(3);
    u1_circ->SetFillColor(kGreen);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber::u2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****u2WireHit: "<<kvocient<<endl;
#endif
    u2_wires[kvocient]->SetLineColor(kGreen+3);
    u2_wires[kvocient]->SetLineWidth(3);
    u2_circ->SetFillColor(kGreen+3);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber::v1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num )
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****v1WireHit: "<<kvocient<<endl;
#endif
    v1_wires[kvocient]->SetLineColor(kAzure+8);
    v1_wires[kvocient]->SetLineWidth(3);
    v1_circ->SetFillColor(kAzure+8);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber::v2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****v2WireHit: "<<kvocient<<endl;
#endif
    v2_wires[kvocient]->SetLineColor(kBlue);
    v2_wires[kvocient]->SetLineWidth(3);
    v2_circ->SetFillColor(kBlue);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  } 
}


void MWDChamber::x1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****x1WireHit: "<<kvocient<<endl;
#endif
    x1_wires[kvocient]->SetLineColor(kRed);
    x1_wires[kvocient]->SetLineWidth(3);
    x1_circ->SetFillColor(kRed);
  }
  else
  {
#if DEBUG_LEVEL >= 3	
    cout<<"No wire with this number"<<endl;
#endif
  }
  
}


void MWDChamber::x2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3	
    cout<<"****x2WireHit: "<<kvocient<<endl;
#endif
    x2_wires[kvocient]->SetLineColor(kRed+2);
    x2_wires[kvocient]->SetLineWidth(3);
    x2_circ->SetFillColor(kRed+2);
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
    u1_wires[i]->SetLineColor(kGray);
    u1_wires[i]->SetLineWidth(1);
    if (type==0) u2_wires[i]->SetLineColor(kGray);
    if (type==0) u2_wires[i]->SetLineWidth(1);
    v1_wires[i]->SetLineColor(kGray);
    v1_wires[i]->SetLineWidth(1);
    if (type==0) v2_wires[i]->SetLineColor(kGray);
    if (type==0) v2_wires[i]->SetLineWidth(1);
    x1_wires[i]->SetLineColor(kGray);
    x1_wires[i]->SetLineWidth(1);
    if (type==0) x2_wires[i]->SetLineColor(kGray);
    if (type==0) x2_wires[i]->SetLineWidth(1);
 
    u1_circ->SetFillColor(kGray);
    if (type==0) u2_circ->SetFillColor(kGray);
    v1_circ->SetFillColor(kGray);
    if (type==0) v2_circ->SetFillColor(kGray);
    x1_circ->SetFillColor(kGray);
    if (type==0) x2_circ->SetFillColor(kGray);
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
	cout<<"HURA: TRACK: (x,y): "<<x<<", "<<y<<endl;
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

