//************************************************************************* 
//  FullTrajectory3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create, show and modify Full tracks in 3D view.
//  Full tracks are those tracks that have been fully reconstructed 
//  (we know their momenta in the target etc.)
//
// 
//
//*************************************************************************



#include "FullTrajectory3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>

#include "EVe_DB.h"

#define DEBUG_LEVEL  0


using namespace std;

FullTrajectory3D::FullTrajectory3D(char *ime, TGeoVolume *tvolume, TGeoManager *mgr, char *pot)
{
	manager = mgr;

// Fist part of the trajectory
	TGeoTube *tube = new TGeoTube("tube",0.0, 0.0, 0.0);
	
	for (int i = 0; i<100; i++) namea[i] = 0;
        strcat(namea,ime);
        strcat(namea, "a");


	for (int i = 0; i<100; i++) patha[i] = 0;
	strcat(patha, pot);
	strcat(patha, ime);
	strcat(patha, "a_1");		

#if DEBUG_LEVEL >= 3	
	cout<<"Name is: "<<namea<<"... path is: "<<patha<<endl;
#endif

       	TGeoVolume *trajectorya = new TGeoVolume(namea,tube);
       	trajectorya->SetLineColor(kGreen);	
       	tvolume->AddNodeOverlap(trajectorya,1);

// Second part of the trajectory

	for (int i = 0; i<100; i++) nameb[i] = 0;
        strcat(nameb,ime);
        strcat(nameb, "b");

	for (int i = 0; i<100; i++) pathb[i] = 0;
	strcat(pathb, pot);
	strcat(pathb, ime);
	strcat(pathb, "b_1");		

#if DEBUG_LEVEL >= 3	
	cout<<"Name is: "<<nameb<<"... path is: "<<pathb<<endl;
#endif

       	TGeoVolume *trajectoryb = new TGeoVolume(nameb,tube);
       	trajectoryb->SetLineColor(kGreen);	
       	tvolume->AddNodeOverlap(trajectoryb,1);


// Third part of the trajectory

	for (int i = 0; i<100; i++) namec[i] = 0;
        strcat(namec,ime);
        strcat(namec, "c");

	for (int i = 0; i<100; i++) pathc[i] = 0;
	strcat(pathc, pot);
	strcat(pathc, ime);
	strcat(pathc, "c_1");		
	
#if DEBUG_LEVEL >= 3
	cout<<"Name is: "<<namec<<"... path is: "<<pathc<<endl;
#endif

       	TGeoVolume *trajectoryc = new TGeoVolume(namec,tube);
       	trajectoryc->SetLineColor(kGreen);	
       	tvolume->AddNodeOverlap(trajectoryc,1);

}

FullTrajectory3D::~FullTrajectory3D()
{

}


void FullTrajectory3D::Track(double p0x, double p0y, double p0z, double xf, double yf, double zf, double thetaf, double phif)
{	
	// Here we draw the final part of the traj. from end of the magnet to the scint. planes
	double alpha = MWDC2_tilt*3.141592654/180.0;	

	double xD1 = xf;
        double yD1 = -yf;
        double zD1 = 0.0;

        double zD2 = MWDC2_z*100.0; // need to convert [m] to [cm]
        double xD2 = xD1 + tan(thetaf)*(zD2-zD1);
        double yD2 = yD1 + tan(-phif)*(zD2-zD1);

	
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

	double xA = x1;
	double yA = y1;
	double zA = z1;

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
	cout<<"Part One"<<endl;
	cout<<"----> Coordinates x0: "<<x0<<", y0: "<<y0<<", z0: "<<z0<<endl;
	cout<<"----> Length: "<<length<<endl;
	cout<<"----> Theta: "<<theta<<endl;
	cout<<"----> Phi: "<<phi<<endl;
#endif

        TGeoRotation r1;
      	r1.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t1(x0, y0,  z0);
        TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1); 
	TGeoPhysicalNode *pn = manager->MakePhysicalNode(patha);
	pn ->Align(comb,new TGeoTube("tube",0.0, 1, length/2.0));



	// Now we draw the part of the traj. from target to magnet
	
	// For now we have point target
	x1 = 0.0;
	y1 = 0.0;
	z1 = 0.0;

	x2 = x1 + magnet_face_dist; 
	y2 = y1 + -magnet_face_dist*p0x/p0z; 
	z2 = z1 + magnet_face_dist*p0y/p0z;

	x0 = (x1+x2)/2.0; 
	y0 = (y1+y2)/2.0;
	z0 = (z1+z2)/2.0;

	theta = 180.0/3.141592654*atan2(sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)),(z2-z1));
	phi = 180.0/3.141592654*atan2(y2- y1, x2 - x1);
	length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

#if DEBUG_LEVEL >= 3	
	cout<<"Part Two"<<endl;
	cout<<"----> Length: "<<length<<endl;
	cout<<"----> Theta: "<<theta<<endl;
	cout<<"----> Phi: "<<phi<<endl;
#endif

	TGeoRotation r2;
      	r2.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t2(x0, y0,  z0);
        TGeoCombiTrans *comb2 = new TGeoCombiTrans(t2, r2); 
	TGeoPhysicalNode *pn2 = manager->MakePhysicalNode(pathb);
	pn2 ->Align(comb2,new TGeoTube("tube",0.0, 1, length/2.0));

	// Finally we join both parts of the track together with the third part of the track
	// This part of the track is not physical, it's purpose is just to create a proper visual
	// effect.

	x1 = x2;
	y1 = y2;
	z1 = z2;

	x2 = xA;
	y2 = yA;
	z2 = zA;

	x0 = (x1+x2)/2.0; 
	y0 = (y1+y2)/2.0;
	z0 = (z1+z2)/2.0;

	theta = 180.0/3.141592654*atan2(sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)),(z2-z1));
	phi = 180.0/3.141592654*atan2(y2- y1, x2 - x1);
	length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

#if DEBUG_LEVEL >= 3
	cout<<"Part Three"<<endl;
	cout<<"----> Length: "<<length<<endl;
	cout<<"----> Theta: "<<theta<<endl;
	cout<<"----> Phi: "<<phi<<endl;
#endif
	
	TGeoRotation r3;
      	r3.SetAngles(theta-90,phi, 90,phi+90,theta,phi);
      	TGeoTranslation t3(x0, y0,  z0);
        TGeoCombiTrans *comb3 = new TGeoCombiTrans(t3, r3); 
	TGeoPhysicalNode *pn3 = manager->MakePhysicalNode(pathc);
	pn3 ->Align(comb3,new TGeoTube("tube",0.0, 1, length/2.0));
}



void FullTrajectory3D::ClearTrack()
{	
	TGeoPhysicalNode *pn1 = manager->MakePhysicalNode(patha);
	pn1 ->Align(new TGeoTranslation(0,0,0), new TGeoTube("tube",0.0, 0.0, 0.0));

	TGeoPhysicalNode *pn2 = manager->MakePhysicalNode(pathb);
	pn2 ->Align(new TGeoTranslation(0,0,0), new TGeoTube("tube",0.0, 0.0, 0.0));

	TGeoPhysicalNode *pn3 = manager->MakePhysicalNode(pathc);
	pn3 ->Align(new TGeoTranslation(0,0,0), new TGeoTube("tube",0.0, 0.0, 0.0));
}


