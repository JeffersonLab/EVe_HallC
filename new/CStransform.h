//************************************************************************* 
//  CSTransform.h  - 4/14/2008
// 
//  by miham
// 
//  We use this class to transform real coordinate values from MWDC and 
//  Scint. (that we get from data) to pixels inside a canvas, where we
//  want to draw out data. 
// 
//
//*************************************************************************
#ifndef ROOT_CStransform
#define ROOT_CStransform

class CStransform {
  
public:
  CStransform (double A, double cX0, double cY0);
  virtual ~CStransform();
  double transXtoCX(double x);
  double transYtoCY(double y);
  double transLtoCL(double l);
  double GetAlpha();
protected:
  double alpha;
  double CX0, CY0;
};

#endif
