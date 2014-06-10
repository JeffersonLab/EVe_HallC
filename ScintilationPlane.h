//************************************************************************* 
//  ScintilationPlane.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view of whole scintilation plane
//  that consists of n scintilation paddles
//
//
//*************************************************************************   
#ifndef ROOT_ScintilationPlane
#define ROOT_ScintilationPlane

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "Scintilator.h"
#include "TLatex.h"
#include "CStransform.h"
#include "TEllipse.h"
#include "EVe_DB.h"

class ScintilationPlane {
  
public:
  //ScintilationPlane(char *ime, int n, double x, double y, double a, double b, double length);
  ScintilationPlane(char *ime, int n,  double plength, double pheight, CStransform *trans);
  virtual ~ScintilationPlane();
  void paddleHit(int num, double left, double right, double y);
  void Track(double x, double y, int i);
  void clear();

protected:
  int N; // number of paddles
  double sx0, sy0,sa,sb;
  double paddle_length;
  double paddle_height;
  Scintilator *paddle[MAX_PADDLE_NUM];
  TLatex *title;
  CStransform *cst;
  TEllipse *track_circ[MAX_TRACK_NUM];
  TLatex *track_index[MAX_TRACK_NUM];
};

#endif
