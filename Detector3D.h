///////////////////////////////////////
/*  Detector3D.h  7/17/14
    
    Ben Davis-Purcell

    Class that joins all detectors in 3D view
    and handles 3D full and partial tracks

*/
///////////////////////////////////////

#ifndef ROOT_Detector3D
#define ROOT_Detector3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "ScintPlane3D.h"
#include "MWDChamber3D.h"
#include "TGeoPhysicalNode.h"
#include "Trajectory3D.h"
#include "FullTrajectory3D.h"
#include "GetVariables.h"

class Detector3D {
  
public:
  Detector3D();
  virtual ~Detector3D();
  
  void Track(double p0x, double p0y, double p0z, double xf, double yf, double zf, double thetaf, double phif);
  void ClearTrack();
  void ClearTracks();
  TGeoManager *mgr;
  MWDChamber3D *mwdc1;
  MWDChamber3D *mwdc2;
  ScintPlane3D *scintE;
  ScintPlane3D *scintdE;
  Trajectory3D *partialTrack[10];
  FullTrajectory3D *fullTrack[10]; 
  

protected:

  TGeoVolume *top;

};

#endif
