///////////////////////////////////////
/*  ScintPlane3D.cxx  7/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddle3Ds to create a 3D plane

*/
///////////////////////////////////////
/*
#include "ScintPlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

ScintPlane3D::ScintPlane3D(char *name, int n, double x, double y, double z, double length, double height, double thickness, TGeoVolume *paddles, int horizontal)
{
   N = n;
   GetVariables *ptr = new GetVariables("HMS.txt");
   int number = ptr->GetInt("Number of paddle PMTs =");
   int rot;
   if (horizontal == 1) {
     rot = 1;
   }
   else {
     rot = 0;
   }

   // Determine center of scint plane
 
   double sy0 = y;
   double sx0 = x - (height*(n)/2.0 - height/2.0);
   double sz0 = z;

   /// FIXME:: Need to incorporate a translation into rotation planes
   for(int i = 0; i<n; i++)
   {
     if (rot ==1) {
       paddle[i] = new ScintillatorPaddle3D(i, height*i+sx0, 0.0+sy0, 0.0+sz0, length, height, thickness, paddles, number, rot);
     }
     else {
       paddle[i] = new ScintillatorPaddle3D(i, height*i-sx0, 0.0+sy0, 0.0+sz0, length, height, thickness, paddles, number, rot);
     }
   }
   
   cout<<"Scintillation Plane 3D is created!"<<endl;
}

ScintPlane3D::~ScintPlane3D()
{

}

// FIXME:: Need to implement new tdchit method as in ScintPlane.cxx
void ScintPlane3D::paddleHit(int num, double left, double right)
{
  GetVariables *ptr = new GetVariables("HMS.txt");
  int numb = ptr->GetInt("Number of Paddle PMTs =");
  if (num<N)
    {
      
      paddle[num]->hit(left,right,numb);
    }
}

void ScintPlane3D::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }
}
*/

#include "ScintPlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include "TGeoMedium.h"

using namespace std;

ScintPlane3D::ScintPlane3D(char* splaneName, TGeoVolume* top)
{
   GetVariables *hms = new GetVariables("HMS.txt");
   
   // Get Values used in construct n paddles for a single ScintPlane
   int numPMT = hms->GetInt("Number of paddle PMTs =");
   double numPaddles = hms->GetInt(Form("%s.PN =",splaneName));
   double length = hms ->GetDouble(Form("%s.PaddleLength =",splaneName));
   double height = hms ->GetDouble(Form("%s.PaddleHeight =",splaneName));
   double thickness = hms ->GetDouble(Form("%s.PaddleThickness =",splaneName));

   

   // Draw the Scintillator Plane in sy configuration
   TGeoBBox* SP = new TGeoBBox(splaneName,111 ,5 ,100 );
   ScintPlane = new TGeoVolume((Form("%s.plane",splaneName)),SP);
   // Draw n paddles for a single sy plane
   for(int i = 0; i<numPaddles; i++)
   { /// FIXME: Better use vector to draw the whole plane, may use Volume Assenblies, also fix in ScintPlane3D.h
     paddle[i] = new ScintillatorPaddle3D(i,numPaddles,
					  length, height, thickness,ScintPlane, numPMT);}
     ///So All Paddles are AddNoded to ScintVolume by calling SinctillatorPaddle3D}

    

    /// Make the Whole ScintPlan rotate according to if the plane is sx or sy
    double angle= hms->GetDouble(Form("%s.angle =",splaneName));
    scintrot.SetAngles(angle, 0, 90, 90, angle-90, 0);
    
    /// Make the Whole ScintPlane rotation and translation combination to settle up
    // transformation used for AddNode a single ScintPlane3D to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    TGeoHMatrix TotRot;
    TGeoCombiTrans *comb;
    
    double tilt = hms-> GetDouble(Form("%s.tilt =",splaneName));
    double xpos = hms-> GetDouble(Form("%s.xpos =",splaneName));
    double ypos = hms-> GetDouble(Form("%s.ypos =",splaneName));
    double zpos = hms-> GetDouble(Form("%s.zpos =",splaneName));


    r1.SetAngles(180 - tilt,0,90 - tilt,0,90,90);
    TotRot = scintrot*r1;
    t1.SetTranslation(xpos, ypos, zpos);
    comb = new TGeoCombiTrans(t1, TotRot); 
    top->AddNodeOverlap(ScintPlane,1,comb);

    cout<<"Scintillation Plane 3D is created!"<<endl;
}

ScintPlane3D::~ScintPlane3D()
{

}

//Using new hit method same as planar view

void ScintPlane3D::LHit(int numL)
{
  if(numL<0) cerr<< "negative hit index for scintplane";
  else paddle[numL]->HitL();
}

void ScintPlane3D::RHit(int numR)
{
  if(numR<0) cerr<< "negative hit index for scintplane";
  else paddle[numR]->HitR();
}

void ScintPlane3D::BHit(int numB)
{
  if(numB<0) cerr<< "negative hit index for scintplane";
  else paddle[numB]->HitB();
}

void ScintPlane3D::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }
}

