///////////////////////////////////////
/*  ScintPlane3D.h  7/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddle3Ds to create a 3D plane

*/
///////////////////////////////////////

#ifndef ROOT_ScintPlane3D
#define ROOT_ScintPlane3D

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "ScintillatorPaddle3D.h"
#include "GetVariables.h"
#include "TLatex.h"

class ScintPlane3D {

public:
  ScintPlane3D(char *name, int n, double x, double y, double z, double length, double height, double thickness, TGeoVolume *paddles);
  virtual ~ScintPlane3D();
  void paddleHit(int num, double left, double right);
  void clear();

protected:
  int N; // number of paddles

  ScintillatorPaddle3D *paddle[30];

};

#endif
