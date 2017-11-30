#ifndef CURVE_1D_BEZIER_H
#define CURVE_1D_BEZIER_H

#include "curve1D.h"

class Curve1DBezier : public Curve1D {
 public:
 Curve1DBezier(const QString &name) : Curve1D(name) {}
 Curve1DBezier(Curve1D *curve,const QString &name) : Curve1D(curve,name) {}
  
  QPainterPath path(float xmin,float xmax) {
      //TODO
    QPainterPath p;

    // empty test 
    if(empty()) 
      return p;

    bool begin = false;
    bool end = false;
    Vector2f p_begin, p_end;

    // left part 
    if(xmin<_points[0][0]) {
      p.moveTo(xmin,_points[0][1]);
      begin = true;
      p_begin[0] = xmin;
      p_begin[1] =_points[0][1];
    } else {
      p.moveTo(_points[0][0],_points[0][1]);
    }

    // right part 
    if(xmax>_points[nbPts()-1][0]) {
        end = true;
        p_end[0] = xmax;
        p_end[1] =_points[nbPts()-1][1];

    }

    // draw function 
    int b = 0;
    int e = 0;
    if (begin) b++;
    if (end) e++;
    Vector2f controlPt;   
    unsigned int N = nbFrames();
    unsigned int Msize = _points.size()+e+b;
    float t;
    float AbsCasteljeau[Msize][Msize];
    float OrdCasteljeau[Msize][Msize];
    if (begin){
        AbsCasteljeau[0][0] = p_begin[0];
        OrdCasteljeau[0][0] = p_begin[1];
    }
    if (end){
        AbsCasteljeau[0][Msize-1] = p_end[0];
        OrdCasteljeau[0][Msize-1] = p_end[1];
    }
    for(unsigned int j=b;j<Msize-e;++j) {
        controlPt = _points[j];
        AbsCasteljeau[0][j] = controlPt[0];
        OrdCasteljeau[0][j] = controlPt[1];
    }
    for(unsigned int k=1;k<N+1;++k) {
        t = (float) k/(float) N;
        for(unsigned int i=1;i<Msize;++i){
            for(unsigned int j=0;j<Msize-i;++j) {
                AbsCasteljeau[i][j] = (1-t)*AbsCasteljeau[i-1][j] + t*AbsCasteljeau[i-1][j+1];
                OrdCasteljeau[i][j] = (1-t)*OrdCasteljeau[i-1][j] + t*OrdCasteljeau[i-1][j+1];
        }
    }
        p.lineTo(AbsCasteljeau[Msize-1][0],OrdCasteljeau[Msize-1][0]);
    }


    return p;
  }

  float evalAt(float x) {

    // special cases 
    if(empty()) return 0.0f;

    //Big mess
    int xmin = 0;
    int xmax = nbFrames()-1;
    bool begin = false;
    bool end = false;
    Vector2f p_begin, p_end;

    // left part 
    if(xmin<_points[0][0]) {
      begin = true;
      p_begin[0] = xmin;
      p_begin[1] =_points[0][1];
    }
    // right part 
    if(xmax>_points[nbPts()-1][0]) {
        end = true;
        p_end[0] = xmax;
        p_end[1] =_points[nbPts()-1][1];
    }
    // draw function 
    int b = 0;
    int e = 0;
    if (begin) b++;
    if (end) e++;
    Vector2f controlPt;   
    unsigned int N = nbFrames();
    unsigned int Msize = _points.size()+e+b;
    float t;
    float AbsCasteljeau[Msize][Msize];
    float OrdCasteljeau[Msize][Msize];
    if (begin){
        AbsCasteljeau[0][0] = p_begin[0];
        OrdCasteljeau[0][0] = p_begin[1];
    }
    if (end){
        AbsCasteljeau[0][Msize-1] = p_end[0];
        OrdCasteljeau[0][Msize-1] = p_end[1];
    }
    for(unsigned int j=b;j<Msize-e;++j) {
        controlPt = _points[j];
        AbsCasteljeau[0][j] = controlPt[0];
        OrdCasteljeau[0][j] = controlPt[1];
    }
    std::vector<Vector2f> framepoints(N+1);
    for(unsigned int k=0;k<N+1;++k) {
        t = (float) k/(float) N;
        for(unsigned int i=1;i<Msize;++i){
            for(unsigned int j=0;j<Msize-i;++j) {
                AbsCasteljeau[i][j] = (1-t)*AbsCasteljeau[i-1][j] + t*AbsCasteljeau[i-1][j+1];
                OrdCasteljeau[i][j] = (1-t)*OrdCasteljeau[i-1][j] + t*OrdCasteljeau[i-1][j+1];
            }
        }
        framepoints[k][0] = AbsCasteljeau[Msize-1][0];
        framepoints[k][1] = OrdCasteljeau[Msize-1][0];
    }
    // linear interp
    for(unsigned int i=0;i<N;++i) {
        if(framepoints[i+1][0]>=x) {
        return framepoints[i][1]+(framepoints[i+1][1]-framepoints[i][1])*
        ((x-framepoints[i][0])/(framepoints[i+1][0]-framepoints[i][0]));
        }
    }
    return framepoints[0][1];
  }

  unsigned int nbFrames();
};


class Curve1DBezierConstructor : public Curve1DConstructor {
 public:
  virtual ~Curve1DBezierConstructor()                     {}
  virtual const QString type() const                      { return "BezierCurve";             }
  virtual Curve1D *create(const QString &name)            { return new Curve1DBezier(name);   }
  virtual Curve1D *create(Curve1D *c,const QString &name) { return new Curve1DBezier(c,name); }
};


#endif // CURVE_1D_BEZIER_H
