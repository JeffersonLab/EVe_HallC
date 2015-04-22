//************************************************************************* 
//  TWire3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create and handle one 3D wire. We use these wires in 
//  class MWDChamber3D.cxx
//
//
//************************************************************************* 

#ifndef ROOT_TWire3D
#define ROOT_TWire3D

#include "TGeoTube.h"
#include "TGeoEltu.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TColor.h"

class TWire3D {

public:
  TWire3D(double angle, double y1, double z1, double y2, double z2,double radius, TGeoVolume *WirePlane, int wirenum);
  virtual ~TWire3D();

  TGeoVolume *wire;
  TGeoEltu* Wiretube;
  TGeoCombiTrans* Wirecomb;

};

#endif
