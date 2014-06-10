//************************************************************************* 
//  TPremica3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create and handle one 3D wire. We use these wires in 
//  class MWDChamber3D.cxx
//
//
//************************************************************************* 
#ifndef ROOT_TPremica3D
#define ROOT_TPremica3D

#include "TGeoTube.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TColor.h"

class TPremica3D {
  
public:
  TPremica3D(double x1, double y1, double z1, double x2,  double y2, double z2,double radij, TGeoVolume *volume);
  virtual ~TPremica3D();
  void SetColor(Style_t barva);

protected:

 TGeoVolume *premica;
  


};

#endif
