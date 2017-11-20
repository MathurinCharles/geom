#include "animationWidgetItems.h"

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <Eigen/Dense>
#include <iostream>
#include "scene.h"
#include "animatedPoint.h"

using namespace std;

void AnimatedPointDrawer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  Scene *sce = Scene::get();
  AnimatedPoint *apt = sce->getCurve(_curveInd)->get(_ptInd);
  Curve1D *cx = apt->getXCurve();
  Curve1D *cy = apt->getYCurve();
  bool s = sce->selectedCurve()==_curveInd && sce->selectedPoint()==_ptInd;
  QPen p(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
  p.setCosmetic(true);

  QPainterPath ptsx = cx->path(0.0f,(float)sce->nbFrames()-1.0f);
  QPainterPath ptsy = cy->path(0.0f,(float)sce->nbFrames()-1.0f);

  // draw curves 
  p.setColor(Qt::blue);

  if(s) {
    p.setColor(p.color().darker());
  }

  painter->setPen(p);
  painter->drawPath(ptsx);

  p.setColor(Qt::magenta);
  
  if(s) {
    p.setColor(p.color().darker());
  }

  painter->setPen(p);
  painter->drawPath(ptsy);

  // draw control points 
  p.setWidth(_size);
  p.setColor(Qt::green);
  painter->setPen(p);
  for(unsigned int i=0;i<cx->nbPts();++i) {
    Vector2f pos = cx->get(i);
    painter->drawPoint(pos[0],pos[1]);
  }

  for(unsigned int i=0;i<cy->nbPts();++i) {
    Vector2f pos = cy->get(i);
    painter->drawPoint(pos[0],pos[1]);
  }
}

QRectF AnimatedPointDrawer::boundingRect() const {
  Scene *sce = Scene::get();
  return QRectF(0,0,sce->nbFrames()-1,max(sce->width(),sce->height()));
}
