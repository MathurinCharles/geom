#ifndef CURVE_1D_BEZIERPARAM_H
#define CURVE_1D_BEZIERPARAM_H

#include <iostream>
#include "curve1D.h"

class Curve1DBezierParam : public Curve1D {
 public:
 Curve1DBezierParam(const QString &name) : Curve1D(name) {}
 Curve1DBezierParam(Curve1D *curve,const QString &name) : Curve1D(curve,name) {}
  
  QPainterPath path(float xmin,float xmax) {
    // TODO
    QPainterPath p;

    // empty test 
    if(empty()) 
      return p;

    // draw function 
    for(unsigned int x=0;x<nbFrames();x++) {
        p.lineTo(x,evalAt(x));
    }

    return p;
  }

  float evalAt(float x) {
    // BezierParam curve value
    unsigned int N = 10*nbPts();
    Vector2f controlPt;
    float t = x/(nbFrames()-1);
    std::cout << "Value of the abcissa x : " << x << std::endl;
    std::cout << "Value of the proportion x/nbFrames : " << t << std::endl;
    float OrdCasteljau[nbPts()][nbPts()];
    for(unsigned int j=0;j<nbPts();++j) {
        controlPt = _points[j];
        OrdCasteljau[0][j] = controlPt[1];
    }

    for(unsigned int i=1;i<nbPts();++i){
        for(unsigned int j=0;j<nbPts()-i;++j) {
            OrdCasteljau[i][j] = (1-t)*OrdCasteljau[i-1][j] + t*OrdCasteljau[i-1][j+1];
        }
    }
    return OrdCasteljau[nbPts()-1][0];
 }

 unsigned int nbFrames();
};


class Curve1DBezierParamConstructor : public Curve1DConstructor {
 public:
  virtual ~Curve1DBezierParamConstructor()                     {}
  virtual const QString type() const                      { return "BezierParamCurve";             }
  virtual Curve1D *create(const QString &name)            { return new Curve1DBezierParam(name);   }
  virtual Curve1D *create(Curve1D *c,const QString &name) { return new Curve1DBezierParam(c,name); }
};


#endif // CURVE_1D_BEZIERPARAM_H
