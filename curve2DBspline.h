#ifndef CURVE_2D_BSPLINE_H
#define CURVE_2D_BSPLINE_H

#include "curve2D.h"

float tau(unsigned int j,unsigned int i,float t,unsigned int K,float ti[]){
  return (t-ti[i])/(ti[i+K-j]-ti[i]);
}

Vector2f Pjit(unsigned int j,unsigned int i,float t, Vector2f CP[],unsigned int K,float ti[]){
  if (j==0) return CP[i];
  return (1-tau(j,i,t,K,ti)) * Pjit(j-1, i-1, t, CP, K, ti) + tau(j,i,t,K,ti) * Pjit(j-1, i, t, CP, K, ti);
}


class Curve2DBspline : public Curve2D {
 public:
 Curve2DBspline(const QString &name) : Curve2D(name) {}
 Curve2DBspline(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  

  QPainterPath path(float frame) {
    QPainterPath p;
    unsigned int N = 10*nbPts();

    if(nbPts()==0) return p;

    unsigned int k = 3;
    Vector2f CP[nbPts()+1];
    float knots[nbPts()+k];
    for (unsigned int p=0;p<k-1;++p){
      knots[p] = 0;
      knots[nbPts()+k-1+p] = nbPts()- 1;
    }
    for (unsigned int i=0;i<nbPts();++i){
      knots[i+k-1] = i;
    }

    Vector2f pt = evalAnimPt(get(0),frame);
    p.moveTo(pt[0],pt[1]);
    CP[0] = pt;

    for (unsigned int i=1;i<nbPts();++i){
      CP[i] = evalAnimPt(get(i),frame);

    }
    CP[nbPts()] = CP[nbPts()-1];

    

    unsigned int l = 0;
    for (float t = 0.001;t<knots[nbPts()+k-1];t+=knots[nbPts()+k-1]/N){
      while (l<nbPts()+k-1 && !(t>=knots[l] && t<knots[l+1])) l++;
      pt =  Pjit(k-1, l, t, CP, k, knots);
      p.lineTo(pt[0],pt[1]);
    }
    return p;
  }
};


class Curve2DBsplineConstructor : public Curve2DConstructor {
 public:
  virtual ~Curve2DBsplineConstructor()                     {}
  virtual const QString type() const                      { return "BsplineCurve";             }
  virtual Curve2D *create(const QString &name)            { return new Curve2DBspline(name);   }
  virtual Curve2D *create(Curve2D *c,const QString &name) { return new Curve2DBspline(c,name); }
};


#endif // CURVE_2D_BSPLINE_H
