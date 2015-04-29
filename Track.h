//*************************************************************************
//  Track.h  - 4/14/2008
//
//  by miham
//
//  This class handles Track projections in projection view
//
//*************************************************************************

#ifndef ROOT_Track
#define ROOT_Track

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "CStransform.h"


class Track
{

public:
    Track(CStransform *cst);
    virtual ~Track();
    void DrawMe(double x1, double y1, double x2, double y2);
    void Clear();
protected:
    TLine *line;
    CStransform *transformation;

};

#endif
