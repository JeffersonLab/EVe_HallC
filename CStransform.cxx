//************************************************************************* 
//  CSTransform.cxx  - 4/14/2008
// 
//  by miham
// 
//  We use this class to transform real coordinate values from MWDC and 
//  Scintillators (that we get from data) to pixels inside a canvas, where we
//  want to draw out data. 
// 
//
//*************************************************************************

#include "CStransform.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>


using namespace std;


CStransform::~CStransform()
{

} 

CStransform::CStransform (double A, double cX0, double cY0)
{
  // A is length in meters
  alpha = 2.0/A;
  CX0 = cX0; 
  CY0 = cY0; 
}

/// FIXME:: Attempt to fix Tracking for rotated ScintPlanes.
/// This does not work, just reflects ScintPlane and ScintPlane 
/// labelling on xy plane
CStransform::CStransform (double A, double cX0, double cY0, int rotation)
{
  alpha = 1.0/A;  // A is length in meters
  if (rotation == 1) {
    CX0 = cX0; 
    CY0 = cY0;
  }
  else {
    CX0 = cY0; 
    CY0 = cX0; 
  }
}

double CStransform::transXtoCX(double x)
{
  return alpha*x+CX0;
} 

double CStransform::transYtoCY(double y)
{
  return alpha*y+CY0;
}

double CStransform::transLtoCL(double l)
{
  return alpha*l;
}

double CStransform::GetAlpha()
{
  return alpha;
}
