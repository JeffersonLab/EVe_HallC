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
    //cerr << "Path for " << n << " th track is "<< path << endl;
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
    //cerr << "Path for " << n << " th track is "<< path << endl;
    Enable(n,x,y,theta,phi);

    //cerr << "Track Num "<< n << " is created." << endl;
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

    //cerr<< "x = " << x << " ,y = " << y << endl;
    //cerr<< "C1x = " << C1x << " ,C1y= " << C1y <<endl;

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
    //cerr << "In canvas 1y = " << C1y << " , 1z= " << C1z << endl;

    tempx=C2x;tempy=C2y;tempz=C2z;
    C2x=tempz; C2y=-tempx; C2z=tempy;

     //Now apply transform to these 2 hit signal in 3D canvas
    // 1st rotate them according to tilt , then translate with their center position in canvas

    //Chamber1
    double C1tilt= HMS-> GetDouble("MWDC1.Tilt =");
    double C1xpos= HMS-> GetDouble("MWDC1.xPos =");
    double C1ypos= HMS-> GetDouble("MWDC1.yPos =");
    double C1zpos= HMS-> GetDouble("MWDC1.zPos =");

    //cerr << " Tilt of chamber 1 is : " << C1tilt << endl;

    tempx= C1x*cos(C1tilt)-C1z*sin(C1tilt);
    tempz= C1x*sin(C1tilt)+C1z*cos(C1tilt);

    C1x= tempx + C1xpos;
    C1y+= C1ypos;
    C1z= tempz + C1zpos;

    //FIXME: Why is the second chamber used at all?
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
    double xmin =-20.0;   // FIXME: where does this number come from?
    double B1x = xmin;
    double B1y = C1y + (C2y-C1y)/(C2x-C1x)*(xmin-C1x);
    double B1z = C1z + (C2z-C1z)/(C2x-C1x)*(xmin-C1x);

    double xmax =390.0;   // FIXME: where does this number come from?
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
    pn->Align(comb, new TGeoTube("Tube",0.0,1.0,Length/2.0));
}

void Trajectory3D::Disable()
{
    TGeoPhysicalNode* pn = Manager->MakePhysicalNode(path);
    // Or we can use SetTubeDimensions
    pn -> Align(new TGeoTranslation(0,0,0), new TGeoTube("Tube",0.0,0.0,0.0));
}
