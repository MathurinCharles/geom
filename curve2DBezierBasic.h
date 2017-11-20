#ifndef CURVE_2D_BEZIERBASIC_H
#define CURVE_2D_BEZIERBASIC_H

#include "curve2D.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

unsigned int factorial(unsigned int n){
    if (n==0) return 1;
    return n*factorial(n-1);
}

float Bezier(unsigned int n, unsigned int i, float t){
    return (factorial(n)*pow(t, i)*pow(1-t, n-i))/(factorial(i)*factorial(n-i));
}

class Curve2DBezierBasic : public Curve2D {
 public:
 Curve2DBezierBasic(const QString &name) : Curve2D(name) {}
 Curve2DBezierBasic(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}

  QPainterPath path(float frame) {
    QPainterPath p;
    if(nbPts()==0) 
      return p;

    Vector2f pt = evalAnimPt(get(0),frame);
    Vector2f controlPt;

    p.moveTo(pt[0],pt[1]);
    //to modify

    unsigned int N = 200;
    float t;
    float coefBezier;
    for(unsigned int i=1;i<N+1;++i) {
        t = (float) i/(float) N;
        pt[0] = 0;
        pt[1] = 0;
        for(unsigned int j=0;j<nbPts();++j) {
            controlPt = evalAnimPt(get(j),frame);
            coefBezier = Bezier(nbPts()-1, j, t);
            pt[0] += controlPt[0]*coefBezier;
            pt[1] += controlPt[1]*coefBezier;
        }
        p.lineTo(pt[0],pt[1]);
    }
    return p;
  }
};


class Curve2DBezierBasicConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DBezierBasicConstructor()                     {}
  virtual const QString type() const                      { return "BezierBasicCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DBezierBasic(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DBezierBasic(c,name); }
};


#endif // CURVE_2D_BezierBasic_H
