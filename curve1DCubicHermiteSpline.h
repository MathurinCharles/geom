#ifndef CURVE_1D_CUBICHERMITESPLINE_H
#define CURVE_1D_CUBICHERMITESPLINE_H

#include "curve1D.h"


class Curve1DCubicHermiteSpline : public Curve1D {
 public:
 Curve1DCubicHermiteSpline(const QString &name) : Curve1D(name) {}
 Curve1DCubicHermiteSpline(Curve1D *curve,const QString &name) : Curve1D(curve,name) {}
  
  QPainterPath path(float xmin,float xmax) {
    QPainterPath p;

    // empty test 
    if(empty()) 
      return p;

    // left part 
    if(xmin<_points[0][0]) {
      p.moveTo(xmin,_points[0][1]);
      p.lineTo(_points[0][0],_points[0][1]);
    } else {
      p.moveTo(_points[0][0],_points[0][1]);
    }

    // draw function
    
    float Xaxis[nbPts()];
    float Xderivates[nbPts()];
    float Yaxis[(nbPts())];
    float Yderivates[(nbPts())];
    float Lderivate;
    float Rderivate;
    Vector2f pt = _points[0];
    Vector2f pt2 = _points[1]; 
    Xaxis[0] = pt[0];
    Yaxis[0] = pt[1];
    Xaxis[1] = pt2[0];
    Yaxis[1] = pt2[1];
    Xderivates[0] = Xaxis[1]-Xaxis[0];
    Yderivates[0] = Yaxis[1]-Yaxis[0];
    for (unsigned int i=2;i<nbPts();++i){
      pt2 = _points[i];
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
    p.lineTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      for (unsigned int k=1;k<11;++k){
        t = (float) k / 10;
        pt[0] = (2*t*t*t - 3*t*t + 1)*Xaxis[i-1] + (t*t*t - 2*t*t + t)*Xderivates[i-1] + (-2*t*t*t + 3*t*t)*Xaxis[i] + (t*t*t - t*t)*Xderivates[i];
        pt[1] = (2*t*t*t - 3*t*t + 1)*Yaxis[i-1] + (t*t*t - 2*t*t + t)*Yderivates[i-1] + (-2*t*t*t + 3*t*t)*Yaxis[i] + (t*t*t - t*t)*Yderivates[i];
        p.lineTo(pt[0],pt[1]);
      }
    }
    // right part 
    if(xmax>_points[nbPts()-1][0]) {
      p.lineTo(xmax,_points[nbPts()-1][1]);
    }
    return p;
  }

  float evalAt(float x) {
    // special cases 
    if(empty()) return 0.0f;
    if(x<=_points[0][0]) return _points[0][1];
    if(x>=_points[nbPts()-1][0]) return _points[nbPts()-1][1];

    // CubicHermiteSpline interp
    
    for(unsigned int i=0;i<nbPts()-1;++i) {
      if(_points[i][0]==x) {
	      return _points[i][1];
      }
    }
    float Xaxis[nbPts()];
    float Xderivates[nbPts()];
    float Yaxis[(nbPts())];
    float Yderivates[(nbPts())];
    float Lderivate;
    float Rderivate;
    Vector2f pt = _points[0];
    Vector2f pt2 = _points[1];
    Xaxis[0] = pt[0];
    Yaxis[0] = pt[1];
    Xaxis[1] = pt2[0];
    Yaxis[1] = pt2[1];
    Xderivates[0] = Xaxis[1]-Xaxis[0];
    Yderivates[0] = Yaxis[1]-Yaxis[0];
    for (unsigned int i=2;i<nbPts();++i){
      pt2 = _points[i];
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
    unsigned int ind = 0;
    while (ind< nbPts()-1 && !(Xaxis[ind]<= x && x<Xaxis[ind+1])){
      ind++;
    }
    t = (x-Xaxis[ind])/(Xaxis[ind+1]-Xaxis[ind]);
    return (2*t*t*t - 3*t*t + 1)*Yaxis[ind] + (t*t*t - 2*t*t + t)*Yderivates[ind] + (-2*t*t*t + 3*t*t)*Yaxis[ind+1] + (t*t*t - t*t)*Yderivates[ind+1];
  }
};


class Curve1DCubicHermiteSplineConstructor : public Curve1DConstructor {
 public:
  virtual ~Curve1DCubicHermiteSplineConstructor()                     {}
  virtual const QString type() const                      { return "CubicHermiteSplineCurve";             }
  virtual Curve1D *create(const QString &name)            { return new Curve1DCubicHermiteSpline(name);   }
  virtual Curve1D *create(Curve1D *c,const QString &name) { return new Curve1DCubicHermiteSpline(c,name); }
};

#endif // CURVE_1D_CUBICHERMITESPLINE_H
