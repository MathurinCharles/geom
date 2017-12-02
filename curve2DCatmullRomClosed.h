#ifndef CURVE_2D_CATMULLROMCLOSED_H
#define CURVE_2D_CATMULLROMCLOSED_H

#include "curve2D.h"

Vector2f evalPolynom(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float t){
  return p0 + p1*t + p2*(t*t) + p3*(t*t*t);
}

class Curve2DCatmullRomClosed : public Curve2D {
 public:
 Curve2DCatmullRomClosed(const QString &name) : Curve2D(name) {}
 Curve2DCatmullRomClosed(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}

  QPainterPath path(float frame) {
    QPainterPath p;
    if(nbPts()==0)
      return p;

    Vector2f pt = evalAnimPt(get(0),frame);
    p.moveTo(pt[0],pt[1]);

    Vector2f pts[nbPts()];
    Vector2f tangts[nbPts()];

    for(unsigned int i=0;i<nbPts();++i) {
      pts[i]= evalAnimPt(get(i),frame);
    }
    for(unsigned int i=0;i<nbPts();++i) {
      tangts[i]= (pts[(i+1+nbPts())%nbPts()]-pts[(i-1+nbPts())%nbPts()])/
      (pts[(i+1+nbPts())%nbPts()][0]-pts[(i-1+nbPts())%nbPts()][0]);
    }

    Vector2f polynoms[nbPts()][4];
    for(unsigned int i=0;i<nbPts();++i) {
      polynoms[i][0] = pts[(i-1+nbPts())%nbPts()];
      polynoms[i][1] = tangts[(i-1+nbPts())%nbPts()];
      polynoms[i][2] = 3*(pts[(i+nbPts())%nbPts()] - pts[(i-1+nbPts())%nbPts()])
      - 2*tangts[(i-1+nbPts())%nbPts()] - tangts[(i+nbPts())%nbPts()];
      polynoms[i][3] = 2*(pts[(i-1+nbPts())%nbPts()] - pts[(i+nbPts())%nbPts()])
      + tangts[(i-1+nbPts())%nbPts()] + tangts[(i+nbPts())%nbPts()];
    }
    unsigned int N = 10;
    float t;
    for(unsigned int i=0;i<nbPts();++i) {
      for(unsigned int j=1;i<N;++i) {
        t = (float)j/(float)N;
        pt = evalPolynom(polynoms[i][0], polynoms[i][1], polynoms[i][2], polynoms[i][3], t);
        p.lineTo(pt[0], pt[1]);
      }
      p.lineTo(pts[i][0], pts[i][1]);
    }
    return p;
  }
};


class Curve2DCatmullRomClosedConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DCatmullRomClosedConstructor()                     {}
  virtual const QString type() const                      { return "CatmullRomClosedCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DCatmullRomClosed(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DCatmullRomClosed(c,name); }
};


#endif // CURVE_2D_CATMULLROMCLOSED_H
