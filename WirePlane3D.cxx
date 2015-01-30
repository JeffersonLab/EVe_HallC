#include "WirePlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

 WirePlane3D::WirePlane3D(char* ChamberName,string PlaneName,TGeoVolume* WireChamber3D, int color )
{
   wirecolor = color;

   //Get all data from HMS.txt
   GetVariables *hms = new GetVariables("HMS.txt");

   //Distance of the wireplane from the front surface of chamber
   double PlaneDist= hms->GetDouble(Form("%s.%s.Dist =",ChamberName,PlaneName.c_str()));
   double Thickness= hms->GetDouble(Form("%s.Thickness =",ChamberName));

   // Height and Width of the wireplane
   double H= 100.0*hms->GetDouble(Form("%s.Height =",ChamberName));
   double W= 100.0*hms->GetDouble(Form("%s.Width =",ChamberName));

   //Number of Wires,radius of wires, and Angle of Wires
   int nWires = hms->GetInt(Form("%s.%s.NumWires =",ChamberName,PlaneName.c_str()));
   double R=WIRE3DRADIUS;
   double Angle = hms->GetDouble(Form("%s.%s.WireAngle =",ChamberName,PlaneName.c_str()));

   double Ang= Angle*3.14159/180.0;
   TGeoBBox *WP= new TGeoBBox(Form("%s.%s.WirePlane",ChamberName,PlaneName.c_str()),R, W/2, H/2);
   WirePlane = new TGeoVolume (Form("%s.%s.WP",ChamberName,PlaneName.c_str()),WP);

   // Below is similar to WirePlane2D classes
   // nWires = actual number of wires
   // WireNum = number of wires that we draw

   if ((nWires/5.0 - int(nWires/5.0))>0) WireNum =  (int) (nWires/5.0) + 1; // must be rounded up
   else WireNum = (int) (nWires/5.0);

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
	 // cerr << "fac th 0.0 = " << fac << "\n";
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
	 // cerr << "fac th 90.0 = " << fac << "\n";
         //cerr << "x1 = " << x1 << "\n";
       }
   }

    else if(Angle>0.0 && Angle<90.0)
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
    TGeoTranslation* WPtrans= new TGeoTranslation(-Thickness/2.0+PlaneDist,0,0);
    WireChamber3D->AddNode(WirePlane,1,WPtrans);
}

WirePlane3D::~WirePlane3D()
{
}

void WirePlane3D::Wire3DHit(int Num)
{
    int Fac = Num/5;
    if(Fac<=WireNum/5)
    {
       //Wires[Num/5]->wire->SetTubeDimensions(0,5*WIRE3DRADIUS,Wires[Num/5]->wire->GetDz()); 
       // Wires[Num/5]->wire->GetNode("tube")->GetVolume()->GetShape()->SetTubeDimensions(0,5*WIRE3DRADIUS,Wires[Num/5]->wire->GetNode("tube")->GetDz());	
        TGeoTube* tube= (TGeoTube*) Wires[Num/5]->wire->GetNode("tube")->GetVolume()->GetShape();
        tube->SetTubeDimensions(0,WIRE3DRADIUS,tube->GetDz());
       Wires[Fac]->wire->SetLineColor(wirecolor);
    }
}

void WirePlane3D::clear()
{
    for (int i=0; i<WireNum; i++)
    {
        Wires[i]->wire->SetLineColor(kBlack);
	TGeoTube* tube= (TGeoTube*) Wires[i]->wire->GetNode("tube")->GetVolume()->GetShape();
        tube->SetTubeDimensions(0,WIRE3DRADIUS,tube->GetDz());
    }
}
