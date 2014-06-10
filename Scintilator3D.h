//************************************************************************* 
//  Scintilator3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is creates 3D view of one scintilation paddle with 
//  corresponding PMTs
//
//
//************************************************************************* 

#ifndef ROOT_Scintilator3D
#define ROOT_Scintilator3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"

class Scintilator3D {
  
public:
  Scintilator3D(int index, double x, double y, double z, double length, double height ,double thickness ,TGeoVolume *paddle);
  virtual ~Scintilator3D();
  void hit(double left, double right);
  void clear();

protected:

  TGeoVolume *scint;
  TGeoVolume *pmt1;
  TGeoVolume *pmt2;  

};

#endif
