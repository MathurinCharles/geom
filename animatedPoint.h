#ifndef ANIMATED_POINT_H
#define ANIMATED_POINT_H

#include <Eigen/Dense>

#include "curve1D.h"

class AnimatedPoint {
 public:
  AnimatedPoint(AnimatedPoint *apt);
  AnimatedPoint(const Vector2f &pt);
  AnimatedPoint(float x,float y);
  ~AnimatedPoint();

  inline Curve1D *getXCurve() { return _xcurve; }
  inline Curve1D *getYCurve() { return _ycurve; }
  
  inline Vector2f evalAt(float t) {
    return Vector2f(_xcurve->evalAt(t),
		    _ycurve->evalAt(t));
  }


  void setType(const QString &type);
  void setAt(float t,const Vector2f &pt);
  void setCurrent(const Vector2f &pt);

  void delAt(float t);
  void clean();

 private:
  Curve1D *_xcurve;
  Curve1D *_ycurve;
};

#endif // ANIMATED_POINT_H
