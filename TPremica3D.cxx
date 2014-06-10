//************************************************************************* 
//  TPremica3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create and handle one 3D wire. We use these wires in 
//  class MWDChamber3D.cxx
//
//
//*************************************************************************  

#include "TPremica3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0


using namespace std;


TPremica3D::TPremica3D(double x1, double y1, double z1, double x2,  double y2, double z2, double radij, TGeoVolume *volume)
{	
	double x0 = (x1+x2)/2.0; 
	double y0 = (y1+y2)/2.0;
	double z0 = (z1+z2)/2.0;

	double theta = 180.0/3.141592654*atan2(sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)),(z2-z1));
	double phi = 180.0/3.141592654*atan2(y2- y1, x2 - x1);
	double length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

	
#if DEBUG_LEVEL >= 3
	cout<<"----> Length: "<<length<<endl;
	cout<<"----> Kot Theta je: "<<theta<<endl;
	cout<<"----> Kot Phi je: "<<phi<<endl;
#endif

      	TGeoTube *cev = new TGeoTube("cev",0.0, radij, length/2.0);
      	premica = new TGeoVolume("drotek",cev);
      	premica->SetLineColor(kBlack);
      	TGeoRotation r1;
      	r1.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t1(x0, y0,  z0);
      	TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1); 
      	volume->AddNode(premica,1,comb);

  
}

TPremica3D::~TPremica3D()
{
}

void TPremica3D::SetColor(Style_t barva)
{
  premica->SetLineColor(barva);
}
