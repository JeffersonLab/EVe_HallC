//************************************************************************* 
//  MWDChamber3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create 3D simulation of MWDC and to show hit wires
//
//
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
#include "TPremica3D.h"

class MWDChamber3D {
  
public:
  MWDChamber3D(char *ime, int t, int n, double x, double y, double z, double width, double height, TGeoVolume *paddle);
  virtual ~MWDChamber3D();
  void u1WireHit(int i);
  void u2WireHit(int i);
  void v1WireHit(int i);
  void v2WireHit(int i);
  void x1WireHit(int i);
  void x2WireHit(int i);
  void clear();

protected:
  int Num; // stevilo linij !!! not true
  TGeoTube *cev1;
  int type;

  TPremica3D *x1_wires[300];
  TPremica3D *x2_wires[300];

  TPremica3D *v1_wires[300];
  TPremica3D *v2_wires[300];

  TPremica3D *u1_wires[300];
  TPremica3D *u2_wires[300];

};

#endif
