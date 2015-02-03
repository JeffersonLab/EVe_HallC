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
#include "TGeoMatrix.h"
#include <string>
#include <cstring>
#include "TGeoManager.h"

class ScintPlane3D {

public:
  ScintPlane3D(char* splaneName, TGeoVolume* top);
  virtual ~ScintPlane3D();
  //void paddleHit(int num, double left, double right);
  void LHit(int numL);
  void RHit(int numR);
  void BHit(int numB);
  void SPHit(int Num, double poshit[], double neghit[]);
  void clear();
  TGeoVolume *ScintPlane;
  //TGeoRotation* scintrot;
protected:
  int N; // number of paddles

  ScintillatorPaddle3D *paddle[30];

};

#endif
