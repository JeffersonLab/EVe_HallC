//************************************************************************* 
//  MultiRoads.h  - 4/14/2008
// 
//  by miham
// 
//  This class handles Multi roads in projection view 
//
// 
//
//************************************************************************* 
#ifndef ROOT_MultiRoads
#define ROOT_MultiRoads

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "Road.h"
#include "CStransform.h"


class MultiRoads{
  
public:
  MultiRoads(CStransform *cst);
  virtual ~MultiRoads();
  void Show(int i, double x[], double z[]);
  void Clear();
protected:

  Road *roads[100];
  CStransform *transformation;
  
};

#endif
