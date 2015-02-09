///////////////////////////////////////
/*  ScintPlane3D.cxx  7/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddle3Ds to create a 3D plane

*/
/////////////////////////////////////


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
   int numPaddles = hms->GetInt(Form("%s.PN =",splaneName));
   N = numPaddles;
   double length = hms ->GetDouble(Form("%s.PaddleLength =",splaneName));
   double height = hms ->GetDouble(Form("%s.PaddleHeight =",splaneName));
   double thickness = hms ->GetDouble(Form("%s.PaddleThickness =",splaneName));

   

   // Draw the Scintillator Plane in sy configuration
   //TGeoBBox* SP = new TGeoBBox(splaneName,4.0*((double)numPaddles)*height ,1.2*thickness ,1.2*length);
   //test data
   TGeoBBox* SP = new TGeoBBox(splaneName,1.2*thickness ,4.0*((double)numPaddles)*height ,3.0*length);
   ScintPlane = new TGeoVolume((Form("%s.plane",splaneName)),SP);
   // Draw n paddles for a single sy plane
   for(int i = 0; i<numPaddles; i++)
   { /// FIXME: Better use vector to draw the whole plane, may use Volume Assenblies, also fix in ScintPlane3D.h
     paddle[i] = new ScintillatorPaddle3D(splaneName,i,numPaddles,
					  length, height, thickness,ScintPlane, numPMT);}
     ///So All Paddles are AddNoded to ScintVolume by calling SinctillatorPaddle3D}

    

    /// Make the Whole ScintPlan rotate according to if the plane is sx or sy
    double angle= hms->GetDouble(Form("%s.angle =",splaneName));
    
    TGeoRotation* scintrot= new TGeoRotation();
    scintrot->SetAngles(90, 0, angle, 90, 90-angle, -90);
    
    
    /// Make the Whole ScintPlane rotation and translation combination to settle up
    // transformation used for AddNode a single ScintPlane3D to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    // TGeoHMatrix TotRot;
    TGeoCombiTrans *comb;
    
    double tilt = hms-> GetDouble(Form("%s.tilt =",splaneName));
    double xpos = hms-> GetDouble(Form("%s.xpos =",splaneName));
    double ypos = hms-> GetDouble(Form("%s.ypos =",splaneName));
    double zpos = hms-> GetDouble(Form("%s.zpos =",splaneName));

    cerr << Form("%s.xpos is ",splaneName) << xpos <<  Form(" %s.ypos is ",splaneName) << ypos << Form(" %s.zpos is ",splaneName) << zpos <<endl;

    TGeoBBox* Box = new TGeoBBox(splaneName,1.5*thickness ,4.5*((double)numPaddles)*height ,4.0*length);
    TGeoVolume *SBox = new TGeoVolume(Form("%s.Box",splaneName),Box);
    SBox->AddNode(ScintPlane,1,scintrot);
    r1.SetAngles(90 - tilt,0,90,90,tilt,180);
    //TotRot = scintrot*r1;
    t1.SetTranslation(xpos, ypos, zpos);
    comb = new TGeoCombiTrans(t1, r1); 
    top->AddNodeOverlap(SBox,1,comb);

    cout<<"Scintillation Plane 3D is created!"<<endl;
}

ScintPlane3D::~ScintPlane3D()
{

}

//Using new hit method same as planar view

void ScintPlane3D::LHit(int numL)
{
  if(numL<0) // cerr<< "negative left hit index for scintplane"<< endl;
    ;
  else paddle[numL]->HitL();
}

void ScintPlane3D::RHit(int numR)
{
  if(numR<0) // cerr<< "negative right hit index for scintplane"<< endl;
    ;
  else paddle[numR]->HitR();
}

void ScintPlane3D::BHit(int numB)
{
  if(numB<0) // cerr<< "negative both hit index for scintplane"<< endl;
    ;
  else paddle[numB]->HitB();
}

void ScintPlane3D::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }
}

void ScintPlane3D :: SPHit(int Num, double poshit[], double neghit[])
{
   double matchR[16];
   double matchL[16];

   for (Int_t q = 0; q<Num; q++)
     {
       matchR[q] = poshit[q];
       RHit(poshit[q]-1);
     }

   for (Int_t q=0;q<Num;q++)
     {
       matchL[q]= neghit[q];
       LHit(neghit[q]-1);
     }

   for (Int_t i=0;i<16;i++) {
     for (Int_t j=0;j<16;j++) {
       if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {

	 BHit(matchR[i]-1);
       } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
	 BHit(matchL[i]-1);
       }
     }  
     matchR[i]=0;
     matchL[i]=0;}
 }
