#ifndef CURVE_2D_CubicHermiteSpline_H
#define CURVE_2D_CubicHermiteSpline_H

#include "curve2D.h"

float h00(float t){ return 2*t*t*t - 3*t*t + 1 ;};
float h10(float t){ return t*t*t - 2*t*t + t ;};
float h01(float t){ return -2*t*t*t + 3*t*t ;};
float h11(float t){ return t*t*t - t*t ;};

class Curve2DCubicHermiteSpline : public Curve2D {
 public:
 Curve2DCubicHermiteSpline(const QString &name) : Curve2D(name) {}
 Curve2DCubicHermiteSpline(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  

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
    

    Xaxis[0] = pt[0];
    Yaxis[0] = pt[1];
    Xaxis[1] = pt2[0];
    Yaxis[1] = pt2[1];
    Xderivates[0] = Xaxis[1]-Xaxis[0];
    Yderivates[0] = Yaxis[1]-Yaxis[0];
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

    
    float t;

    p.moveTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      for (unsigned int k=1;k<11;++k){
        t = (float) k / 10;
        pt[0] = h00(t)*Xaxis[i-1] + h10(t)*Xderivates[i-1] + h01(t)*Xaxis[i] + h11(t)*Xderivates[i];
        pt[1] = h00(t)*Yaxis[i-1] + h10(t)*Yderivates[i-1] + h01(t)*Yaxis[i] + h11(t)*Yderivates[i];
        p.lineTo(pt[0],pt[1]);
      }
    }
    return p;
  }
};


class Curve2DCubicHermiteSplineConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DCubicHermiteSplineConstructor()                     {}
  virtual const QString type() const                      { return "CubicHermiteSplineCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DCubicHermiteSpline(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DCubicHermiteSpline(c,name); }
};


#endif // CURVE_2D_CubicHermiteSpline_H
