#include "drawingWidgetItems.h"

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <Eigen/Dense>
#include <iostream>
#include "scene.h"
#include "curve2D.h"

using namespace std;

void Curve2DDrawer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  Scene *sce = Scene::get();
  Curve2D *curve = sce->getCurve(_curveInd);


  // draw lines between control points
  QBrush b(curve->brushColor());
  QPen p(Qt::black,1,Qt::DashLine,Qt::RoundCap,Qt::RoundJoin);
  QPointF points[curve->nbPts()];
  painter->setPen(p);
  for(unsigned int i=0;i<curve->nbPts();++i) {
    Vector2f pt = curve->get(i)->evalAt((float)sce->currentFrame());
    points[i].setX(pt[0]);
    points[i].setY(pt[1]);
  }
  painter->drawPolyline(points,curve->nbPts());


  p.setColor(curve->penColor());
  p.setWidth(curve->penWidth());
  p.setStyle(Qt::SolidLine);
  //QPen p(curve->penColor(),curve->penWidth(),Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

  
  QPainterPath pts = curve->path((float)(sce->currentFrame()));
  painter->setPen(p);
  painter->drawPath(pts);
  painter->fillPath(pts,b);


}

QRectF Curve2DDrawer::boundingRect() const {
  Scene *sce = Scene::get();
  return QRectF(0,0,sce->width(),sce->height());
}

ControlPoint2D::ControlPoint2D(DrawingWidget *drawingWidget,unsigned int curveInd,unsigned int ptInd) 
   : _drawingWidget(drawingWidget),_curveInd(curveInd),_ptInd(ptInd) {
  Scene *sce = Scene::get();

  setFlags(ItemIsMovable | ItemIgnoresTransformations | ItemIsSelectable);
  setZValue(1);

  Curve2D *c = sce->getCurve(_curveInd);
  Vector2f pt = c->evalAnimPt(c->get(_ptInd),(float)(sce->currentFrame()));
  setPos(pt[0],pt[1]);
}

void ControlPoint2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  Scene *sce = Scene::get();

  QPen p(Qt::black,_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  // test selection
  bool curveSelected = sce->isCurveSelected(_curveInd);
  bool pointSelected = sce->isPointSelected(_curveInd,_ptInd);
  
  if(curveSelected) {
    p.setWidth(_size+_size/3);
  }
  
  if(pointSelected) {
    p.setWidth(_size+_size/2);
  }

  painter->setPen(p);
  painter->drawPoint(0,0);
  p.setWidth(_size-2);
  
  if(pointSelected)
    p.setColor(Qt::red);
  else if(curveSelected)
    p.setColor(Qt::yellow);
  else
    p.setColor(Qt::blue);

  painter->setPen(p);
  painter->drawPoint(0,0);
}



QRectF ControlPoint2D::boundingRect() const {
  qreal s= _size/2;
  return QRectF(-s,-s,_size,_size);
}

void ControlPoint2D::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);

  Scene *sce = Scene::get();

  if(sce->currentTool()==Scene::EDIT_CURVE && !sce->isAnimated() &&
     (event->buttons() & Qt::LeftButton)) {
    sce->setSelectedCurve(_curveInd);
    sce->setSelectedPoint(_ptInd);
    setSelected(true);
    _drawingWidget->selectionChanged();
    _drawingWidget->refresh();
    _drawingWidget->refreshAnimation();


  } else {
    setFlag(QGraphicsItem::ItemIsMovable,false);
  }

}

void ControlPoint2D::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {
  QGraphicsItem::mouseMoveEvent(event);
 
  Scene *sce = Scene::get();

  if(sce->currentTool()==Scene::EDIT_CURVE && !sce->isAnimated() &&
     (event->buttons() & Qt::LeftButton)) {
    QPointF p = event->scenePos();
    sce->getCurve(_curveInd)->set(_ptInd,p.x(),p.y());
    _drawingWidget->refresh();
    _drawingWidget->refreshAnimation();
  }

}

void ControlPoint2D::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  setSelected(false);
  setFlag(QGraphicsItem::ItemIsMovable,true);
  QGraphicsItem::mouseReleaseEvent(event);
}
