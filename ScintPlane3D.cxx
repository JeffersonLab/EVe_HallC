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

ScintPlane3D::ScintPlane3D(char* splaneName, GetVariables* DB, TGeoVolume* top)
{
    // Get Values used in construct n paddles for a single ScintPlane
    int numPMT = DB->GetInt("Number of paddle PMTs =");
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
        paddle.push_back ( new ScintillatorPaddle3D(splaneName, i, numPaddles,
                           length, height, thickness,ScintPlane, numPMT) );

    /// Make the Whole ScintPlan rotate according to if the plane is sx or sy
    double angle= DB->GetDouble(Form("%s.angle =",splaneName));

    TGeoRotation* scintrot= new TGeoRotation();
    scintrot->SetAngles(90, 0, angle, 90, 90-angle, -90);

    /// Make the Whole ScintPlane rotation and translation combination to settle up
    // transformation used for AddNode a single ScintPlane3D to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    TGeoCombiTrans *comb;

    double tilt = DB-> GetDouble(Form("%s.tilt =",splaneName));
    double xpos = DB-> GetDouble(Form("%s.xpos =",splaneName));
    double ypos = DB-> GetDouble(Form("%s.ypos =",splaneName));
    double zpos = DB-> GetDouble(Form("%s.zpos =",splaneName));

    //cerr << Form("%s.xpos is ",splaneName) << xpos <<  Form(" %s.ypos is ",splaneName) << ypos << Form(" %s.zpos is ",splaneName) << zpos <<endl;

    TGeoBBox* Box = new TGeoBBox(splaneName,1.5*thickness ,4.5*((double)numPaddles)*height ,4.0*length);
    TGeoVolume *SBox = new TGeoVolume(Form("%s.Box",splaneName),Box);
    SBox->AddNode(ScintPlane,1,scintrot);
    r1.SetAngles(90 - tilt,0,90,90,tilt,180);
    t1.SetTranslation(xpos, ypos, zpos);
    comb = new TGeoCombiTrans(t1, r1);
    top->AddNodeOverlap(SBox,1,comb);

    cout<<"Scintillation Plane 3D is created!"<<endl;
}

ScintPlane3D::~ScintPlane3D()
{

}

//Using new hit method same as planar view

void ScintPlane3D::LHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "left hit index out of bounds in scintplane3D: "<< num << endl;
    else
        paddle[num]->HitL();
}

void ScintPlane3D::RHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "right hit index out of bounds in scintplane3D: "<< num << endl;
    else
        paddle[num]->HitR();
}

void ScintPlane3D::BHit(int num)
{
    if( num<0 || num >= numPaddles)
        cerr<< "paddle hit index out of bounds in scintplane3D: "<< num << endl;
    else {
        paddle[num]->HitL();
        paddle[num]->HitR();
        paddle[num]->HitPaddle();
    }
}

void ScintPlane3D::clear()
{
    for(int i = 0; i<numPaddles; i++) {
        paddle[i]->clear();
    }
}

void ScintPlane3D::SPHit(int NumL, int NumR, double poshit[], double neghit[])
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
