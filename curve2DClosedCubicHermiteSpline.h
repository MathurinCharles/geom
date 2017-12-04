#ifndef CURVE_2D_CUBICHERMITESPLINE_H
#define CURVE_2D_CUBICHERMITESPLINE_H
float h00(float t){ return 2*t*t*t - 3*t*t + 1 ;};
float h10(float t){ return t*t*t - 2*t*t + t ;};
float h01(float t){ return -2*t*t*t + 3*t*t ;};
float h11(float t){ return t*t*t - t*t ;};
#endif
#ifndef CURVE_2D_CLOSEDCUBICHERMITESPLINE_H
#define CURVE_2D_CLOSEDCUBICHERMITESPLINE_H

#include "curve2D.h"

class Curve2DClosedCubicHermiteSpline : public Curve2D {
 public:
 Curve2DClosedCubicHermiteSpline(const QString &name) : Curve2D(name) {}
 Curve2DClosedCubicHermiteSpline(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  

  QPainterPath path(float frame) {
   QPainterPath p;
   
    if(nbPts()==0) 
      return p;
    if(nbPts()==1) 
      return p;
    if(nbPts()==2) {
      Vector2f pt = evalAnimPt(get(0),frame);
      p.moveTo(pt[0],pt[1]);
      pt = evalAnimPt(get(1),frame);
      p.lineTo(pt[0],pt[1]);
      return p;
    }

    float Xaxis[nbPts()];
    float Xderivates[nbPts()];
    float Yaxis[(nbPts())];
    float Yderivates[(nbPts())];
    float Lderivate;
    float Rderivate;
    Vector2f pt = evalAnimPt(get(0),frame);
    Vector2f pt2 = evalAnimPt(get(1),frame);
    Vector2f pdeb = pt;
    

    Xaxis[0] = pt[0];
    Yaxis[0] = pt[1];
    Xaxis[1] = pt2[0];
    Yaxis[1] = pt2[1];
    for (unsigned int i=2;i<nbPts();++i){
      pt2 = evalAnimPt(get(i),frame);
      Xaxis[i] = pt2[0];
      Yaxis[i] = pt2[1];
      Lderivate = Xaxis[i-1]-Xaxis[i-2];
      Rderivate = Xaxis[i]-Xaxis[i-1];
      Xderivates[i-1] = (Lderivate + Rderivate)/2;
      Lderivate = Yaxis[i-1] - Yaxis[i-2];
      Rderivate = Yaxis[i] - Yaxis[i-1];
      Yderivates[i-1] = (Lderivate + Rderivate)/2;
    }
    Xderivates[nbPts()-1] = Xaxis[nbPts()-1]-Xaxis[nbPts()-2];
    Yderivates[nbPts()-1] = Yaxis[nbPts()-1]-Yaxis[nbPts()-2];
    Xderivates[0] = (Xaxis[1]-Xaxis[0] + Xaxis[0]-Xaxis[nbPts()-1])/2;
    Yderivates[0] = (Yaxis[1]-Yaxis[0] + Yaxis[0]-Yaxis[nbPts()-1])/2;

    
    float t;

    p.moveTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      for (unsigned int k=1;k<21;++k){
        t = (float) k / 20;
        pt[0] = h00(t)*Xaxis[i-1] + h10(t)*Xderivates[i-1] + h01(t)*Xaxis[i] + h11(t)*Xderivates[i];
        pt[1] = h00(t)*Yaxis[i-1] + h10(t)*Yderivates[i-1] + h01(t)*Yaxis[i] + h11(t)*Yderivates[i];
        p.lineTo(pt[0],pt[1]);
      }
    }
    for (unsigned int k=1;k<21;++k){
        t = (float) k / 20;
        pt[0] = h00(t)*Xaxis[nbPts()-1] + h10(t)*Xderivates[nbPts()-1] + h01(t)*Xaxis[0] + h11(t)*Xderivates[0];
        pt[1] = h00(t)*Yaxis[nbPts()-1] + h10(t)*Yderivates[nbPts()-1] + h01(t)*Yaxis[0] + h11(t)*Yderivates[0];
        p.lineTo(pt[0],pt[1]);
      }
    return p;
  }
};


class Curve2DClosedCubicHermiteSplineConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DClosedCubicHermiteSplineConstructor()                     {}
  virtual const QString type() const                      { return "ClosedCubicHermiteSplineCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DClosedCubicHermiteSpline(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DClosedCubicHermiteSpline(c,name); }
};


#endif // CURVE_2D_ClosedCubicHermiteSpline_H
