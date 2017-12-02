#ifndef CURVE_2D_LINEARCLOSED_H
#define CURVE_2D_LINEARCLOSED_H

#include "curve2D.h"

class Curve2DLinearClosed : public Curve2D {
 public:
 Curve2DLinearClosed(const QString &name) : Curve2D(name) {}
 Curve2DLinearClosed(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  QPainterPath path(float frame) {
    QPainterPath p;
    if(nbPts()==0) 
      return p;

    Vector2f pt = evalAnimPt(get(0),frame);
    Vector2f ptf = pt;

    p.moveTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      pt = evalAnimPt(get(i),frame);
      p.lineTo(pt[0],pt[1]);
    }
    p.lineTo(ptf[0],ptf[1]);
    return p;
  }
};


class Curve2DLinearClosedConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DLinearClosedConstructor()                     {}
  virtual const QString type() const                      { return "LinearClosedCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DLinearClosed(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DLinearClosed(c,name); }
};


#endif // CURVE_2D_LINEARCLOSED_H
