#ifndef CURVE_1D_H
#define CURVE_1D_H

#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <vector>
#include <assert.h>
#include <QString>
#include <QPainterPath>
#include <QColor>

using namespace Eigen;

class Curve1D {
 public:
 Curve1D(const QString &name) : _name(name) {}
 Curve1D(Curve1D *curve,const QString &name)
   : _name(name) {
    assert(curve!=NULL);
    for(unsigned int i=0;i<curve->nbPts();++i) {
      add(Vector2f(curve->get(i)));
    }
  }

  virtual ~Curve1D() {
    clear();
  }

  inline virtual void add(const Vector2f &pt) {
    std::vector<Vector2f>::iterator it = _points.begin();
    
    while(it!=_points.end() && (*it)[0]<pt[0]) {
      it++;
    }
    
    if(it!=_points.end() && (*it)[0]==pt[0]) {
      // simply move the current keyframe
      (*it)[1] = pt[1];
    } else {
      // add a new control point
      _points.insert(it,pt);
    }
  }

  inline virtual void set(unsigned int i,const Vector2f &pt) {
    assert(i<nbPts());

    if((i>0 && pt[0]<_points[i-1][0]) || 
       (i<nbPts()-1 && pt[0]>_points[i+1][0]))
      return;
    
    _points[i] = pt;
  }
  
  inline virtual void del(unsigned int i) {
    assert(i<nbPts());
    _points.erase(_points.begin()+i);
  }

  inline virtual Vector2f get(unsigned int i) const {
    assert(i<nbPts());
    return _points[i];
  }
  
  inline virtual void clear() {
    _points.clear();
  }
  
  inline unsigned int nbPts() const {
    return static_cast<unsigned int>(_points.size());
  }

  inline bool empty() const {
    return nbPts()==0;
  }

  inline QString name() const {
    return _name;
  }

  inline void setName(const QString &name) {
    _name = name;
  }

  virtual QPainterPath path(float xmin,float xmax) = 0;
  virtual float evalAt(float x) = 0;
  
 protected:
  std::vector<Vector2f> _points;
  QString               _name;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class Curve1DConstructor {
 public:
  virtual ~Curve1DConstructor() {}
  virtual const QString type() const = 0;
  virtual Curve1D *create(const QString &) = 0;
  virtual Curve1D *create(Curve1D *,const QString &) = 0;
};

#endif // CURVE_1D_H
