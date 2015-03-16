#ifndef ROOT_Trajectory3D
#define ROOT_Trajectory3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include "TGeoTube.h"
#include "TGeoManager.h"


class Trajectory3D{
public:
  Trajectory3D(TGeoVolume* Top, TGeoManager* Mgr, int n);

  Trajectory3D(TGeoVolume* Top, TGeoManager* Mgr, int n,
               double x, double y, double theta, double phi);

  virtual ~Trajectory3D();
  void Enable(int n, double x, double y, double theta, double phi);
  void Disable();

protected:
  char* path;
  TGeoVolume* Ray;
  TGeoManager* Manager;
};

#endif

