//************************************************************************* 
//  MWDChamber3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is used to create 3D simulation of MWDC and to show hit wires
//
//  Updated by Ben Davis-Purcell - 8/26/2014
//*************************************************************************  


#include "MWDChamber3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#define DEBUG_LEVEL 0


using namespace std;



MWDChamber3D::MWDChamber3D(char *name, int t, int n, double x, double y, double z, double width, double height, TGeoVolume *paddle)
{
   // Type 0 is a chamber that has double wire layers  e.g. XX'UVYY'
   // Type 1 is chamber with single wire layers e.g. XUY
   type = t;

   // n = actual number of wires
   // Num = number ow wires that we draw
   if ((n/5.0 - int(n/5.0))>0) Num =  (int) (n/5.0) + 1; // must be rounded up
   else Num = (int) (n/5.0);

   // First we create a frame
   Double_t starting_point1[3]={-(width + 1.0)/2.0+x,y,z};
   TGeoBBox *surface1 = new TGeoBBox("surface1",1.0,2,height/2.0,starting_point1);
   TGeoVolume *left_wall = new TGeoVolume("left_wall",surface1);
   left_wall->SetLineColor(kBlack);
   paddle->AddNode(left_wall,1);
   paddle->AddNode(left_wall,2, new TGeoTranslation((width + 1.0)+ x,0.0 + y,0.0 + z));

   Double_t starting_point2[3]={x,y,-(height+1.0)/2.0+z};
   TGeoBBox *surface2 = new TGeoBBox("surface2",(width + 2.0)/2.0,2,1.0,starting_point2);
   TGeoVolume *lower_wall = new TGeoVolume("lower_wall",surface2);
   lower_wall->SetLineColor(kBlack);
   paddle->AddNode(lower_wall,1);
   paddle->AddNode(lower_wall,2, new TGeoTranslation(x,0.0 + y, (height+1.0) + z)); 


   //Once we have frame, we can put wires into it
   if (type==0)
   {

     // X- wires
     for (int i = 0; i<Num; i++)
     {
	double fac = (height)*(i)/(Num);
        x1_wires[i] = new TWire3D(-width/2.0+x, -0.25, (height/2.0-fac)+y, width/2.0+x, -0.25, (height/2.0-fac)+y, 0.2, paddle);
     }

     for (int i = 0; i<Num; i++)
     {
        double fac =  (height)/Num/2.0 + (height)*(i)/(Num);
        x2_wires[i] = new TWire3D(-width/2.0+x, 0.25, (height/2.0-fac)+y, width/2.0+x, 0.25 , (height/2.0-fac)+y, 0.2, paddle);
     }
 
     // V-wires
     for (int i = 0; i<Num; i++)
     {
	double cn =- (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v1_wires[i] = new TWire3D (0.0+x - width/2.0, 1.25 , cn+y-height/2.0, width+x- width/2.0, 1.25, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);
          
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v1_wires[i] = new TWire3D (0.0+x - width/2.0, 1.25, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x- width/2.0, 1.25, height+y-height/2.0, 0.2, paddle);
	}
	else
	{
	  v1_wires[i] = new TWire3D((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.25, height*0.0+y-height/2.0, width+x- width/2.0, 1.25, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle);
	}
     }

     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/Num/2 - (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v2_wires[i] = new TWire3D (0.0+x - width/2.0, 1.75, cn+y-height/2.0, width+x- width/2.0, 1.75, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);   
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v2_wires[i] = new TWire3D (0.0+x - width/2.0, 1.75, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x - width/2.0, 1.75, height+y-height/2.0, 0.2, paddle);  
	}
	else
	{
	  v2_wires[i] = new TWire3D ((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.75, height*0.0+y-height/2.0, width+x- width/2.0, 1.75, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle); 
	}

     }


     // U-wires
     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);

	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TWire3D(0.0+x-width/2.0, -1.75, cn+y - height/2.0, width+x-width/2.0, -1.75, (-width*sqrt(3.)/3.0+ cn)+y - height/2.0, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u1_wires[i] = new TWire3D((cn-height)*3.0/sqrt(3.0)+x-width/2.0, -1.75, height+y- height/2.0, width+x-width/2.0, -1.75, (-sqrt(3.)/3.0*width+ cn)+y- height/2.0, 0.2, paddle);
	}
	else
	{
	  u1_wires[i] = new TWire3D(0.0+x -width/2.0, -1.75, cn+y - height/2.0, (cn*3.0/sqrt(3.0))+x-width/2.0+0.001, -1.75, 0.0+y -height/2.0, 0.2, paddle);
	}
     }

     for (int i = 0; i<Num; i++)
     {
	double cn =-(height*2.0/sqrt(3.))/(Num)/2 -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);


	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u2_wires[i] =  new TWire3D(0.0+x - width/2.0, -1.25, cn+y-height/2.0, width+x- width/2.0, -1.25, (-width*sqrt(3.)/3.0+ cn)+y-height/2.0+0.001, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u2_wires[i] =  new TWire3D((cn-height)*3.0/sqrt(3.0)+x - width/2.0, -1.25, height +y-height/2.0, width+x - width/2.0+0.001, -1.25, (-sqrt(3.)/3.0*width+ cn)+y-height/2.0+0.001, 0.2, paddle);
	}
	else
	{
	  u2_wires[i] =  new TWire3D(0.0+x- width/2.0, -1.25, cn+y- height/2.0, (cn*3.0/sqrt(3.0))+x- width/2.0 +0.001, -1.25, 0.0+y- height/2.0+0.0001, 0.2, paddle);
	}
     }

   }
   else
   {

     // X-wires
     for (int i = 0; i<Num; i++)
     {
	double fac = (height)*(i)/(Num);
        x1_wires[i] = new TWire3D(-width/2.0+x, 0.0, (height/2.0-fac)+y, width/2.0+x, 0.0, (height/2.0-fac)+y, 0.2, paddle);
     }

     // V- wires
     for (int i = 0; i<Num; i++)
     {
	double cn =- (height*2.0/sqrt(3.))/(Num)*(i) + height;

        if (cn <= height - sqrt(3.)/3.0*width && cn>=0.0 )
        {
	  v1_wires[i] = new TWire3D (0.0+x - width/2.0, 1.0 , cn+y-height/2.0, width+x- width/2.0, 1.0, (sqrt(3.)/3.0*width+ cn)+y-height/2.0, 0.2, paddle);
          
        }
        else if (cn>height - sqrt(3.)/3.0*width)
	{
	  v1_wires[i] = new TWire3D (0.0+x - width/2.0, 1.0, cn+y-height/2.0, (height-cn)*3.0/sqrt(3.)+x- width/2.0, 1.0, height+y-height/2.0, 0.2, paddle);
	}
	else
	{
	  v1_wires[i] = new TWire3D((3.0/sqrt(3.)*(-cn))+x - width/2.0, 1.0, height*0.0+y-height/2.0, width+x- width/2.0, 1.0, (width -3.0/sqrt(3.)*(-cn))*sqrt(3.)/3+y-height/2.0, 0.2, paddle);
	}
     }

     // U-wires
     for (int i = 0; i<Num; i++)
     {
	double cn = -(height*2.0/sqrt(3.))/(Num)*(i) + height+(width*sqrt(3.)/3.0);
	
	if (cn < height && cn >width*sqrt(3.)/3.0)
	{
	  u1_wires[i] = new TWire3D(0.0+x-width/2.0, -1.0, cn+y - height/2.0, width+x-width/2.0, -1.0, (-width*sqrt(3.)/3.0+ cn)+y - height/2.0, 0.2, paddle);
	}
	else if (cn>=height)
	{
	  u1_wires[i] = new TWire3D((cn-height)*3.0/sqrt(3.0)+x-width/2.0, -1.0, height+y- height/2.0, width+x-width/2.0, -1.0, (-sqrt(3.)/3.0*width+ cn)+y- height/2.0, 0.2, paddle);
	}
	else
	{
	  u1_wires[i] = new TWire3D(0.0+x -width/2.0, -1.0, cn+y - height/2.0, (cn*3.0/sqrt(3.0))+x-width/2.0+0.001, -1.0, 0.0+y -height/2.0, 0.2, paddle);
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
  int quotient = (int) i/5;
  if (quotient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****u1WireHit: "<<quotient<<endl;
#endif
    u1_wires[quotient]->SetColor(2);
  }
  
}

void MWDChamber3D::u2WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****u2WireHit: "<<quotient<<endl;
#endif
    u2_wires[quotient]->SetColor(2);
  }

}


void MWDChamber3D::v1WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****v1WireHit: "<<quotient<<endl;
#endif
    v1_wires[quotient]->SetColor(3);
  }

}


void MWDChamber3D::v2WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****v2WireHit: "<<quotient<<endl;
#endif
    v2_wires[quotient]->SetColor(3);
  }

}


void MWDChamber3D::x1WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****x1WireHit: "<<quotient<<endl;
#endif
    x1_wires[quotient]->SetColor(4);
  }

}


void MWDChamber3D::x2WireHit(int i)
{
  int quotient = (int) i/5;
  if (quotient<Num && type==0)
  {
#if DEBUG_LEVEL >= 3
    cout<<"****x2WireHit: "<<quotient<<endl;
#endif
    x2_wires[quotient]->SetColor(4);
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



