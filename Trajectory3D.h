//************************************************************************* 
//  Trajectory3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create, show and modify partial tracks in 3D view.
//
// 
//
//*************************************************************************  

#ifndef ROOT_Trajectory3D
#define ROOT_Trajectory3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TGeoArb8.h"
#include "TGeoPhysicalNode.h"

class Trajectory3D {
  
public:
  Trajectory3D(char *ime,TGeoVolume *tvolume, TGeoManager *mgr,  char *pot);
  void Track(double xf, double yf, double zf, double thetaf, double phif);
  virtual ~Trajectory3D();
  void ClearTrack();
  
protected:
	char name[100];
	char path[100];
	TGeoManager *manager;		

};

#endif
