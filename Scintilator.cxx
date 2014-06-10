
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
   // x in y sta koordinati spodnjega levega roba objekta. a in b pa 
   // dolocata vetrikalno in horizontalno velikost fotopomnozevalke.
   // !!! Koordinate tock so vedno med 0 in 1. 

   sx0 = x- a*0.21875;
   sy0 = y;
   sa = a;
   sb = b;
   paddle_length = length;

   // najprej naredimo levo fotopomnozevalko
   Double_t xL[7] = {a*0.0 + sx0 ,a*0.09375 + sx0 ,a*0.21875 + sx0 , a*0.21875 + sx0 , a*0.09375 + sx0 , a*0.0 + sx0 , a*0.0 + sx0 };
   Double_t yL[7] = {b*0.175 + y, b*0.175 + y, b*0.25 + y, b*0.0 + y, b*0.075 + y, b*0.075 + y, b*0.175 + y};
   plineL = new TPolyLine(7,xL,yL);
   plineL->SetFillColor(38);
   plineL->SetLineColor(1);
   plineL->SetLineWidth(1);
   plineL->Draw("f");
   plineL->Draw();

   // sedaj naredimo desno fotopomnozevalko

   Double_t xR[7] = {a*0.78125 + sx0 ,a*0.90625 + sx0 ,a*1.0 + sx0 ,a*1.0 + sx0 ,a*0.90625 + sx0 , a*0.78125 + sx0 , a*0.78125 + sx0 };
   Double_t yR[7] = {b*0.25 + y , b*0.175 + y, b*0.175 + y, b*0.075 + y, b*0.075 + y, b*0.0 + y, b*0.25 + y};

   plineR = new TPolyLine(7,xR,yR);
   plineR->SetFillColor(38);
   plineR->SetLineColor(1);
   plineR->SetLineWidth(1);
   plineR->Draw("f");
   plineR->Draw();

   // sedaj naredimo se scintilacijski material

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
  // ce komora ni bila zadeta vrne program 1E-35
  // ce je bila komora legitimno zadeta vrne program
  // stevilko, ki je manjsa od 0.
  // Program lahko vrne tudi nekaj kar je >0 
  // vendar tega program ne tretira, kot legitimni
  // dogodek
 
  // zadeta je bila le desna fotopomnozevalka
  if (left<min && right>min)
  {
    plineR->SetFillColor(2);
  } 

  // zadeta je bila le leva fotopomnozevalka
  if (left>min  && right<min )
  {
    plineL->SetFillColor(2);
  }

  // zadeti sta obe fotopomnozevalki
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
