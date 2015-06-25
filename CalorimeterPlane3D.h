#ifndef ROOT_CalorimeterPlane3D
#define ROOT_CalorimeterPlane3D

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "ScintillatorPaddle3D.h"
#include "GetVariables.h"
#include "CalorimeterBlock3D.h"
#include "TLatex.h"
#include "TGeoMatrix.h"
#include <string>
#include <cstring>
#include "TGeoManager.h"
#include <vector>


class CalorimeterPlane3D
{

public:
    
    CalorimeterPlane3D(char* splaneName, GetVariables* DB, TGeoVolume* top, int shift);
    virtual ~CalorimeterPlane3D();
    void LHit(int numL);
    void RHit(int numR);
    void BHit(int numB);
    void SPHit(int NumL,int NumR, double poshit[], double neghit[]);
    void clear();
    TGeoVolume *ScintPlane;
protected:
    int numPaddles; // number of paddles
    std::vector<CalorimeterBlock3D*> paddle;
};

#endif
