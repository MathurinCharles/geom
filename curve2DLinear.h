#ifndef CURVE_2D_LINEAR_H
#define CURVE_2D_LINEAR_H

#include "curve2D.h"

class Curve2DLinear : public Curve2D {
 public:
 Curve2DLinear(const QString &name) : Curve2D(name) {}
 Curve2DLinear(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  QPainterPath path(float frame) {
    QPainterPath p;
    if(nbPts()==0) 
      return p;

    Vector2f pt = evalAnimPt(get(0),frame);

    p.moveTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      pt = evalAnimPt(get(i),frame);
      p.lineTo(pt[0],pt[1]);
    }
    return p;
  }
};


class Curve2DLinearConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DLinearConstructor()                     {}
  virtual const QString type() const                      { return "LinearCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DLinear(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DLinear(c,name); }
};


#endif // CURVE_2D_LINEAR_H
