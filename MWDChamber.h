

//************************************************************************* 
//  MWDChamber.h  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view of the MWDC.
//
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
  //MWDChamber(char *ime, int n, double x, double y, double a, double b, int t);
  MWDChamber(char *ime, int n, double Hight, CStransform *cst, int t);
  virtual ~MWDChamber();
  void u1WireHit(int i);
  void u2WireHit(int i);
  void v1WireHit(int i);
  void v2WireHit(int i);
  void x1WireHit(int i);
  void x2WireHit(int i);
  void clear();
  void Track(double x, double y, int i);

protected:
  int Num; // stevilo linij !!! not true
  TPolyLine *okvir;
  TPolyLine *okvir2;
  TLine *u1_wires[1000];
  TLine *u2_wires[1000];
  TLine *v1_wires[1000];
  TLine *v2_wires[1000];
  TLine *x1_wires[1000];
  TLine *x2_wires[1000];
  TLatex *title;
  CStransform *cst;
  double fa,fb;
 
  int type;


  TEllipse *u1_circ;
  TEllipse *u2_circ;
  TEllipse *v1_circ;
  TEllipse *v2_circ;
  TEllipse *x1_circ;
  TEllipse *x2_circ;

  TEllipse *track_circ[MAX_TRACK_NUM];

};

#endif
