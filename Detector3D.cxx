///////////////////////////////////////
/*  Detector3D.h  7/17/14

    Ben Davis-Purcell

    Class that joins all detectors in 3D view
    and handles 3D full and partial tracks

*/
///////////////////////////////////////

#include "Detector3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include "TGeoSphere.h"
#include "TWire3D.h"
#include "GetVariables.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGLViewer.h"
#include "TGLPerspectiveCamera.h"

#include <map>
#include <string>
#include <vector>

#define DEG2RAD 3.1415926/180.

using namespace std;

Detector3D::Detector3D()
{
    mgr = new TGeoManager("Geom", "Detector Stack");
    TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
    TGeoMedium *medium =  new TGeoMedium("Vacuum",1, matVacuum);
    top = mgr->MakeBox("TOP",medium,600,300,300);
    mgr->SetTopVolume(top);
    mgr->SetVerboseLevel(0);

    //Test code for checking global visibility settings of 3D view
    //1. Below there are 6 boxes, one contain another sequencially
    //   SetVisLevel(int n) determines the deepest level of TGeoVolunme can be shown
    //2. Notice: top volume is level 0, Box1 volume is level 1, and so on.
    //3. https://root.cern.ch/root/html/TGeoManager.html states that SetVisOption(0)
    //   will show intermediate volumes, but I didn't observe that.
    //4. It seems the default level is 5: Can compare SetVisLevel(6), SetVisLevel(5)
    //   and commented out SetVisLevel in 3D view.
    /*
          mgr->SetVisOption(1);
          mgr->SetVisLevel(6);

          TGeoBBox *box1 = new TGeoBBox ("Box1", 128,128,128);
          TGeoTranslation bt(-200,0,0);
          TGeoRotation br;
          br.SetAngles(90,0,90,90,0,0);
          TGeoCombiTrans *btr = new TGeoCombiTrans (bt,br);
          TGeoVolume *Box1 = new TGeoVolume ("Box1",box1);
          Box1 -> SetLineColor(kBlack);
          top -> AddNode(Box1,1,btr);

          TGeoBBox *box2 = new TGeoBBox ("Box2", 64,64,64);
          TGeoVolume *Box2 = new TGeoVolume ("Box2",box2);
          Box2 -> SetLineColor(kBlack);
          Box1 -> AddNode(Box2,1);

          TGeoBBox *box3 = new TGeoBBox ("Box3", 32,32,32);
          TGeoVolume *Box3 = new TGeoVolume ("Box3",box3);
          Box3 -> SetLineColor(kBlack);
          Box2 -> AddNode(Box3,1);

          TGeoBBox *box4 = new TGeoBBox ("Box4", 16,16,16);
          TGeoVolume *Box4 = new TGeoVolume ("Box4",box4);
          Box4 -> SetLineColor(kBlack);
          Box3 -> AddNode(Box4,1);

          TGeoBBox *box5 = new TGeoBBox ("Box5", 8,8,8);
          TGeoVolume *Box5 = new TGeoVolume ("Box5",box5);
          Box5 -> SetLineColor(kBlack);
          Box4 -> AddNode(Box5,1);

          TGeoBBox *box6 = new TGeoBBox ("Box6", 4,4,4);
          TGeoVolume *Box6 = new TGeoVolume ("Box6",box6);
          Box6 -> SetLineColor(kBlack);
          Box5 -> AddNode(Box6,1);
    */

    string PN[6]= {"x", "y", "u", "v", "yp", "xp"};
    vector<string> PlaneNames(&PN[0],&PN[0]+6);
    // First MWDC
    GetVariables *hmsDB = new GetVariables("SHMS.txt");
    MWDC1 = new WireChamber3D((char*) "MWDC1",  PlaneNames, hmsDB, top, mgr);

    // Second MWDC
    MWDC2 = new WireChamber3D((char*) "MWDC2",  PlaneNames, hmsDB, top, mgr);

    // s1x - Scintillation Plane
    s1xplane = new ScintPlane3D((char*)"s1x", hmsDB, top);

    // s1y - Scintillation Plane
    s1yplane = new ScintPlane3D((char*)"s1y", hmsDB, top);

    //s2x Scint Plane
    s2xplane = new ScintPlane3D((char*)"s2x", hmsDB, top);

    //s2y Scint Plane
    // changed volume size to 60
    s2yplane = new ScintPlane3D((char*)"s2y", hmsDB, top);

    pr[0] = new CalorimeterPlane3D((char*)"pr", hmsDB, top, 0);
    pr[1] = new CalorimeterPlane3D((char*)"pr", hmsDB, top, 1);

    // There will be 10 tracks for any fixed number of tracks in this code,
    // The reason for it is after you call CloseGeometry() function, you
    // cannot dynamically change number of TGeoVolume in root.
    for(uint i=0; i<10; i++) {
        TrackList.push_back(new Trajectory3D(top,mgr,i));
    }

    mgr->CloseGeometry();
    //top->Raytrace();
    top->Draw("ogl");
    TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
    if( v != NULL ) {
        static TGLViewer::ECameraType CamType=TGLViewer::kCameraPerspXOY;
        static Double_t fov = 30;
        static Double_t dolly = 300.0;            // FIXME: should be computed somehow
        static Double_t center[3] = {150, 0, 0};  // FIXME: should be fetched from bounding box
        static Double_t vRotate = -40.0*DEG2RAD;  // ROOT docs say degrees, it's Radians...
        static Double_t hRotate = -40.0*DEG2RAD;  // ROOT docs say degrees, it's Radians...
        v->SetResetCamerasOnUpdate(kFALSE);
        v->SetPerspectiveCamera (CamType,fov,dolly,center,hRotate,vRotate);
        v->SetCurrentCamera(CamType);
    }
    cout<<"Detector Created!"<<endl;
}


Detector3D::~Detector3D()
{

}


void Detector3D::ClearTracks()
{
    for (uint i=0; i<TrackList.size(); i++) {
        TrackList[i]->Disable();
    }
}
