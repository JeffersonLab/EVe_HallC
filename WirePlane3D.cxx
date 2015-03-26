
#include "WirePlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

WirePlane3D::WirePlane3D(char* ChamberName,string PlaneName,TGeoVolume* WireChamber3D,TGeoVolume* Top,TGeoManager* Mgr,int color)
{
   wirecolor = color;

   //Get all data from HMS.txt
   GetVariables *HMS = new GetVariables("HMS.txt");

   //Distance of the wireplane from the front surface of chamber
   double PlaneDist= HMS->GetDouble(Form("%s.%s.Dist =",ChamberName,PlaneName.c_str()));
   double Thickness= HMS->GetDouble(Form("%s.Thickness =",ChamberName));

   // Height and Width of the wireplane
   double H= 100*HMS->GetDouble(Form("%s.Height =",ChamberName));
   double W= 100*HMS->GetDouble(Form("%s.Width =",ChamberName));

   //Number of Wires,radius of wires, and Angle of Wires
   int nWires = HMS->GetInt(Form("%s.%s.NumWires =",ChamberName,PlaneName.c_str()));
   double R=WIRE3DRADIUS;
   double Angle = HMS->GetDouble(Form("%s.%s.WireAngle =",ChamberName,PlaneName.c_str()));

   double Ang= Angle*3.14159/180.0;
   TGeoBBox *WP= new TGeoBBox(Form("%s.%s.WirePlane",ChamberName,PlaneName.c_str()),R, W/2, H/2);
   WirePlane = new TGeoVolume (Form("%s.%s.WP",ChamberName,PlaneName.c_str()),WP);

   // Below is similar to WirePlane2D classes
   // nWires = actual number of wires
   // WireNum = number of wires that we draw

   if ((double(nWires)/SPARSIFY - nWires/SPARSIFY)>0){
     WireNum = nWires/SPARSIFY + 1; // must be rounded up
   } else {
     WireNum = nWires/SPARSIFY;
   }

   //Draw Wires, assign them with index
   double d;
   double y1,z1,y2,z2,fac;

   if(Angle==0.0)
   {
       for(int n=0;n<WireNum;n++)
       {
	 fac=((double)n/(double)WireNum);
	 y1=W/2;
	 z1=-H/2+H*(1-fac);
	 y2=-W/2;
	 z2=-H/2+H*(1-fac);
	 Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R, WirePlane, n));
       }
   }

   else if(Angle==90.0)
    {
        for(int n=0;n<WireNum;n++)
       {
         fac=((double)n/(double)WireNum);
         y1=W/2-W*fac;
         z1=-H/2;
         y2=W/2-W*fac;
         z2=H/2;
         Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R,WirePlane, n));
       }
   }
   else if(fabs(Ang)<atan(H/W)){
   if(Angle>0.0 && Angle<90.0)
    {
       d=H+W*tan(Ang);
       // cerr << "d = " << d << " ,b+a*tan(th) =" << b+a*tan(th) << "\n";
       for(int  n=0; n<WireNum; n++)
       {
	   fac=((double)n/(double)WireNum);

           //  cerr << "fac = " << fac << "\n";

           if((W*tan(Ang)-d*fac)>0)
           {
               y1=W/2;
               z1=H/2-d*fac;
               y2=W/2-d*fac/tan(Ang);
               z2=H/2;
           }
           else if((H-d*fac)<0)
           {
               y1=W/2-(d*fac-H)/tan(Ang);
               z1=-H/2;
               y2=-W/2;
               z2=H/2-d*fac+W*tan(Ang);
           }
           else
           {
               y1=W/2;
               z1=H/2-d*fac;
               y2=-W/2;
               z2=H/2-d*fac+W*tan(Ang);
           }
           Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R,WirePlane, n));
       }
   }

   else if (Angle<0.0 && Angle > -90.0)
   {
       Ang=-Ang;
       d=H+W*tan(Ang);
       // cerr << "d = " << d << " ,b+a*tan(th) =" << b+a*tan(th) << "\n" << "tan(th)= " << tan(th) << "\n";

       for(int  n=0; n<WireNum; n++)
       {
	   fac=((double)n/(double)WireNum);

           // cerr << "fac = " << fac << "\n";

           if((d*fac-W*tan(Ang))<0)
           {
	       y1=-W/2+d*fac/tan(Ang);
               z1=H/2;
               y2=-W/2;
               z2=-H/2+d*(1-fac)-W*tan(Ang);
           }
           else if((d*(1-fac)-W*tan(Ang))<0)
           {
	           y2=W/2-d*(1-fac)/tan(Ang);
               z2=-H/2;
               y1=W/2;
               z1=-H/2+d*(1-fac);
           }
           else
           {
               y1=W/2;
               z1=-H/2+d*(1-fac);
               y2=-W/2;
               z2=-H/2+d*(1-fac)-W*tan(Ang);
           }
        Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R,WirePlane,n));
	//cerr << "Draw Wire.\n";
       }
   }
   }
   
   else {
    
    if(Angle>0.0)
    {
       d=W+H/tan(Ang);
       for(int  n=0; n<WireNum; n++)
       {
	   fac=((double)n/(double)WireNum);

           if((d*fac)<(H/tan(Ang)))
           {
               y1=W/2;
               z1=H/2-d*fac*tan(Ang);
               y2=W/2-d*fac;
               z2=H/2;
           }
           else if((W-d*fac)<0)
           {
               y1=W/2-d*fac+H/tan(Ang);
               z1=-H/2;
               y2=-W/2;
               z2=H/2-(d*fac-W)*tan(Ang);
           }
           else
           {
               y1=W/2-d*fac+H/tan(Ang);
               z1=-H/2;
               y2=W/2-d*fac;
               z2=H/2;
           }
           Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R,WirePlane, n));
       }
   }

   else if (Angle<0.0)
   {
       Ang=-Ang;
       d=W+H/tan(Ang);

       for(int  n=0; n<WireNum; n++)
       {
	   fac=((double)n/(double)WireNum);

           if((d*fac-H/tan(Ang))<0)
           {
	       y1=W/2;
               z1=-H/2+(d*fac)*tan(Ang);
               y2=W/2-d*fac;
               z2=-H/2;
           }
           else if((d*fac-W)>0)
           {
	       y2=-W/2;
               z2=-H/2+(d*fac-W)*tan(Ang);
               y1= W/2-d*fac+H/tan(Ang);
               z1=H/2;
           }
           else
           {
               y1=W/2-d*fac;
               z1=-H/2;
               y2=W/2-d*fac+H/tan(Ang);
               z2=H/2;
           }
        Wires.push_back(new TWire3D(Angle,y1,z1,y2,z2,R,WirePlane,n));
	//cerr << "Draw Wire.\n";
       }
     }
     } 

     
     

   cerr << "WirePlane " << PlaneName << " is created." << endl;
    
    TGeoTranslation* WPtrans= new TGeoTranslation(-Thickness/2.0+PlaneDist,0,0);
    WireChamber3D->AddNodeOverlap(WirePlane,1,WPtrans);

    //To handle wirehit
    mgr = Mgr;
    TGeoTube* Hittube = new TGeoTube(Form("Hittube %s %s",ChamberName,PlaneName.c_str()),0.0,0.0,0.0);
    HitTube = new TGeoVolume(Form("Hittube.%s.%s",ChamberName,PlaneName.c_str()),Hittube);
    HitTube->SetLineColor(wirecolor+1); //make hittube different color for different track
       
    // Add HitTube to top Volume, which makes all HitTube shown in x=0 plane, wrong
    //printf("%s\n",HitTube->GetNode(Form("Hittube.%s.%s",ChamberName,PlaneName.c_str()))->GetPath());
    Top->AddNodeOverlap(HitTube,1);    
    path = Form("/TOP_1/Hittube.%s.%s_1",ChamberName,PlaneName.c_str());
    PN= mgr -> MakePhysicalNode(path);
    
    

    //use path through WirePlane, so HitTube stay in WirePlane it belongs, but segfault when run
    //WirePlane->AddNodeOverlap(HitTube,1);
    //path = Form("/TOP_1/%s.Chamber_1/%s.%s.WP_1/Hittube.%s.%s_1",ChamberName,ChamberName,PlaneName.c_str(),ChamberName,PlaneName.c_str());
    //PN= mgr -> MakePhysicalNode(path);
    //path = Form("Hittube.%s.%s_1",ChamberName,PlaneName.c_str());
    
    //Below using PNEntry, setting path to WirePlane, segfault when change to 3D display
    //TGeoPNEntry * pne = new TGeoPNEntry(Form("Hittube.%s.%s_1",ChamberName,PlaneName.c_str()),Form("path_to_Hittube.%s.%s_1",ChamberName,PlaneName.c_str()));
    //pne -> SetPhysicalNode(PN);
    //cerr << "Path for hit wire is " << path << endl;
    //WireChamber3D->AddNodeOverlap(WirePlane,1,WPtrans);
    
}

WirePlane3D::~WirePlane3D()
{
}

void WirePlane3D::Wire3DHit(int Num)
{   
    int Fac = Num/SPARSIFY;
    /*
    if(Fac<=WireNum)
    {
       Wires[Fac]->wire->SetLineColor(wirecolor);
       TGeoTube* tube= (TGeoTube*) Wires[Fac]->wire->GetShape();
       tube->SetTubeDimensions(0, 10*WIRE3DRADIUS, tube->GetDz());
    } else {
      cerr << Form("WARNING:  WirePlane3D::Wire3DHit(%d): %d > WireNum(%d)/SPARSIFY", Num, Fac, WireNum) << endl;
      }  */
    TGeoTube* Hittube = Wires[Fac]->Wiretube;
    TGeoCombiTrans* Hitcomb= Wires[Fac]-> Wirecomb;
    TGeoTube* Showtube = new TGeoTube("HitTube",0.0,10.0*(Hittube->GetRmax()),Hittube->GetDz());
    PN->Align(Hitcomb, Showtube);
    
}

void WirePlane3D::clear()
{
    /*
    for (int i=0; i<WireNum; i++)
    {
       Wires[i]->wire->SetLineColor(kBlack);
       TGeoTube* tube= (TGeoTube*) Wires[i]->wire->GetShape();
       tube->SetTubeDimensions(0, WIRE3DRADIUS, tube->GetDz());
       }*/
  //PN= mgr -> MakePhysicalNode(path);
  PN -> Align(new TGeoTranslation(0,0,0), new TGeoTube("Tube",0.0,0.0,0.0));
}
