//************************************************************************* 
//  Trajectory3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create, show and modify partial tracks in 3D view.
//
// 
//
//************************************************************************* 

#include "Trajectory3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>

#include "EVe_DB.h"

#define DEBUG_LEVEL 0


using namespace std;

Trajectory3D::Trajectory3D(char *objectname, TGeoVolume *tvolume, TGeoManager *mgr, char *pot)
{
	for (int i = 0; i<100; i++) name[i] = 0;
       	strcat(name,objectname);
	
        manager = mgr;  
      	TGeoTube *tube = new TGeoTube("tube",0.0, 0.0, 0.0);	
       	TGeoVolume *trajectory = new TGeoVolume(name,tube);
       	trajectory->SetLineColor(kBlue);	
       	tvolume->AddNodeOverlap(trajectory,1);

	for (int i = 0; i<100; i++) path[i] = 0;
	strcat(path, pot);
	strcat(path, objectname);
	strcat(path, "_1");

#if DEBUG_LEVEL >= 3
	cout<<"Name is: "<<name<<"... path is: "<<path<<endl;
#endif
	
}

Trajectory3D::~Trajectory3D()
{

}


void Trajectory3D::Track(double xf, double yf, double zf, double thetaf, double phif)
{	
	double alpha = MWDC2_tilt*3.141592654/180.0;	

	double xD1 = xf;
        double yD1 = -yf;
        double zD1 = 0.0;

        double zD2 = MWDC2_z*100.0;  // ned to transform [m] to [cm]
        //double xD2 = xD1 + tan(thetaf)*(zD2-zD1);
        //double yD2 = yD1 + tan(-phif)*(zD2-zD1);
	double xD2 = xD1 + thetaf*(zD2-zD1);
        double yD2 = yD1 + (-phif)*(zD2-zD1);

	
  	// (zD2,xD2) --> (x2, z2) corresponds to second MWDC plane
  	double xb = zD2*cos(alpha) + xD2*sin(alpha) + MWDC1_xpos; 
  	double zb = (zD2*sin(alpha) - xD2*cos(alpha) + MWDC1_zpos);
	double yb = yD2 + MWDC1_ypos;

  	// (zD1,xD1) --> (x1, z1) corresponds to first MWDC plane
  	double xa = zD1*cos(alpha) + xD1*sin(alpha) + MWDC1_xpos;
  	double ya = yD1 + MWDC1_ypos;
  	double za = zD1*sin(alpha) - xD1*cos(alpha) + MWDC1_zpos;



	double x1 = xa - 70.0;
	double z1 = za - 70.0*(zb-za)/(xb-xa); 
	double y1 = ya - 70.0*(yb-ya)/(xb-xa);

	double x2 = xa + 150.0;
	double z2 = za + 150.0*(zb-za)/(xb-xa); 
	double y2 = ya + 150.0*(yb-ya)/(xb-xa);

	double x0 = (x1+x2)/2.0; 
	double y0 = (y1+y2)/2.0;
	double z0 = (z1+z2)/2.0;
	

	double theta = 180.0/3.141592654*atan2(sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)),(z2-z1));
	double phi = 180.0/3.141592654*atan2(y2- y1, x2 - x1);
	double length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

#if DEBUG_LEVEL >= 3
	cout<<"----> x0: "<<x0<<", y0: "<<y0<<", z0: "<<z0<<endl;
	cout<<"----> Length: "<<length<<endl;
	cout<<"----> Theta: "<<theta<<endl;
	cout<<"----> Phi: "<<phi<<endl;
#endif

        TGeoRotation r1;
      	r1.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t1(x0, y0,  z0);
        TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1); 
	TGeoPhysicalNode *pn = manager->MakePhysicalNode(path);
	pn ->Align(comb,new TGeoTube("tube",0.0, 1, length/2.0));	

}



void Trajectory3D::ClearTrack()
{	

	TGeoPhysicalNode *pn = manager->MakePhysicalNode(path);
	pn ->Align(new TGeoTranslation(0,0,0), new TGeoTube("tube",0.0, 0.0, 0.0));

}


