#ifndef ROOT_CalorimeterBlock3D
#define ROOT_CalorimeterBlock3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTrd2.h"
#include "TGeoTube.h"
#include "TGeoPhysicalNode.h"
#include "TGeoCompositeShape.h"


class CalorimeterBlock3D
{

public:
  
CalorimeterBlock3D(char* PlaneName,int index,int n,
                         double length, double height ,double thickness ,
                         TGeoVolume *paddle, int numPMT, int PMTdirection );
    virtual ~CalorimeterBlock3D();
    void HitL();
    void HitR();
    void HitPaddle();
    void clear();
   bool odd;
   bool even;


protected:

    TGeoVolume *scint;
    TGeoVolume *pmt1;
    TGeoVolume *pmt2;
};

#endif

