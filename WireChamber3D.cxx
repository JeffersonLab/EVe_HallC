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

WireChamber3D::WireChamber3D(char* ChamberName, vector<string> PlaneNames, TGeoVolume* top, TGeoManager* mgr)
{
   //Get Dimension data from HMS.txt
   // Height and Width of the wireplane

   GetVariables *hms = new GetVariables("HMS.txt");
   double H= 100.0*hms->GetDouble(Form("%s.Height =",ChamberName));
   double W= 100.0*hms->GetDouble(Form("%s.Width =",ChamberName));

   double CT= hms->GetDouble(Form("%s.Thickness =",ChamberName));
   double WT= hms->GetDouble(Form("%s.WallThickness =",ChamberName));

   TGeoBBox *ChamberBox = new TGeoBBox(Form("%s.ChamberBox",ChamberName),1.5*CT/2.0,1.5*W/2.0,1.5*H/2.0);
   Chamber3D = new TGeoVolume(Form("%s.Chamber",ChamberName),ChamberBox);
   //Drawing Frame of the WireChamber

   TGeoBBox *LRWall = new TGeoBBox("LRWall",CT/2.0,WT/2.0,H/2.0);
   TGeoVolume *LeftWall = new TGeoVolume("LeftWall",LRWall);
   LeftWall->SetLineColor(kBlack);
   Chamber3D->AddNode(LeftWall,1,new TGeoTranslation(0,(W+WT)/2.0,0));
   Chamber3D->AddNode(LeftWall,2,new TGeoTranslation(0,-(W+WT)/2.0,0));

   TGeoBBox *ULWall = new TGeoBBox("ULWall",CT/2.0,W/2.0+WT,WT/2.0);
   TGeoVolume *UpperWall = new TGeoVolume("UpperWall",ULWall);
   UpperWall->SetLineColor(kBlack);
   Chamber3D->AddNode(UpperWall,1, new TGeoTranslation(0,0,(H+WT)/2.0));
   Chamber3D->AddNode(UpperWall,2, new TGeoTranslation(0,0,-(H+WT)/2.0));

   //Draw all WirePlanes for the WireChamber
   for(unsigned int i=0; i< PlaneNames.size(); i++){
     WirePlanes.insert(std::pair<string, WirePlane3D>(PlaneNames[i],WirePlane3D(ChamberName,PlaneNames[i],Chamber3D, top,mgr, i+1)));}

   //Get Rotation and Translation, AddNode to top Volume
    TGeoRotation r1;
    TGeoTranslation t1;
    TGeoCombiTrans *comb;

    double tilt = hms-> GetDouble(Form("%s.Tilt =",ChamberName));
    double x0 = hms-> GetDouble(Form("%s.xPos =",ChamberName));
    double y0 = hms-> GetDouble(Form("%s.yPos =",ChamberName));
    double z0 = hms-> GetDouble(Form("%s.zPos =",ChamberName));
    
    //cerr << Form("%s.xpos is ",ChamberName) << x0 <<Form(" %s.ypos is ",ChamberName) << y0 <<Form(" %s.zpos is ",ChamberName) << z0 << endl;

    r1.SetAngles(90 - tilt,0,90,90,tilt,180);
    t1.SetTranslation(x0+CT/2.0, y0, z0);
    comb = new TGeoCombiTrans(t1, r1);
    top->AddNodeOverlap(Chamber3D,1,comb);

    cout<<"Chamber 3D is created!"<<endl;
}

WireChamber3D::~WireChamber3D(){}

void WireChamber3D::WireHit3D(string PlaneName, int WireNum)
{
      
  //TGeoTube *tube= (TGeoTube *) WirePlanes.find(PlaneName) ->second.Wires[(int) (WireNum/SPARSIFY)]->wire->GetShape();
       WirePlanes.find(PlaneName)->second.Wire3DHit(WireNum);
}

void WireChamber3D::clear()
{
    std::map<string, WirePlane3D>:: iterator itr;

    for (itr=WirePlanes.begin(); itr!= WirePlanes.end(); itr++)
        itr->second.clear();
}
