#ifndef DRAWING_WIDGET_ITEMS_H
#define DRAWING_WIDGET_ITEMS_H

#include <QGraphicsItem>
#include "drawingWidget.h"

class Curve2DDrawer : public QGraphicsItem {
 public:
 Curve2DDrawer(DrawingWidget *drawingWidget,unsigned int curveInd) 
   : _drawingWidget(drawingWidget),_curveInd(curveInd) {}

  inline unsigned int curveInd() const {return _curveInd;}
  inline void setCurveInd(unsigned int ind) {_curveInd = ind;}

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  
 private:
  DrawingWidget *_drawingWidget;
  unsigned int _curveInd;
};

class ControlPoint2D : public QGraphicsItem {
 public:
  ControlPoint2D(DrawingWidget *drawingWidget,unsigned int curveInd,unsigned int ptInd);

  inline unsigned int curveInd() const {return _curveInd;}
  inline void setCurveInd(unsigned int ind) {_curveInd = ind;}

  inline unsigned int ptInd() const {return _ptInd;}
  inline void setPtInd(unsigned int ind) {_ptInd = ind;}

  inline void updatePos(const QPointF &pos) { setPos(pos.x(),pos.y()); }

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent (QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

 private:
  DrawingWidget *_drawingWidget;
  unsigned int _curveInd;
  unsigned int _ptInd;
  static const int _size = 10;
};

#endif // DRAWING_WIDGET_ITEMS_H
