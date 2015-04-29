#include "WireChamber3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include "GetVariables.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

WireChamber3D::WireChamber3D(char* ChamberName, vector<string> PlaneNames, GetVariables* DB,
                                TGeoVolume* top, TGeoManager* mgr)
{
    double H= DB->GetDouble(Form("%s.Height =",ChamberName));
    double W= DB->GetDouble(Form("%s.Width =",ChamberName));

    double CT= 100.0*DB->GetDouble(Form("%s.Thickness =",ChamberName));
    double WT= 100.0*DB->GetDouble(Form("%s.WallThickness =",ChamberName));

    TGeoBBox *ChamberBox = new TGeoBBox(Form("%s.ChamberBox",ChamberName),5.0*CT/2.0,1.5*W/2.0,1.5*H/2.0);
    Chamber3D = new TGeoVolume(Form("%s.Chamber",ChamberName),ChamberBox);
    //Drawing Frame of the WireChamber

    TGeoBBox *LRWall = new TGeoBBox("LRWall",CT/2.0,WT/2.0,H/2.0);
    TGeoVolume *LeftWall = new TGeoVolume("LeftWall",LRWall);
    LeftWall->SetLineColor(kBlack);
    Chamber3D->AddNodeOverlap(LeftWall,1,new TGeoTranslation(0,(W+WT)/2.0,0));
    Chamber3D->AddNodeOverlap(LeftWall,2,new TGeoTranslation(0,-(W+WT)/2.0,0));

    TGeoBBox *ULWall = new TGeoBBox("ULWall",CT/2.0,W/2.0+WT,WT/2.0);
    TGeoVolume *UpperWall = new TGeoVolume("UpperWall",ULWall);
    UpperWall->SetLineColor(kBlack);
    Chamber3D->AddNodeOverlap(UpperWall,1, new TGeoTranslation(0,0,(H+WT)/2.0));
    Chamber3D->AddNodeOverlap(UpperWall,2, new TGeoTranslation(0,0,-(H+WT)/2.0));

    //Draw all WirePlanes for the WireChamber
    for(unsigned int i=0; i< PlaneNames.size(); i++) {
        WirePlanes.insert(std::pair<string, WirePlane3D>(PlaneNames[i],WirePlane3D(ChamberName,PlaneNames[i],Chamber3D, top,mgr, i+2)));
    }

    //Get Rotation and Translation, AddNode to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    TGeoCombiTrans *comb;

    double tilt = DB-> GetDouble(Form("%s.Tilt =",ChamberName));
    double x0 = 100.0*DB-> GetDouble(Form("%s.xPos =",ChamberName));
    double y0 = 100.0*DB-> GetDouble(Form("%s.yPos =",ChamberName));
    double z0 = 100.0*DB-> GetDouble(Form("%s.zPos =",ChamberName));

    //cerr << Form("%s.xpos is ",ChamberName) << x0 <<Form(" %s.ypos is ",ChamberName) << y0 <<Form(" %s.zpos is ",ChamberName) << z0 << endl;

    r1.SetAngles(90 - tilt,0,90,90,tilt,180);

    if(x0 > 0)
        t1.SetTranslation(x0, y0, z0);
    else
        t1.SetTranslation(-300.0-x0, y0, z0);
    comb = new TGeoCombiTrans(t1, r1);
    top->AddNodeOverlap(Chamber3D,1,comb);

    // Test all TGeoVolume visibility related setting function:

    // Volume hierachy is : top -> Chamber3D -> LeftWall, UpperWall
    //                                       -> WirePlanes -> Wires

    // General rule 1:
    // For Chamber 3D: since is not a final level, global setting will make it invisible, even SetVisibility is flag kTRUE
    // Rule 2:
    // All below functions have already set default values when TGeoVolume constructors have been called. For example, SetVisibility default setting is kTRUE.

    // InvisibleAll(kTRUE) will set Chamber3D and its direct daughters (LeftWall, UpperWall and WirePlanes in this case)
    // to invisible, but Wires will still be visible in 3D view
    //Chamber3D->InvisibleAll(kTRUE);

    // SetVisibility(kFALSE) will set Chamber3D invisible. But since global setting has already made it, it won't make any change.
    //Chamber3D->SetVisibility(kTRUE);

    // VisibleDaughters (kFALSE) will set Chamber3D's Daughters invisible, and since SetVIsbility is default kTRUE, Chamber3D itself will be shown in 3D view.
    //Chamber3D->VisibleDaughters(kFALSE);

    // Unkown function, will not make changes here but can make LeftWall invisible.
    //Chamber3D->SetAttVisibility(kFALSE);

    // Below 3 functions didn't make any change. However SetVisOnly may related to TGeoVolume::DrawOnly() method.
    //Chamber3D->SetVisContainers(kTRUE);
    //Chamber3D->SetVisOnly(kFALSE);
    //Chamber3D->SetVisLeaves(kFALSE);

    // Use those functions for LeftWall Volume, the difference is that LeftWall is 'leaves' (it contains no volume)
    // This will make LeftWall and its replicate (RightWall) invisible.
    //LeftWall->InvisibleAll(kTRUE);

    // This will make LeftWall and it replicate invisible.
    //LeftWall->SetVisibility(kFALSE);

    // This will not make any change either kTRUE or kFALSE
    //LeftWall->VisibleDaughters(kFALSE);

    // This can make LeftWall invisible.
    //LeftWall->SetAttVisibility(kFALSE);

    // Below functions will not change visibilities of LeftWall.
    //LeftWall->SetVisContainers(kFALSE);
    //LeftWall->SetVisOnly(kTRUE);
    //LeftWall-> SetVisLeaves(kFALSE);


    // Test code to test Setvisibility function: when used in constructor, it will change visibility.
    // When used in WireHit3D() and clear () below, after click 'Shwo this Event' button, the whole 3D view will be blank. Reason is unknown.
    //LeftWall->SetVisibility (kFALSE);
    //UpperWall->SetVisibility (kTRUE);
    cout<<"Chamber 3D is created!"<<endl;
}

WireChamber3D::~WireChamber3D() {}

void WireChamber3D::WireHit3D(string PlaneName, int WireNum)
{

    //TGeoTube *tube= (TGeoTube *) WirePlanes.find(PlaneName) ->second.Wires[(int) (WireNum/SPARSIFY)]->wire->GetShape();
    WirePlanes.find(PlaneName)->second.Wire3DHit(WireNum);

    //LeftWall->SetVisibility (kFALSE);
    //UpperWall->SetVisibility (kTRUE);
}

void WireChamber3D::clear()
{
    std::map<string, WirePlane3D>:: iterator itr;

    for (itr=WirePlanes.begin(); itr!= WirePlanes.end(); itr++)
        itr->second.clear();

    //LeftWall->SetVisibility (kTRUE);
    //UpperWall->SetVisibility (kTRUE);
}
