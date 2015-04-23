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

#include <map>
#include <string>
#include <vector>

using namespace std;

Detector3D::Detector3D()
{
   
      mgr = new TGeoManager("Geom", "Composite shape example");
      TGeoMedium *medium = 0;     
      top = mgr->MakeBox("TOP",medium,600,300,300);
      mgr->SetTopVolume(top); 

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

    string PN[6]={"x", "y", "u", "v", "yp", "xp"};
    vector<string> PlaneNames(&PN[0],&PN[0]+6);
    // First MWDC
     MWDC1 = new WireChamber3D((char*) "MWDC1",  PlaneNames, top, mgr);
     

     //Test code to check visibility settings.
     //MWDC1-> LeftWall-> InvisibleAll(kFALSE);
     //MWDC1-> UpperWall->InvisibleAll(kTRUE);

    // Second MWDC
     MWDC2 = new WireChamber3D((char*) "MWDC2",  PlaneNames, top, mgr);


      //test code: check what transformation : translation and rotation did to a box in top volume
      
      TGeoBBox *box = new TGeoBBox("Box",5, 5, 5); 
      TGeoTranslation boxt(0,0,0);
      TGeoRotation boxr;
      double ang = 0;
      boxr.SetAngles(90,0,90-ang,90,ang,-90);
      TGeoCombiTrans *boxCT= new TGeoCombiTrans(boxt,boxr); 
      TGeoVolume *Box = new TGeoVolume ("Box", box);
      Box ->SetLineColor(kBlack);
      top -> AddNode(Box,1,boxCT);
      

      // s1x - Scintillation Plane     
      s1xplane = new ScintPlane3D((char*)"s1x",top);

      // s1y - Scintillation Plane  
      s1yplane = new ScintPlane3D((char*)"s1y",top);
      
      //s2x Scint Plane
      s2xplane = new ScintPlane3D((char*)"s2x",top);

      //s2y Scint Plane
      // changed volume size to 60
      s2yplane = new ScintPlane3D((char*)"s2y",top);    

      // There will be 10 tracks for any fixed number of tracks in this code,
      // The reason for it is after you call CloseGeometry() function, you cannot dynamically change number of TGeoVolume in root.
      for(int i=0; i<10; i++){
	TrackList.push_back(new Trajectory3D(top,mgr,i));}

       mgr->CloseGeometry();
       top->Raytrace();
       cout<<"Detector Created!"<<endl;
}


Detector3D::~Detector3D()
{

}


void Detector3D::ClearTracks()
{	
}
