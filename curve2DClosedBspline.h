#ifndef CURVE_2D_ClosedBspline_H
#define CURVE_2D_ClosedBspline_H

#include "curve2D.h"
#include "curve2DBspline.h"



/* float omega(unsigned int i,unsigned int k,float t,float tiTable[],unsigned int n){
  if (tiTable[i]<tiTable[i+k]) return (t-tiTable[i])/(tiTable[i+k]-tiTable[i]);
  else return 0;
} */

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
    //std::cout << "KNOTS :" << std::endl;
    for (unsigned int p=0;p<k-1;++p){
      knots[p] = 0;
      knots[(nbPts()+k)+k-1+p] = (nbPts()+k)- 1;
    }
    for (unsigned int i=0;i<(nbPts()+k);++i){
      knots[i+k-1] = i;
    }
    for (unsigned int p=0;p<(nbPts()+k)+k;++p){
      //std::cout << "knots[" << p << "]:" << knots[p] << ",";
    }
    //std::cout << std::endl << "_________________________________" << std::endl;
    Vector2f pt = evalAnimPt(get(0),frame);
    CP[0] = pt;
    //std::cout << "CP :" << std::endl;
    for (unsigned int i=1;i<nbPts();++i){
      CP[i] = evalAnimPt(get(i),frame);
      if (i<k) CP[i+nbPts()] = CP[i];
      //std::cout << CP[i] << std::endl;
    }
    CP[(nbPts()+k)] = CP[(nbPts()+k)-1];
    //std::cout << std::endl << "______-------------------________" << std::endl;
    

    unsigned int l = 1;
    for (float t = knots[k]+0.001;t<knots[nbPts()+2*k-3];t+=knots[(nbPts()+k)+k-1]/N){
      while (l<(nbPts()+k)+k-1 && !(t>=knots[l] && t<knots[l+1])) l++;
      ////std::cout << "l = " << l << " and t = " << t << std::endl;
      ////std::cout << "Limite: " << nbPts()+k-1 << std::endl;
      ////std::cout << "________________________Limite: " << nbPts()-1 << std::endl;
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
