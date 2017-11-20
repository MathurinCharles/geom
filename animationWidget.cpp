#include "animationWidget.h"
#include "mainWidget.h"
#include "scene.h"
#include "animationWidgetItems.h"
#include <iostream>
#include <assert.h>
#include <QTransform>

using namespace std;

AnimationWidget::AnimationWidget(MainWindow *mainWindow,QWidget *parent)
  : QGraphicsView(parent),
    _mainWindow(mainWindow) {
  Scene *sce = Scene::get();
  _gscene = new QGraphicsScene(this);
  _gscene->setItemIndexMethod(QGraphicsScene::NoIndex);
  setScene(_gscene);
  setCacheMode(CacheBackground);
  setViewportUpdateMode(FullViewportUpdate);
  setRenderHint(QPainter::Antialiasing);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  resetResol();
}

AnimationWidget::~AnimationWidget() {
  
}


void AnimationWidget::drawForeground(QPainter * painter, const QRectF & rect) {
  Scene *sce = Scene::get();
  painter->setWorldMatrixEnabled(false);

  // draw viewport
  QPen p(Qt::black,1);
  painter->setPen(p);
  QPointF p1 = mapFromScene(QPoint(0,0));
  QPointF p2 = mapFromScene(QPoint(sce->nbFrames()-1,max(sce->width(),sce->height())));
  painter->drawRect(QRectF(p1,p2));

  // draw frame 
  p.setWidth(2);
  p.setColor(Qt::red);
  painter->setPen(p);
  p1 = mapFromScene(QPoint(sce->currentFrame(),0));
  p2 = mapFromScene(QPoint(sce->currentFrame(),max(sce->width(),sce->height())));
  painter->drawLine(p1,p2);

  painter->setWorldMatrixEnabled(true);
}


void AnimationWidget::selectionChanged() {
  Scene *sce = Scene::get();

  clean();
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT: 
    addAnimatedPoint(sce->selectedCurve(),sce->selectedPoint());
    break;
  case Scene::KF_MODE_CURVE: 
    addAnimatedCurve(sce->selectedCurve());
    break;
  case Scene::KF_MODE_SCENE: 
    addAnimatedScene();
    break;
  default: break;
  }
}

void AnimationWidget::resetResol() {
  Scene *sce = Scene::get();
  resetMatrix();
  setSceneRect(0,0,sce->nbFrames()-1,max(sce->width(),sce->height()));

  QPointF p1s = QPointF(0,0);
  QPointF p2s = QPointF(sce->nbFrames()-1,max(sce->width(),sce->height()));

  QPointF p1v = mapToScene(QPoint(0,0));
  QPointF p2v = mapToScene(QPoint(width(),height()));

  float s1 = p2s.x()-p1s.x();
  float s2 = p2v.x()-p1v.x();  
  float s3 = p2s.y()-p1s.y();
  float s4 = p2v.y()-p1v.y();

  scale(s2/s1,s4/s3);
}

void AnimationWidget::clean() {
  for(unsigned int i=0;i<_animPts.size();++i) {
    scene()->removeItem(_animPts[i]);
    delete _animPts[i];
  }
  _animPts.clear();
}

void AnimationWidget::addAnimatedPoint(int curveInd,int ptInd) {
  Scene *sce = Scene::get();

  if(curveInd<0 || ptInd<0) 
    return;
  
  assert(curveInd<(int)sce->nbCurves());
  assert(ptInd<(int)(sce->getCurve(curveInd)->nbPts()));

  AnimatedPointDrawer *apt = new AnimatedPointDrawer(this,curveInd,ptInd);
  
  _animPts.push_back(apt);
  _gscene->addItem(apt);
}

void AnimationWidget::addAnimatedCurve(int curveInd) {
  Scene *sce = Scene::get();

  if(curveInd<0) 
    return;
  
  assert(curveInd<(int)sce->nbCurves());
  Curve2D *c = sce->getCurve(curveInd);
  for(unsigned int i=0;i<c->nbPts();++i) {
    addAnimatedPoint(curveInd,i);
  }
}

void AnimationWidget::addAnimatedScene() {
  Scene *sce = Scene::get();

  for(unsigned int i=0;i<sce->nbCurves();++i) {
    addAnimatedCurve(i);
  }
}

void AnimationWidget::wheelEvent(QWheelEvent * event) {
 const double s = 1000.0;
 
 double f1 = pow(2.0,-event->delta()/s);
 double f2 = f1;

 if(event->modifiers() & Qt::ControlModifier) {
   f1 = 1.0;
 } else if(event->modifiers() & Qt::ShiftModifier) {
   f2 = 1.0;
 } 

 scale(f1,f2);
}
