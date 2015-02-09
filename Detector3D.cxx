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
#include "EVe_DB.h"
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

    string PN[6]={"x", "y", "u", "v", "yp", "xp"};
    vector<string> PlaneNames(&PN[0],&PN[0]+6);
    // First MWDC
    MWDC1 = new WireChamber3D((char*) "MWDC1",  PlaneNames, top);

    // Second MWDC
    MWDC2 = new WireChamber3D((char*) "MWDC2",  PlaneNames, top);


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

      /*
       // In the end we create potential tracks
       // Important: Track order is important :Full tracks must be created 
       // before partial tracks.
	
       // First we create full tracks
       fullTrack[0] = new FullTrajectory3D((char*)"FT1", top, mgr, (char*)"/TOP_1/");
       fullTrack[1] = new FullTrajectory3D((char*)"FT2", top, mgr, (char*)"/TOP_1/");
       fullTrack[2] = new FullTrajectory3D((char*)"FT3", top, mgr, (char*)"/TOP_1/");
       fullTrack[3] = new FullTrajectory3D((char*)"FT4", top, mgr, (char*)"/TOP_1/");
       fullTrack[4] = new FullTrajectory3D((char*)"FT5", top, mgr, (char*)"/TOP_1/");
       fullTrack[5] = new FullTrajectory3D((char*)"FT6", top, mgr, (char*)"/TOP_1/");
       fullTrack[6] = new FullTrajectory3D((char*)"FT7", top, mgr, (char*)"/TOP_1/");
       fullTrack[7] = new FullTrajectory3D((char*)"FT8", top, mgr, (char*)"/TOP_1/");
       fullTrack[8] = new FullTrajectory3D((char*)"FT9", top, mgr, (char*)"/TOP_1/");
       fullTrack[9] = new FullTrajectory3D((char*)"FT10", top, mgr, (char*)"/TOP_1/");


       // Now we create partial tracks
       partialTrack[0] = new Trajectory3D((char*)"PT1", top, mgr, (char*)"/TOP_1/");
       partialTrack[1] = new Trajectory3D((char*)"PT2", top, mgr, (char*)"/TOP_1/");
       partialTrack[2] = new Trajectory3D((char*)"PT3", top, mgr, (char*)"/TOP_1/");	
       partialTrack[3] = new Trajectory3D((char*)"PT4", top, mgr, (char*)"/TOP_1/");	
       partialTrack[4] = new Trajectory3D((char*)"PT5", top, mgr, (char*)"/TOP_1/");	
       partialTrack[5] = new Trajectory3D((char*)"PT6", top, mgr, (char*)"/TOP_1/");	
       partialTrack[6] = new Trajectory3D((char*)"PT7", top, mgr, (char*)"/TOP_1/");	
       partialTrack[7] = new Trajectory3D((char*)"PT8", top, mgr, (char*)"/TOP_1/");	
       partialTrack[8] = new Trajectory3D((char*)"PT9", top, mgr, (char*)"/TOP_1/");	
       partialTrack[9] = new Trajectory3D((char*)"PT10", top, mgr, (char*)"/TOP_1/");		
      */

       mgr->CloseGeometry();
       top->Raytrace();


       cout<<"Detector Created!"<<endl;
}


Detector3D::~Detector3D()
{

}


void Detector3D::ClearTracks()
{	
  //std::vector<Trajectory3D* >:: iterator itr;

  //  for (itr=Trajectory.begin(); itr!= Trajectory.end(); itr++)
  //  Trajectory.clear();
  Trajectory.erase(Trajectory.begin(),Trajectory.end());
  //for (int i=0; i<Trajectory.size();i++)
  //  delete[] Trajectory[i];
}
