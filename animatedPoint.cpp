#include <QStringList>
#include "animatedPoint.h"
#include "scene.h"
#include "curve1D.h"

AnimatedPoint::AnimatedPoint(AnimatedPoint *pt) {
  Scene *sce = Scene::get();

  _xcurve = sce->createFunction(pt->getXCurve()->name(),pt->getXCurve());
  _ycurve = sce->createFunction(pt->getYCurve()->name(),pt->getYCurve());
}

AnimatedPoint::AnimatedPoint(const Vector2f &pt) {
  Scene *sce = Scene::get();
  QStringList l = sce->functionTypes();
  assert(l.size()>0);
  float t = (float)sce->currentFrame();
  _xcurve = sce->createFunction(l[0]);
  _ycurve = sce->createFunction(l[0]);
  _xcurve->add(Vector2f(t,pt[0]));
  _ycurve->add(Vector2f(t,pt[1]));
}

AnimatedPoint::AnimatedPoint(float x,float y) {
  Scene *sce = Scene::get();
  QStringList l = sce->functionTypes();
  assert(l.size()>0);
  float t = (float)sce->currentFrame();
  _xcurve = sce->createFunction(l[0]);
  _ycurve = sce->createFunction(l[0]);
  _xcurve->add(Vector2f(t,x));
  _ycurve->add(Vector2f(t,y));
}

AnimatedPoint::~AnimatedPoint() {
  delete _xcurve;
  delete _ycurve;
}

void AnimatedPoint::setAt(float t,const Vector2f &pt) {
  // the point will be added only if there is not already an existing t value
  // otherwise, it will simply be modified
  _xcurve->add(Vector2f(t,pt[0]));
  _ycurve->add(Vector2f(t,pt[1]));
}

void AnimatedPoint::setCurrent(const Vector2f &pt) {
  Scene *sce = Scene::get();
  setAt((float)sce->currentFrame(),pt);
}


void AnimatedPoint::clean() {
  // keep only one keyframe
  Vector2f kfx = _xcurve->get(0);
  Vector2f kfy = _ycurve->get(0);
  kfx[0] = 0.0f;
  kfy[0] = 0.0f;
  _xcurve->clear();
  _ycurve->clear();
  _xcurve->add(kfx);
  _ycurve->add(kfy);
}

void AnimatedPoint::delAt(float t) {
  // delete all control points at frame=t
  for(unsigned int i=0;i<_xcurve->nbPts();++i) {
    Vector2f p = _xcurve->get(i);
    if(p[0]==t) {
      _xcurve->del(i);
      break;
    }
  }
  
  for(unsigned int i=0;i<_ycurve->nbPts();++i) {
    Vector2f p = _ycurve->get(i);
    if(p[0]==t) {
      _ycurve->del(i);
      break;
    }
  }
}

void AnimatedPoint::setType(const QString &type) {
  Scene *sce = Scene::get();
  Curve1D *cx = getXCurve();
  Curve1D *cy = getYCurve();
  _xcurve = sce->createFunction(type,cx);
  _ycurve = sce->createFunction(type,cy);
  delete cx;
  delete cy;
}
