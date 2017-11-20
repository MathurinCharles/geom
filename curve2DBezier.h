#ifndef CURVE_2D_BEZIER_H
#define CURVE_2D_BEZIER_H

#include "curve2D.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

class Curve2DBezier : public Curve2D {
 public:
 Curve2DBezier(const QString &name) : Curve2D(name) {}
 Curve2DBezier(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}

  QPainterPath path(float frame) {
    QPainterPath p;
    if(nbPts()==0) 
      return p;

    Vector2f pt = evalAnimPt(get(0),frame);
    Vector2f controlPt;

    p.moveTo(pt[0],pt[1]);
    //to modify

    unsigned int N = 10*nbPts();
    float t;
    float AbsCasteljeau[nbPts()][nbPts()];
    float OrdCasteljeau[nbPts()][nbPts()];
    for(unsigned int j=0;j<nbPts();++j) {
        controlPt = evalAnimPt(get(j),frame);
        AbsCasteljeau[0][j] = controlPt[0];
        OrdCasteljeau[0][j] = controlPt[1];
    }
    for(unsigned int k=1;k<N+1;++k) {
        t = (float) k/(float) N;
        for(unsigned int i=1;i<nbPts();++i){
            for(unsigned int j=0;j<nbPts()-i;++j) {
                AbsCasteljeau[i][j] = (1-t)*AbsCasteljeau[i-1][j] + t*AbsCasteljeau[i-1][j+1];
                OrdCasteljeau[i][j] = (1-t)*OrdCasteljeau[i-1][j] + t*OrdCasteljeau[i-1][j+1];
        }
    }
        p.lineTo(AbsCasteljeau[nbPts()-1][0],OrdCasteljeau[nbPts()-1][0]);
    }
    return p;
  }
};


class Curve2DBezierConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DBezierConstructor()                     {}
  virtual const QString type() const                      { return "BezierCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DBezier(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DBezier(c,name); }
};


#endif // CURVE_2D_BEZIER_H
