#ifndef ROOT_WirePlane3D
#define ROOT_WirePlane3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoTube.h"
#include "TWire3D.h"
#include "GetVariables.h"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#define WIRE3DRADIUS 0.2

using namespace std;

class WirePlane3D{
public:
    WirePlane3D(char* ChamberName,string PlaneName,TGeoVolume* WireChamber3D, int color);
    virtual ~WirePlane3D();
    void Wire3DHit(int Num);
    void clear();

protected:
    int WireNum;
    int wirecolor;
    vector<TWire3D*> Wires;
    TGeoVolume* WirePlane;
};

#endif
