#ifndef ROOT_WirePlane3D
#define ROOT_WirePlane3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoTube.h"
#include "TGeoEltu.h"
#include "TWire3D.h"
#include "GetVariables.h"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "TGeoPhysicalNode.h"

#define WIRE3DRADIUS 0.2
#define SPARSIFY     5            /* Scale drawn wires down by SPARSIFY */

using namespace std;

class WirePlane3D{
public:
  WirePlane3D(char* ChamberName,string PlaneName,TGeoVolume* WireChamber3D,TGeoVolume* Top,TGeoManager* Mgr,int color);
    virtual ~WirePlane3D();
    void Wire3DHit(int Num);
    void clear();
    vector<TWire3D*> Wires;

protected:
    int WireNum;
    int wirecolor;
    TGeoVolume* WirePlane;
    char* path;
    TGeoManager* mgr;
    TGeoVolume* HitTube;
    TGeoPhysicalNode* PN;
};

#endif
