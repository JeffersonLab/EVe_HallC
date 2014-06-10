//************************************************************************* 
//  ScintilationPlane3D.h  - 4/14/2008
// 
//  by miham
// 
//  This class is creates 3D view of one scintillation plane, consisting 
//  n scintillation paddles.
//
//
//************************************************************************* 
#ifndef ROOT_ScintilationPlane3D
#define ROOT_ScintilationPlane3D

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "Scintilator3D.h"
#include "TLatex.h"

class ScintilationPlane3D {
  
public:
  ScintilationPlane3D(char *ime, int n, double x, double y, double z, double length, double height, double thickness, TGeoVolume *paddles);
  virtual ~ScintilationPlane3D();
  void paddleHit(int num, double left, double right);
  void clear();

protected:
  int N; // number of paddles

  Scintilator3D *paddle[30];
};

#endif
