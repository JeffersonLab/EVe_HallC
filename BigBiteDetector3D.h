//************************************************************************* 
//  BigBiteDetector3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class joins together magnet and all detectors (in 3D view) and creates 
//  full 3D view of the Big Bite spectrometer. It also handles the
//  creation and manipulation with 3D ( full and partial ) tracks.
// 
//
//************************************************************************* 

#ifndef ROOT_BigBiteDetector3D
#define ROOT_BigBiteDetector3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
///#include "BigBiteOgrodje3D.h"
#include "ScintilationPlane3D.h"
#include "MWDChamber3D.h"
#include  "TGeoPhysicalNode.h"
#include "Trajectory3D.h"
#include "FullTrajectory3D.h"

class BigBiteDetector3D {
  
public:
  BigBiteDetector3D();
  virtual ~BigBiteDetector3D();
  
  void Track(double p0x, double p0y, double p0z, double xf, double yf, double zf, double thetaf, double phif);
  void ClearTrack();
  void ClearTracks();
  TGeoManager *mgr;
  MWDChamber3D *mwdc1;
  MWDChamber3D *mwdc2;
  ScintilationPlane3D *scintE;
  ScintilationPlane3D *scintdE;
  Trajectory3D *partialTrack[10];
  FullTrajectory3D *fullTrack[10]; 
  

protected:

  TGeoVolume *top;

};

#endif
