///////////////////////////////////////
/*  CalorimeterPlane3D.cxx  6/24/15

    Christian Davison & Mitchell Kerver

    Class that creates an array of calorimeter blocks to create a 3D plane

*/
///////////////////////////////////////


#include "ScintPlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include "TGeoMedium.h"
#include "CalorimeterPlane3D.h"

using namespace std;

CalorimeterPlane3D::CalorimeterPlane3D(char* splaneName, GetVariables* DB, TGeoVolume* top,int shift)
{
    // Get Values used in construct n paddles for a single ScintPlane
    int numPMT = 1;//DB->GetInt("Number of paddle PMTs =");

//string stringName;
std::string stringName(splaneName);
std::cout<<stringName;
for(int i=0;i<5;i++){
if(stringName.compare(Form("Cal%dx",i))==0){
  numPMT=1;
  break;
  }
}
    numPaddles = DB->GetInt(Form("%s.PN =",splaneName));
    double length = DB ->GetDouble(Form("%s.PaddleLength =",splaneName));
    double height = DB ->GetDouble(Form("%s.PaddleHeight =",splaneName));
    double thickness = DB ->GetDouble(Form("%s.PaddleThickness =",splaneName));

    // Draw the Scintillator Plane in sy configuration
    TGeoBBox* SP = new TGeoBBox(splaneName,1.2*thickness ,4.0*((double)numPaddles)*height ,3.0*length);
    ScintPlane = new TGeoVolume((Form("%s.plane",splaneName)),SP);
    // Draw n paddles for a single sy plane
    for(int i = 0; i<numPaddles; i++)
        //FIXME: Better use vector to draw the whole plane, may use Volume
        //  Assenblies, also fix in ScintPlane3D.h
         paddle.push_back ( new CalorimeterBlock3D(splaneName, i, numPaddles,
                           length, height, thickness,ScintPlane, numPMT, shift) );

    /// Make the Whole ScintPlan rotate according to if the plane is sx or sy
    double angle= DB->GetDouble(Form("%s.angle =",splaneName));
//zrotation variable rotates the calorimeter plane through the xy-plane
    double zrotation = DB->GetDouble(Form("%s.zrotation =",splaneName));

    TGeoRotation* scintrot= new TGeoRotation();

    //scintrot->SetAngles(90, 0, angle, 90, 90-angle, -90);

//SetAngles rotates the calorimeter planes
scintrot->SetAngles(zrotation,90,0);


//if(stringName.compare("Cal4x")==0){
//cintrot->SetAngles(90,90,0);
//
//



    /// Make the Whole ScintPlane rotation and translation combination to settle up
    // transformation used for AddNode a single ScintPlane3D to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    TGeoCombiTrans *comb;

    double tilt = DB-> GetDouble(Form("%s.tilt =",splaneName));
    double xpos = DB-> GetDouble(Form("%s.xpos =",splaneName));
    double ypos = DB-> GetDouble(Form("%s.ypos =",splaneName));

//FIXMEEEE pass in new parameter? calorimeters facing each other(alternating) or same direction

//If the calorimeter plane gets rotated, then the x-direction does not change, and all the calorimeter planes get put next to one another in the y-plane
 if(zrotation!=0){
   ypos=ypos+thickness*shift/2;
}

else{
//Shift variable is equal to the index of the for loop in detector 3D
//Every even indexed plane gets shifted in the x-direction so that it gets lined up behind the previous even indexed plane

   if(shift%2==0){
     xpos=xpos+10*shift;
    }

//The odd plane gets shifted the same amount as the previous even plane so that they are lined up next to one another in the x-direction

   if(shift%2!=0){
     xpos=xpos+(shift-1)*10;
    }

   // double ypos = DB-> GetDouble(Form("%s.ypos =",splaneName));

//Every odd indexed plane gets moved in the y-direction so that it can be placed directly next to one another in the y-direction

   if(shift%2!=0)
   {
      ypos=-ypos;
   }
}
    double zpos = DB-> GetDouble(Form("%s.zpos =",splaneName));

    //cerr << Form("%s.xpos is ",splaneName) << xpos <<  Form(" %s.ypos is ",splaneName) << ypos << Form(" %s.zpos is ",splaneName) << zpos <<endl;

    TGeoBBox* Box = new TGeoBBox(splaneName,1.5*thickness ,4.5*((double)numPaddles)*height ,4.0*length);
    TGeoVolume *SBox = new TGeoVolume(Form("%s.Box",splaneName),Box);
    SBox->AddNode(ScintPlane,1,scintrot);
    r1.SetAngles(90 - tilt,0,90,90,tilt,180);
    t1.SetTranslation(xpos, ypos, zpos);
    comb = new TGeoCombiTrans(t1, r1);
    top->AddNodeOverlap(SBox,1,comb);

    cout<<"  Calorimeter Plane 3D is created!"<<endl;
}

CalorimeterPlane3D::~CalorimeterPlane3D()
{

}

void CalorimeterPlane3D::LHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "left hit index out of bounds in scintplane3D: "<< num << endl;
    else
        paddle[num]->HitL();
}

void CalorimeterPlane3D::RHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "right hit index out of bounds in scintplane3D: "<< num << endl;
    else
        paddle[num]->HitR();
}

void CalorimeterPlane3D::BHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "paddle hit index out of bounds in scintplane3D: "<< num << endl;
    else {
        paddle[num]->HitL();
        paddle[num]->HitR();
        paddle[num]->HitPaddle();
    }
}

void CalorimeterPlane3D::clear()
{
    for(int i = 0; i<numPaddles; i++) {
        paddle[i]->clear();
    }
}

void CalorimeterPlane3D::SPHit(int NumL, int NumR, double poshit[], double neghit[])
{
    for (int rh=0; rh<NumL; rh++) {
        RHit(poshit[rh]-1);
    }

    for (int lh=0; lh<NumR; lh++) {
        LHit(neghit[lh]-1);
    }

    for (int rh=0; rh<NumR; rh++) {
        for (int lh=0; lh<NumL; lh++) {
            if(poshit[lh]==neghit[rh]) BHit(poshit[lh]-1);
        }
    }
}
