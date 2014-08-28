//************************************************************************* 
//  TWire3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create and handle one 3D wire. We use these wires in 
//  class MWDChamber3D.cxx
//
//
//*************************************************************************  

#include "TWire3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0


using namespace std;


TWire3D::TWire3D(double x1, double y1, double z1, double x2,  double y2, double z2, double radius, TGeoVolume *volume)
{	
	double x0 = (x1+x2)/2.0; 
	double y0 = (y1+y2)/2.0;
	double z0 = (z1+z2)/2.0;

	double theta = 180.0/3.141592654*atan2(sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)),(z2-z1));
	double phi = 180.0/3.141592654*atan2(y2- y1, x2 - x1);
	double length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));


      	TGeoTube *tube = new TGeoTube("tube",0.0, radius, length/2.0);
      	wire = new TGeoVolume("wires",tube);
      	wire->SetLineColor(kBlack);
      	TGeoRotation r1;
      	r1.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t1(x0, y0,  z0);
      	TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1); 
      	volume->AddNode(wire,1,comb);

  
}

TWire3D::~TWire3D()
{
}

void TWire3D::SetColor(Style_t colour)
{
  wire->SetLineColor(colour);
}
