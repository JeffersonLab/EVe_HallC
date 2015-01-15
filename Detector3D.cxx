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



using namespace std;

Detector3D::Detector3D()
{
   
      mgr = new TGeoManager("Geom", "Composite shape example");
      TGeoMedium *medium = 0;
      top = mgr->MakeBox("TOP",medium,450,300,300);
      mgr->SetTopVolume(top); 


      TGeoRotation r1;
      //TGeoRotation r2;
      TGeoTranslation t1(100.0, 0.0, 60.0);
      TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1);
      
      GetVariables* HMSvars=new GetVariables("HMS.txt"); 

      // First MWDC      
  // FIXME: Wire number is different in different wire planes. For now we assume
  // in 3D view, that the number of wires is in all three w.p. the same. 
      TGeoVolume *mwdc_volume = mgr->MakeBox("mwdc_volume1",medium,50,5,100);
      mwdc1 = new MWDChamber3D((char*)"MWDC1",0, HMSvars->GetDouble("MWDC-1.x.numWires ="), 0.0, 0.0, 0.0, MWDC1_length, MWDC1_height, mwdc_volume);
      r1.SetAngles(90,90,90+MWDC1_tilt,180,MWDC1_tilt,180);
      t1.SetTranslation(MWDC1_xpos-50.0, MWDC1_ypos, MWDC1_zpos);
      comb = new TGeoCombiTrans(t1, r1);
      top->AddNodeOverlap(mwdc_volume,1, comb);

      // Second MWDC
  // FIXME: Wire number is different in different wire planes. For now we assume
  // in 3D view, that the number of wires is in all three w.p. the same. 
      TGeoVolume *mwdc_volume2 = mgr->MakeBox("mwdc_volume2",medium,50,5,105);
      mwdc2 = new MWDChamber3D((char*)"MWDC2",0,HMSvars->GetDouble("MWDC-2.x.numWires ="), 0.0, 0.0, 0.0, MWDC2_length, MWDC2_height, mwdc_volume2);
      r1.SetAngles(90,90,90+MWDC2_tilt,180,MWDC2_tilt,180);
      t1.SetTranslation(MWDC2_xpos-30.0, MWDC2_ypos, MWDC2_zpos);
      comb = new TGeoCombiTrans(t1, r1);
      top->AddNodeOverlap(mwdc_volume2,1, comb);

      

      // s1x - Scintillation Plane     
      s1xplane = new ScintPlane3D((char*)"s1x",top);

      // s1y - Scintillation Plane  
      s1yplane = new ScintPlane3D((char*)"s1y",top);
      
      //s2x Scint Plane
      s2xplane = new ScintPlane3D((char*)"s2x",top);

      //s2y Scint Plane
      // changed volume size to 60
      s2yplane = new ScintPlane3D((char*)"s2y",top);

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

       mgr->CloseGeometry();
       top->Raytrace();


       cout<<"Detector Created!"<<endl;
}


Detector3D::~Detector3D()
{

}


void Detector3D::ClearTracks()
{	
	
       for (int i = 0; i<10; i++)
       {
	  partialTrack[i]->ClearTrack();
	  fullTrack[i]->ClearTrack();	
       }	
	
	

}
