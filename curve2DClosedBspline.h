#ifndef CURVE_2D_CLOSEDBSPLINE_H
#define CURVE_2D_CLOSEDBSPLINE_H

#include "curve2D.h"
#include "curve2DBspline.h"

class Curve2DClosedBspline : public Curve2D {
 public:
 Curve2DClosedBspline(const QString &name) : Curve2D(name) {}
 Curve2DClosedBspline(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  

  QPainterPath path(float frame) {
    QPainterPath p;
    unsigned int k = 3;
    unsigned int N = 10*(nbPts()+k);
    bool firstpoint = true;

    if(nbPts()==0) return p;

    
    Vector2f CP[(nbPts()+k)+1];
    float knots[(nbPts()+k)+k];
    for (unsigned int p=0;p<k-1;++p){
      knots[p] = 0;
      knots[(nbPts()+k)+k-1+p] = (nbPts()+k)- 1;
    }
    for (unsigned int i=0;i<(nbPts()+k);++i){
      knots[i+k-1] = i;
    }

    Vector2f pt = evalAnimPt(get(0),frame);
    CP[0] = pt;

    for (unsigned int i=1;i<nbPts();++i){
      CP[i] = evalAnimPt(get(i),frame);
      if (i<k) CP[i+nbPts()] = CP[i];

    }
    CP[(nbPts()+k)] = CP[(nbPts()+k)-1];

    

    unsigned int l = 1;
    for (float t = knots[k]+0.001;t<knots[nbPts()+2*k-3];t+=knots[(nbPts()+k)+k-1]/N){
      while (l<(nbPts()+k)+k-1 && !(t>=knots[l] && t<knots[l+1])) l++;
      pt =  Pjit(k-1, l, t, CP, k, knots);
      if (firstpoint){
        p.moveTo(pt[0],pt[1]);
        firstpoint = false;
      }
      p.lineTo(pt[0],pt[1]);
    }
    return p;
  }
};


class Curve2DClosedBsplineConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DClosedBsplineConstructor()                     {}
  virtual const QString type() const                      { return "ClosedBsplineCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DClosedBspline(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DClosedBspline(c,name); }
};


#endif // CURVE_2D_ClosedBspline_H
