//************************************************************************* 
//  WirePlane.h  - 4/14/2008
// 
//  by miham
// 
//  This class creates the projection view for given wire plane in given 
//  wire chamber. 
//
// 
//
//************************************************************************* 

#ifndef ROOT_WirePlane
#define ROOT_WirePlane

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "EVe_DB.h"


class WirePlane {
  
public:
  WirePlane(char *name, int n, double x, double y, double l, double a, double b, int f=0);
  virtual ~WirePlane();
  void SetWire(int i, double t);
  void clear();
  void Hit(Double_t *signal);


protected:
  int WireNum;
  double x0, y0,a0,b0;
  double rescale_me;
  TEllipse *wire[400];
  TLine *time[400];
  int type;
  double length;
  char *planename;
 

};

#endif
