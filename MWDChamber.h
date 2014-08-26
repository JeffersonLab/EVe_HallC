//************************************************************************* 
//  MWDChamber.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view of the MWDC.
//  Updated by Ben Davis-Purcell - 8/26/2014
//
//*************************************************************************   
#ifndef ROOT_MWDChamber
#define ROOT_MWDChamber

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "CStransform.h"
#include "EVe_DB.h"

class MWDChamber {
  
public:
  MWDChamber(char *name, int n, double Height, double width, CStransform *cst, int t);
  virtual ~MWDChamber();
  void uWireHit(int i);
  void vWireHit(int i);
  void y1WireHit(int i);
  void ypWireHit(int i);
  void x1WireHit(int i);
  void xpWireHit(int i);
  void clear();
  void Track(double x, double y, int i);

protected:
  int Num; 
  TPolyLine *box;
  TPolyLine *box2;
  TLine *u_wires[1000];
  TLine *v_wires[1000];
  TLine *y1_wires[1000];
  TLine *yp_wires[1000];
  TLine *x1_wires[1000];
  TLine *xp_wires[1000];
  TLatex *title;
  CStransform *cst;
  double fa,fb;
 
  int type;


  TEllipse *u_circ;
  TEllipse *v_circ;
  TEllipse *y1_circ;
  TEllipse *yp_circ;
  TEllipse *x1_circ;
  TEllipse *xp_circ;

  TEllipse *track_circ[MAX_TRACK_NUM];

};

#endif
