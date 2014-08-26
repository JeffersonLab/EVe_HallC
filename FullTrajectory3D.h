//************************************************************************* 
//  FullTrajectory3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create, show and modify Full tracks in 3D view.
//  Full tracks are those tracks that have been fully reconstructed 
//  (we know their momenta in the target etc.)
//
// 
//
//*************************************************************************

#ifndef ROOT_FullTrajectory3D
#define ROOT_FullTrajectory3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TGeoArb8.h"
#include "TGeoPhysicalNode.h"

class FullTrajectory3D {
  
public:
  FullTrajectory3D(char *name, TGeoVolume *tvolume, TGeoManager *mgr,  char *pot);
  void Track(double p0x, double p0y, double p0z, double xf, double yf, double zf, double thetaf, double phif);
  virtual ~FullTrajectory3D();
  void ClearTrack();
  
protected:
	char namea[100];
	char nameb[100];
	char namec[100];
	char patha[100];
	char pathb[100];
	char pathc[100];
	TGeoManager *manager;		
};

#endif
