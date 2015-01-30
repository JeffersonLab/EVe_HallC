#ifndef ROOT_WireChamber3D
#define ROOT_WireChamber3D

#include "TCanvas.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TWire3D.h"
#include "WirePlane3D.h"

#include <map>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class WireChamber3D
{
public:
    WireChamber3D(char* Name, vector<string> PlaneNames, TGeoVolume* top);
    virtual ~WireChamber3D();
    void WireHit3D(string PlaneName,int  WireNum);
    void clear();

protected:
    TGeoVolume* Chamber3D;

    std::map <string, WirePlane3D> WirePlanes;

};

#endif // ROOT_MWDChamber3D
