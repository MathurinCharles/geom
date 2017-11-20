#ifndef ANIMATION_WIDGET_ITEMS_H
#define ANIMATION_WIDGET_ITEMS_H

#include <QGraphicsItem>
#include "animationWidget.h"

class AnimatedPointDrawer : public QGraphicsItem {
 public:
 AnimatedPointDrawer(AnimationWidget *animationWidget,int curveInd,int ptInd) 
   : _animationWidget(animationWidget),_curveInd(curveInd),_ptInd(ptInd) {}

  inline int curveInd() const {return _curveInd;}
  inline void setCurveInd(int ind) {_curveInd = ind;}
  inline int ptInd() const {return _ptInd;}
  inline void setPtInd(int ind) {_ptInd = ind;}

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  
 private:
  static const int _size = 10;

  AnimationWidget *_animationWidget;
  int _curveInd;
  int _ptInd;
};


#endif // DRAWING_WIDGET_ITEMS_H
