//************************************************************************* 
//  MWDChamber3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create 3D simulation of MWDC and to show hit wires
//
//
//*************************************************************************  


#include "MWDChamber3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0


using namespace std;



MWDChamber3D::MWDChamber3D(char *ime, int t, int n, double x, double y, double z, double width, double height, TGeoVolume *paddle)
{
   // Type 0 is a chamber that has double wire layers  e.g. UUXXYY
   // Type 1 is chamber with single wire layers e.g. UXV
   type = t;

   // n = actual number of wires
   // Num = number ow wires that we draw
   if ((n/5.0 - int(n/5.0))>0) Num =  (int) (n/5.0) + 1; // mus be rounded up
   else Num = (int) (n/5.0);

#if DEBUG_LEVEL >= 3
   cout<<"IMAMO n = "<<n<<", Num = "<<Num<<endl;
#endif

   // First we create a frame
   Double_t izhodisce1[3]={-(width + 1.0)/2.0+x,y,z};
   TGeoBBox *ploskev1 = new TGeoBBox("ploskev1",1.0,2,height/2.0,izhodisce1);
   TGeoVolume *leva_stena = new TGeoVolume("leva_stena",ploskev1);
   leva_stena->SetLineColor(kBlack);
   paddle->AddNode(leva_stena,1);
   paddle->AddNode(leva_stena,2, new TGeoTranslation((width + 1.0)+ x,0.0 + y,0.0 + z));

   Double_t izhodisce2[3]={x,y,-(height+1.0)/2.0+z};
   TGeoBBox *ploskev2 = new TGeoBBox("ploskev2",(width + 2.0)/2.0,2,1.0,izhodisce2);
   TGeoVolume *spodnja_stena = new TGeoVolume("spodnja_stena",ploskev2);
   spodnja_stena->SetLineColor(kBlack);
   paddle->AddNode(spodnja_stena,1);
   paddle->AddNode(spodnja_stena,2, new TGeoTranslation(x,0.0 + y, (height+1.0) + z)); 


   //Once we have frame, we can put wires into it
   if (type==0)
   {

     // X- wires
     for (int i = 0; i<Num; i++)
     {
	double fac = (height)*(i)/(Num);
        x1_wires[i] = new TPremica3D(-width/2.0+x, -0.25, (height/2.0-fac)+y, width/2.0+x, -0.25, (height/2.0-fac)+y, 0.2, paddle);
     }

     for (int i = 0; i<Num; i++)
     {
        double fac =  (height)/Num/2.0 + (height)*(i)/(Num);
        x2_wires[i] = new TPremica3D(-width/2.0+x, 0.25, (height/2.0-fac)+y, width/2.0+x, 0.25 , (height/2.0-fac)+y, 0.2, paddle);
     }
 
     // V-wires
     for (int i = 0; i<Num; i++)
     {
	double cn =- (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v1_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.25 , cn+y-height/2.0, width+x- width/2.0, 1.25, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);
          
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v1_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.25, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x- width/2.0, 1.25, height+y-height/2.0, 0.2, paddle);
	}
	else
	{
	  v1_wires[i] = new TPremica3D((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.25, height*0.0+y-height/2.0, width+x- width/2.0, 1.25, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle);
	}
     }

     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/Num/2 - (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v2_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.75, cn+y-height/2.0, width+x- width/2.0, 1.75, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);   
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v2_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.75, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x - width/2.0, 1.75, height+y-height/2.0, 0.2, paddle);  
	}
	else
	{
	  v2_wires[i] = new TPremica3D ((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.75, height*0.0+y-height/2.0, width+x- width/2.0, 1.75, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle); 
	}

     }


     // U-wires
     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);

	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TPremica3D(0.0+x-width/2.0, -1.75, cn+y - height/2.0, width+x-width/2.0, -1.75, (-width*sqrt(3.)/3.0+ cn)+y - height/2.0, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u1_wires[i] = new TPremica3D((cn-height)*3.0/sqrt(3.0)+x-width/2.0, -1.75, height+y- height/2.0, width+x-width/2.0, -1.75, (-sqrt(3.)/3.0*width+ cn)+y- height/2.0, 0.2, paddle);
	}
	else
	{
	  u1_wires[i] = new TPremica3D(0.0+x -width/2.0, -1.75, cn+y - height/2.0, (cn*3.0/sqrt(3.0))+x-width/2.0+0.001, -1.75, 0.0+y -height/2.0, 0.2, paddle);
	}
     }

     for (int i = 0; i<Num; i++)
     {
	double cn =-(height*2.0/sqrt(3.))/(Num)/2 -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);


	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u2_wires[i] =  new TPremica3D(0.0+x - width/2.0, -1.25, cn+y-height/2.0, width+x- width/2.0, -1.25, (-width*sqrt(3.)/3.0+ cn)+y-height/2.0+0.001, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u2_wires[i] =  new TPremica3D((cn-height)*3.0/sqrt(3.0)+x - width/2.0, -1.25, height +y-height/2.0, width+x - width/2.0+0.001, -1.25, (-sqrt(3.)/3.0*width+ cn)+y-height/2.0+0.001, 0.2, paddle);
	}
	else
	{
	  u2_wires[i] =  new TPremica3D(0.0+x- width/2.0, -1.25, cn+y- height/2.0, (cn*3.0/sqrt(3.0))+x- width/2.0 +0.001, -1.25, 0.0+y- height/2.0+0.0001, 0.2, paddle);
	}
     }

   }
   else
   {

     // X-wires
     for (int i = 0; i<Num; i++)
     {
	double fac = (height)*(i)/(Num);
        x1_wires[i] = new TPremica3D(-width/2.0+x, 0.0, (height/2.0-fac)+y, width/2.0+x, 0.0, (height/2.0-fac)+y, 0.2, paddle);
     }

     // V- wires
     for (int i = 0; i<Num; i++)
     {
	double cn =- (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v1_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.0 , cn+y-height/2.0, width+x- width/2.0, 1.0, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);
          
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v1_wires[i] = new TPremica3D (0.0+x - width/2.0, 1.0, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x- width/2.0, 1.0, height+y-height/2.0, 0.2, paddle);
	}
	else
	{
	  v1_wires[i] = new TPremica3D((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.0, height*0.0+y-height/2.0, width+x- width/2.0, 1.0, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle);
	}
     }

     // U-wires
     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);
	
	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TPremica3D(0.0+x-width/2.0, -1.0, cn+y - height/2.0, width+x-width/2.0, -1.0, (-width*sqrt(3.)/3.0+ cn)+y - height/2.0, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u1_wires[i] = new TPremica3D((cn-height)*3.0/sqrt(3.0)+x-width/2.0, -1.0, height+y- height/2.0, width+x-width/2.0, -1.0, (-sqrt(3.)/3.0*width+ cn)+y- height/2.0, 0.2, paddle);
	}
	else
	{
	  u1_wires[i] = new TPremica3D(0.0+x -width/2.0, -1.0, cn+y - height/2.0, (cn*3.0/sqrt(3.0))+x-width/2.0+0.001, -1.0, 0.0+y -height/2.0, 0.2, paddle);
	}
     }
   }

   cout<<"MWDChamber is created!"<<endl;
}

MWDChamber3D::~MWDChamber3D()
{

}

void MWDChamber3D::u1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****u1WireHit: "<<kvocient<<endl;
#endif
    u1_wires[kvocient]->SetColor(2);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber3D::u2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****u2WireHit: "<<kvocient<<endl;
#endif
    u2_wires[kvocient]->SetColor(2);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber3D::v1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****v1WireHit: "<<kvocient<<endl;
#endif
    v1_wires[kvocient]->SetColor(3);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber3D::v2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****v2WireHit: "<<kvocient<<endl;
#endif
    v2_wires[kvocient]->SetColor(3);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber3D::x1WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****x1WireHit: "<<kvocient<<endl;
#endif
    x1_wires[kvocient]->SetColor(4);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}


void MWDChamber3D::x2WireHit(int i)
{
  int kvocient = (int) i/5;
  if (kvocient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****x2WireHit: "<<kvocient<<endl;
#endif
    x2_wires[kvocient]->SetColor(4);
  }
  else
  {
#if DEBUG_LEVEL >= 3
    cout<<"No wire with this number"<<endl;
#endif
  }
}

void MWDChamber3D::clear()
{
  for (int i = 0; i<Num; i++)
  {
    u1_wires[i]->SetColor(1);
    if (type==0) u2_wires[i]->SetColor(1);
    v1_wires[i]->SetColor(1);
    if (type==0) v2_wires[i]->SetColor(1);
    x1_wires[i]->SetColor(1);
    if (type==0) x2_wires[i]->SetColor(1);
  }
}



