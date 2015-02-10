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
#include "TWire3D.h"
#include "WirePlane3D.h"
#include "WireChamber3D.h"
//#include "TGeoPhysicalNode.h"
#include "Trajectory3D.h"
#include "GetVariables.h"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class Detector3D {
  
public:
  Detector3D();
  virtual ~Detector3D();
  
  void Track(double p0x, double p0y, double p0z, 
            double xf, double yf, double zf, double thetaf, double phif);
  void ClearTrack();
  void ClearTracks();
  TGeoManager *mgr;
  WireChamber3D *MWDC1;
  WireChamber3D *MWDC2;
  ScintPlane3D *s1xplane;
  ScintPlane3D *s1yplane;
  ScintPlane3D *s2xplane;
  ScintPlane3D *s2yplane;

  vector<Trajectory3D* > TrackList;

  TGeoVolume *top;

};

#endif
