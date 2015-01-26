///////////////////////////////////////
/*  ScintillatorPaddle3D.h  7/16/14
    
    Ben Davis-Purcell

    Class that creates a single 3D scintillation paddle with attached PMTs

*/
///////////////////////////////////////
#ifndef ROOT_ScintillatorPaddle3D
#define ROOT_ScintillatorPaddle3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TGeoPhysicalNode.h"
#include "TGeoCompositeShape.h"


class ScintillatorPaddle3D {

 public:
  ScintillatorPaddle3D(char* PlaneName,int index,int n,
                       double length, double height ,double thickness ,
                       TGeoVolume *paddle, int numPMT /*, int rotation*/);
  virtual ~ScintillatorPaddle3D();
  //void hit(double left, double right, int nPMT);
  void HitL();
  void HitR();
  void HitB();
  void clear();

 protected:

  TGeoVolume *scint;
  TGeoVolume *pmt1;
  TGeoVolume *pmt2;
  double plength;
  //  int n;

};

#endif



