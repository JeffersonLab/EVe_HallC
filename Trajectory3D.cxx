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
/*
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

*/
#include "TGeoPhysicalNode.h"
#include "Trajectory3D.h"
#include "TMath.h"
#include "GetVariables.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

Trajectory3D::Trajectory3D(TGeoVolume* Top, TGeoManager* Mgr, int n)
{
    Manager=Mgr;
    TGeoTube* TrackRay = new TGeoTube (Form("TrackrayNum %d",n), 0.0 ,0.0, 0.0);
    Ray = new TGeoVolume(Form("TrackRayNum%d",n),TrackRay);
    Ray ->SetLineColor(n+2);
    Top->AddNodeOverlap(Ray,1);
    path = Form("/TOP_1/TrackRayNum%d_1",n);
    cerr << "Path for " << n << " th track is "<< path << endl;
}


Trajectory3D::Trajectory3D(TGeoVolume* Top, TGeoManager* Mgr, int n,
               double x, double y, double theta, double phi)
{
    Manager=Mgr;
    TGeoTube* TrackRay = new TGeoTube (Form("TrackrayNum %d",n), 0.0 ,0.0, 0.0);
    Ray = new TGeoVolume(Form("TrackRayNum%d",n),TrackRay);
    Ray ->SetLineColor(n+2);
    Top->AddNodeOverlap(Ray,1);
    path = Form("/TOP_1/TrackRayNum%d_1",n);
    cerr << "Path for " << n << " th track is "<< path << endl;
    Enable(n,x,y,theta,phi);

    cerr << "Track Num "<< n << " is created." << endl;
}

Trajectory3D::~Trajectory3D()
{

}

void Trajectory3D::Enable(int n, double x, double y, double theta, double phi)
{
    GetVariables* HMS= new GetVariables("HMS.txt");

    double C1x = -y;
    double C1y = -x;
    double C1z = 0.0;

    cerr<< "x = " << x << " ,y = " << y << endl;
    cerr<< "C1x = " << C1x << " ,C1y= " << C1y <<endl;

    double xdiff = HMS-> GetDouble("MWDC2.xPos =")- HMS-> GetDouble("MWDC1.xPos =");
    double ydiff = HMS-> GetDouble("MWDC2.yPos =")- HMS-> GetDouble("MWDC1.yPos =");
    double zdiff = HMS-> GetDouble("MWDC2.zPos =")- HMS-> GetDouble("MWDC1.zPos =");
    double Dist = sqrt(xdiff*xdiff+ydiff*ydiff+zdiff*zdiff);

    double C2z = 0.0;
    double C2x = C1x + (-phi)*Dist;
    double C2y = C1y + (-theta)*Dist;

    //Change to Canvas Coordinates: (x,y,z)->(z,x,y)
    double tempx,tempy,tempz;
    tempx=C1x;tempy=C1y;tempz=C1z;
    C1x=tempz; C1y=-tempx; C1z=tempy;
    cerr << "In canvas 1y = " << C1y << " , 1z= " << C1z << endl;

    tempx=C2x;tempy=C2y;tempz=C2z;
    C2x=tempz; C2y=-tempx; C2z=tempy;

     //Now apply transform to these 2 hit signal in 3D canvas
    // 1st rotate them according to tilt , then translate with their center position in canvas

    //Chamber1
    double C1tilt= HMS-> GetDouble("MWDC1.Tilt =");
    double C1xpos= HMS-> GetDouble("MWDC1.xPos =");
    double C1ypos= HMS-> GetDouble("MWDC1.yPos =");
    double C1zpos= HMS-> GetDouble("MWDC1.zPos =");

    cerr << " Tilt of chamber 1 is : " << C1tilt << endl;

    tempx= C1x*cos(C1tilt)-C1z*sin(C1tilt);
    tempz= C1x*sin(C1tilt)+C1z*cos(C1tilt);

    C1x= tempx + C1xpos;
    C1y+= C1ypos;
    C1z= tempz + C1zpos;

    //Chamber2
    double C2tilt= HMS-> GetDouble("MWDC2.Tilt =");
    double C2xpos= HMS-> GetDouble("MWDC2.xPos =");
    double C2ypos= HMS-> GetDouble("MWDC2.yPos =");
    double C2zpos= HMS-> GetDouble("MWDC2.zPos =");

    tempx= C2x*cos(C2tilt)-C2z*sin(C2tilt);
    tempz= C2x*sin(C2tilt)+C2z*cos(C2tilt);

    C2x= tempx + C2xpos;
    C2y+=  C2ypos;
    C2z= tempz + C2zpos;

    //We want draw the tracking ray till the boundary of the canvas
    // so compute the ray position on the boundaries
    double xmin =0.0;
    double B1x = xmin;
    double B1y = C1y + (C2y-C1y)/(C2x-C1x)*(xmin-C1x);
    double B1z = C1z + (C2z-C1z)/(C2x-C1x)*(xmin-C1x);

    double xmax =370.0;
    double B2x = xmax;
    double B2y = C2y + (C2y-C1y)/(C2x-C1x)*(xmax-C2x);
    double B2z = C2z + (C2z-C1z)/(C2x-C1x)*(xmax-C2x);

    //now we draw the ray from (B1x,B1y,B1z) to (B2x,B2y,B2z)
    //Center of the ray
    double x0 = (B1x+B2x)/2;
    double y0 = (B1y+B2y)/2;
    double z0 = (B1z+B2z)/2;

    double Theta = 180.0/3.141592654*atan2(sqrt((B2y-B1y)*(B2y-B1y) + (B2x-B1x)*(B2x-B1x)),(B2z-B1z));
    double Phi = 180.0/3.141592654*atan2(B2y- B1y, B2x - B1x);
    double Length = sqrt((B2x-B1x)*(B2x-B1x) + (B2y-B1y)*(B2y-B1y) + (B2z-B1z)*(B2z-B1z));

    TGeoRotation r1;
    r1.SetAngles(Theta-90,Phi, 90,Phi+90,Theta,Phi);
    TGeoTranslation t1(x0, y0,  z0);
    TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1);

    TGeoPhysicalNode* pn = Manager->MakePhysicalNode(path);
    pn ->Align(comb, new TGeoTube("Tube",0.0,1.0,Length/2.0));
}

void Trajectory3D::Disable()
{
    TGeoPhysicalNode* pn = Manager->MakePhysicalNode(path);
    // Or we can use SetTubeDimensions
    pn -> Align(new TGeoTranslation(0,0,0), new TGeoTube("Tube",0.0,0.0,0.0));
}
