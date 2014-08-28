//************************************************************************* 
//  MWDChamber3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create 3D simulation of MWDC and to show hit wires
//
//  Updated by Ben Davis-Purcell - 8/26/2014
//************************************************************************* 
 
#ifndef ROOT_MWDChamber3D
#define ROOT_MWDChamber3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TWire3D.h"

class MWDChamber3D {
  
public:
  MWDChamber3D(char *name, int t, int n, double x, double y, double z, double width, double height, TGeoVolume *paddle);
  virtual ~MWDChamber3D();
  void u1WireHit(int i);
  void u2WireHit(int i);
  void v1WireHit(int i);
  void v2WireHit(int i);
  void x1WireHit(int i);
  void x2WireHit(int i);
  void clear();

protected:
  int Num; 
  TGeoTube *tube1;
  int type;

  TWire3D *x1_wires[300];
  TWire3D *x2_wires[300];

  TWire3D *v1_wires[300];
  TWire3D *v2_wires[300];

  TWire3D *u1_wires[300];
  TWire3D *u2_wires[300];

};

#endif
