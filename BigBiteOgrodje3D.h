//************************************************************************* 
//  BigBiteOgrodje3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class creates the 3D view of Big Bite Magnet
// 
//
//************************************************************************* 

#ifndef ROOT_BigBiteOgrodje3D
#define ROOT_BigBiteOgrodje3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TGeoArb8.h"

class BigBiteOgrodje3D {
  
public:
  BigBiteOgrodje3D( double x, double y, double z, TGeoVolume *paddle);
  virtual ~BigBiteOgrodje3D();
  
protected:

};

#endif
