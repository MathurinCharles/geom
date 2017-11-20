#ifndef DRAWING_WIDGET_H
#define DRAWING_WIDGET_H

#include <QGraphicsView>
#include <QTime>
#include <vector>
#include "curve2D.h"

class MainWindow;
class ControlPoint2D;
class Curve2DDrawer;

class DrawingWidget : public QGraphicsView {
  Q_OBJECT

    public:
  DrawingWidget(MainWindow *mainWindow,QWidget *parent=NULL);
  ~DrawingWidget();

  inline void refresh() {
    scene()->update(0,0,width(),height());
  }

  void refreshAnimation();

  void frameChanged();
  void setResol(int w,int h);
  void selectionChanged();

 protected:
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent * event);
  virtual void drawBackground(QPainter * painter, const QRectF & rect);

 private:
  MainWindow *_mainWindow;
  QGraphicsScene *_gscene;

  void addCurve(Curve2D *curve);
  void deleteSelectedCurve();
  void addPointToSelectedCurve(float x,float y);
  void addPointBeforeSelectedPoint(float x,float y);
  void addPointAfterSelectedPoint(float x,float y);
  void deleteSelectedPoint();
  
  std::vector<std::vector<ControlPoint2D *> > _points;
  std::vector<Curve2DDrawer *>                _curves;
};

#endif // DRAWING_WIDGET_H
