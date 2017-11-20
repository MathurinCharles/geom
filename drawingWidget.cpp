#include "drawingWidget.h"
#include "mainWidget.h"
#include "scene.h"
#include "drawingWidgetItems.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

using namespace std;

DrawingWidget::DrawingWidget(MainWindow *mainWindow,QWidget *parent)
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
  setResol(sce->width(),sce->height());
}

void DrawingWidget::setResol(int w,int h) {
  setSceneRect(0,0,w,h);
}

DrawingWidget::~DrawingWidget() {
  
}

void DrawingWidget::drawBackground(QPainter * painter, const QRectF & rect) {
  Scene *sce = Scene::get();
  painter->setWorldMatrixEnabled(false);

  QPen p(Qt::black,1);
  painter->setPen(p);
  QPointF p1 = mapFromScene(QPoint(0,0));
  QPointF p2 = mapFromScene(QPoint(sce->width(),sce->height()));
  painter->drawRect(QRectF(p1,p2));

  painter->setWorldMatrixEnabled(true);
}

void DrawingWidget::wheelEvent(QWheelEvent * event) {
 const double s = 1000.0;
 const double f = pow(2.0,-event->delta()/s);
 scale(f,f);
}

void DrawingWidget::keyPressEvent(QKeyEvent *event) {
  QGraphicsView::keyPressEvent(event);
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
  QGraphicsView::mouseMoveEvent(event);
  Scene *sce = Scene::get();

  if(sce->isAnimated()) {
    return;
  }

  QPointF pos = mapToScene(QPoint(event->x(),event->y()));

  // the create mode 
  if(sce->currentTool()==Scene::CREATE_CURVE) {
    int curveInd = sce->selectedCurve();
    if(curveInd==-1)
      return;
    int ptInd = sce->selectedPoint();
    if(ptInd==-1)
      return;
    sce->getCurve(curveInd)->set(ptInd,pos.x(),pos.y());
    _points[curveInd][ptInd]->setPos(pos);
  }
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);

  
  Scene *sce = Scene::get();
  if(sce->isAnimated()) {
    return;
  }

  QPointF pos = mapToScene(QPoint(event->x(),event->y()));

  // CREATE MODE
  if(sce->currentTool()==Scene::CREATE_CURVE) {
    if(event->buttons() & Qt::LeftButton) {

      Curve2D *curve;
      if(sce->selectedCurve()==-1) {
	// need to create a new curve 
	curve = sce->createCurve(sce->currentCurveType());
	addCurve(curve);
	
      } else {
	curve = sce->getCurve(sce->selectedCurve());
      }
      
      // add a point to this curve 
      addPointToSelectedCurve(pos.x(),pos.y());
    } else if(event->buttons() & Qt::RightButton) {
      // end of curve 
      if(sce->selectedCurve()>=0) {
	sce->setSelectedCurve(-1);
	sce->cleanSelectedPoints();
	selectionChanged();
      }
    }

    // EDIT MODE 
  } else if(sce->currentTool()==Scene::EDIT_CURVE) {
    if(sce->selectedCurve()>=0 && sce->selectedPoint()>=0) {
      ControlPoint2D *cc = NULL;
      switch(sce->editMode()) {
      case Scene::EDIT_ADD_BEFORE:
	cc = _points[sce->selectedCurve()][sce->selectedPoint()];
	if(!(items(event->pos()).contains(cc))) {
	  addPointBeforeSelectedPoint(pos.x(),pos.y());
	}
	break;
      case Scene::EDIT_ADD_AFTER:
	cc = _points[sce->selectedCurve()][sce->selectedPoint()];
	if(!(items(event->pos()).contains(cc))) {
	  addPointAfterSelectedPoint(pos.x(),pos.y());
	}
	break;
      case Scene::EDIT_DEL_PT:
	deleteSelectedPoint();
	break;
      default: break;
      }
    }
  }
  
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *event) {


  QGraphicsView::mouseReleaseEvent(event);
  Scene *sce = Scene::get();
  
  if(sce->isAnimated()) {
    return;
  }

  refresh();
}


void DrawingWidget::addCurve(Curve2D *curve) {
  // add curve to the scene 
  Scene *sce = Scene::get();
  sce->addCurve(curve);
  sce->setSelectedCurve(sce->nbCurves()-1);
  
  // add graphics items 
  _curves.push_back(new Curve2DDrawer(this,sce->nbCurves()-1));
  _gscene->addItem(_curves[_curves.size()-1]);
  _points.push_back(vector<ControlPoint2D *>());
  for(unsigned int i=0;i<curve->nbPts();++i) {
    ControlPoint2D *cc = new ControlPoint2D(this,sce->nbCurves()-1,i);
    _points[_points.size()-1].push_back(cc);
    _gscene->addItem(cc);
  }
  selectionChanged();
}

void DrawingWidget::addPointToSelectedCurve(float x,float y) {
  Scene *sce = Scene::get();
  int curveInd = sce->selectedCurve();

  if(curveInd<0) return; // no curve selected

  // add pt to scene
  Curve2D *curve = sce->getCurve(curveInd);
  curve->add(x,y);
  sce->setSelectedPoint(curve->nbPts()-1);
  
  // add graphics items
  ControlPoint2D *cc = new ControlPoint2D(this,curveInd,curve->nbPts()-1);
  _points[curveInd].push_back(cc);
  _gscene->addItem(cc);
  selectionChanged();
}

void DrawingWidget::deleteSelectedCurve() {
  Scene *sce = Scene::get();
  int curveInd = sce->selectedCurve();
  if(curveInd<0) return;

  // remove from scene
  sce->delCurve(curveInd);

  // update all indices
  for(unsigned int i=curveInd+1;i<_curves.size();++i) {
    _curves[i]->setCurveInd(i-1);
    for(unsigned int j=0;j<_points[i].size();++j) {
      _points[i][j]->setCurveInd(i-1);
    }
  }

  // remove elements from graphics scene
  scene()->removeItem(_curves[curveInd]);
  delete _curves[curveInd];
  _curves.erase(_curves.begin()+curveInd);
  for(unsigned int i=0;i<_points[curveInd].size();++i) {
    scene()->removeItem(_points[curveInd][i]);
    delete _points[curveInd][i];
  }
  _points[curveInd].clear();
  _points.erase(_points.begin()+curveInd);
  
  sce->setSelectedCurve(-1);
  sce->cleanSelectedPoints();
  selectionChanged();
}

void DrawingWidget::addPointBeforeSelectedPoint(float x,float y) {
  Scene *sce = Scene::get();
  int curveInd = sce->selectedCurve();
  int ptInd = sce->selectedPoint();

  if(curveInd<0 || ptInd<0) return; 

  // add pt to the curve 
  Curve2D *curve = sce->getCurve(curveInd);
  curve->addBefore(ptInd,x,y);

  // create graphics item
  ControlPoint2D *cc = new ControlPoint2D(this,curveInd,ptInd);
  _points[curveInd].insert(_points[curveInd].begin()+ptInd,cc);  
  _gscene->addItem(cc);

  // update pt indices
  for(unsigned int i=ptInd+1;i<_points[curveInd].size();++i) {
    _points[curveInd][i]->setPtInd(i);
  }

  // update current selected point
  sce->setSelectedPoint(ptInd+1);
  selectionChanged();
}

void DrawingWidget::addPointAfterSelectedPoint(float x,float y) {
  Scene *sce = Scene::get();
  int curveInd = sce->selectedCurve();
  int ptInd = sce->selectedPoint();

  if(curveInd<0 || ptInd<0) return;

  // add pt to the curve 
  Curve2D *curve = sce->getCurve(curveInd);
  curve->addAfter(ptInd,x,y);

  // create graphics item
  ControlPoint2D *cc = new ControlPoint2D(this,curveInd,ptInd+1);
  _points[curveInd].insert(_points[curveInd].begin()+ptInd+1,cc);  
  _gscene->addItem(cc);

  // update pt indices
  for(unsigned int i=ptInd+2;i<_points[curveInd].size();++i) {
    _points[curveInd][i]->setPtInd(i);
  }
  selectionChanged();
}

void DrawingWidget::deleteSelectedPoint() {
  Scene *sce = Scene::get();
  int curveInd = sce->selectedCurve();
  int ptInd = sce->selectedPoint();

  if(curveInd<0 || ptInd<0) return;

  Curve2D *curve = sce->getCurve(curveInd);
  curve->del(ptInd);
  ControlPoint2D *cc = _points[curveInd][ptInd];
  _points[curveInd].erase(_points[curveInd].begin()+ptInd);
  scene()->removeItem(cc);
  delete cc;

  // update pt indices
  for(unsigned int i=ptInd;i<_points[curveInd].size();++i) {
    _points[curveInd][i]->setPtInd(i);
  }
  
  sce->cleanSelectedPoints();

  if(curve->nbPts()==0) {
    deleteSelectedCurve();
  }
  selectionChanged();
}

void DrawingWidget::frameChanged() {
  Scene *sce = Scene::get();
  for(unsigned int i=0;i<sce->nbCurves();++i) {
    Curve2D *c = sce->getCurve(i);
    for(unsigned int j=0;j<c->nbPts();++j) {
      Vector2f pt = c->evalAnimPt(c->get(j),(float)sce->currentFrame());
      _points[i][j]->updatePos(QPointF(pt[0],pt[1]));
    }
  }
}

void DrawingWidget::selectionChanged() {
  _mainWindow->selectionChanged();
}

void DrawingWidget::refreshAnimation() {
  _mainWindow->refreshAnimation();
}
