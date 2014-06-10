//************************************************************************* 
//  ScintillationPlane.h  - Monday, June 9/14
// 
//  by Ben Davis-Purcell
// 
//  This class is used to create planar view of whole scintilation plane
//  that consists of n scintilation paddles
//
//
//*************************************************************************   
#ifndef ROOT_ScintillationPlane
#define ROOT_ScintillationPlane

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "Scintillator.h"
#include "TLatex.h"
#include "CStransform.h"
#include "TEllipse.h"
#include "EVe_DB.h"

class ScintillationPlane {
  
public:
  //ScintillationPlane(char *ime, int n, double x, double y, double a, double b, double length);
  ScintillationPlane(char *ime, int n,  double plength, double pheight, CStransform *trans);
  virtual ~ScintillationPlane();
  void paddleHit(int num, double left, double right, double y);
  void Track(double x, double y, int i);
  void clear();

protected:
  int N; // number of paddles
  double sx0, sy0,sa,sb;
  double paddle_length;
  double paddle_height;
  Scintillator *paddle[MAX_PADDLE_NUM];
  TLatex *title;
  CStransform *cst;
  TEllipse *track_circ[MAX_TRACK_NUM];
  TLatex *track_index[MAX_TRACK_NUM];
};

#endif
