#ifndef CURVE_2D_Bspline_H
#define CURVE_2D_Bspline_H

#include "curve2D.h"

float omega(unsigned int i,unsigned int k,float t,float tiTable[],unsigned int n){
    assert(i+k<n);
    if (tiTable[i]<tiTable[i+k]) return (t-tiTable[i])/(tiTable[i+k]-tiTable[i]);
    else return 0;
}

class Curve2DBspline : public Curve2D {
 public:
 Curve2DBspline(const QString &name) : Curve2D(name) {}
 Curve2DBspline(Curve2D *curve,const QString &name) : Curve2D(curve,name) {}
  
  

  QPainterPath path(float frame) {
    QPainterPath p;
    unsigned int N = 10*nbPts();
    float t;
    unsigned int K = 1;
    float tiTable[nbPts()];
    Vector2f DeboorCox[K+1][K+1];

    if(nbPts()==0) 
      return p;
    
    for (unsigned int i=0;i<nbPts();++i){
      tiTable[i] = i;
    }

    unsigned int i = K;
    for (unsigned int k=1; k<N+1;++k){
      t = (float) k*tiTable[nbPts()-1] / (float) N;
      while (t<tiTable[i] && i<nbPts()) i++;
      for (unsigned int j=i-K;j<i+1;++j){
        DeboorCox[0][j-i+K] = evalAnimPt(get(j),frame);
      }
      for (unsigned int l=1; l<K+1; ++l){
        for (unsigned int c=0;c<K+1-l;++c){
          DeboorCox[l][c] = omega(c+i-K,K-l+1,t,tiTable,nbPts())*DeboorCox[l-1][c] + (1-omega(c+i-K,K-l+1,t,tiTable,nbPts()))*DeboorCox[l-1][c+1] ;
        }
      }
      p.lineTo(DeboorCox[K][0][0],DeboorCox[K][0][1]);
    }






    Vector2f pt = evalAnimPt(get(0),frame);

    p.moveTo(pt[0],pt[1]);
    for(unsigned int i=1;i<nbPts();++i) {
      pt = evalAnimPt(get(i),frame);
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


#endif // CURVE_2D_Bspline_H
