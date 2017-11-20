#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <assert.h>
#include <QString>
#include <QStringList>
#include <QColor>
#include "curve2D.h"
#include "curve1D.h"
#include "animatedPoint.h"

class Scene {
 public:
  enum { CREATE_CURVE=0, EDIT_CURVE=1 };
  enum { EDIT_ADD_BEFORE=0, EDIT_ADD_AFTER=1, EDIT_DEL_PT=2 };
  enum { KF_MODE_PT=0, KF_MODE_CURVE=1,KF_MODE_SCENE=2 };

  ~Scene();
  
  static Scene *get() {
    if(!_instance) {
      _instance = new Scene();
    }
    return _instance;
  } 

  inline void clean() {
    for(unsigned int i=0;i<nbCurves();++i) {
      delete _curves[i];
    }
    _curves.clear();
  }

  inline unsigned int nbCurves() const {
    return _curves.size();
  }

  inline Curve2D *getCurve(unsigned int ind) {
    assert(ind<nbCurves());
    return _curves[ind];
  }

  inline Curve2D *createCurve(const QString &type,Curve2D *c=NULL) {
    std::map<QString,Curve2DConstructor *>::const_iterator n = _curveBuilders.find(type);

    assert(n!=_curveBuilders.end());
    Curve2DConstructor *cc = n->second;
    Curve2D *curve = c ? cc->create(c,type) : cc->create(type);
    curve->setPenColor(currentPenColor());
    curve->setBrushColor(currentBrushColor());
    curve->setPenWidth(currentPenWidth());
    return curve;
  } 

  inline Curve1D *createFunction(const QString &type,Curve1D *c=NULL) {
    std::map<QString,Curve1DConstructor *>::const_iterator n = _functionBuilders.find(type);

    assert(n!=_functionBuilders.end());
    Curve1DConstructor *cc = n->second;
    Curve1D *curve = c ? cc->create(c,type) : cc->create(type);
    return curve;
  } 

  inline void addCurve(Curve2D *curve) {
    _curves.push_back(curve);
  }
  
  inline void delCurve(unsigned int i) {
    assert(i<nbCurves());
    delete _curves[i];
    _curves[i] = NULL;
    _curves.erase(_curves.begin()+i);
  }

  inline void addKeyframe(int curveInd,int ptInd) {
    // add keyframe to a particular point 
    if(curveInd<0 || ptInd<0)
      return;

    assert(curveInd<(int)nbCurves());
    assert(ptInd<(int)_curves[curveInd]->nbPts());

    AnimatedPoint *pt = _curves[curveInd]->get(ptInd);
    float f = (float)currentFrame();
    pt->setAt(f,pt->evalAt(f));
  }

  inline void addKeyframe(int curveInd) {
    // add keyframe to a curve
    if(curveInd<0)
      return;

    assert(curveInd<(int)nbCurves());

    for(unsigned int i=0;i<_curves[curveInd]->nbPts();++i) {
      addKeyframe(curveInd,i);
    }
  }

  inline void addKeyframe() {
    // add keyframe to the whole scene 
    for(unsigned int i=0;i<nbCurves();++i) {
      addKeyframe(i);
    }
  }


  inline void delKeyframe(int curveInd,int ptInd) {
    if(curveInd<0 || ptInd<0)
      return;

    assert(curveInd<(int)nbCurves());
    assert(ptInd<(int)_curves[curveInd]->nbPts());

    AnimatedPoint *pt = _curves[curveInd]->get(ptInd);
    float f = (float)currentFrame();
    pt->delAt(f);
  }

  inline void delKeyframe(int curveInd) {
    // del keyframe to a curve
    if(curveInd<0)
      return;
    
    assert(curveInd<(int)nbCurves());
    
    for(unsigned int i=0;i<_curves[curveInd]->nbPts();++i) {
      delKeyframe(curveInd,i);
    }
  }

  inline void delKeyframe() {
    // del keyframe to the whole scene 
    for(unsigned int i=0;i<nbCurves();++i) {
      delKeyframe(i);
    }
  }

  unsigned int nextKeyframe(int curveInd,int ptInd) {
    if(curveInd<0 || ptInd<0)
      return currentFrame();
    
    assert(curveInd<(int)nbCurves());
    assert(ptInd<(int)_curves[curveInd]->nbPts());
    
    AnimatedPoint *pt = _curves[curveInd]->get(ptInd);
    unsigned int current = currentFrame();
    unsigned int found = nbFrames()-1;
    
    Curve1D *c;
    c = pt->getXCurve();
    for(unsigned int i=0;i<c->nbPts();++i) {
      Vector2f p = c->get(i);
      if(p[0]>(float)current && p[0]<=(float)found) {
	found = (unsigned int)p[0];
	break;
      }
    }

    c = pt->getYCurve();
    for(unsigned int i=0;i<c->nbPts();++i) {
      Vector2f p = c->get(i);
      if(p[0]>(float)current && p[0]<=(float)found) {
	found = (unsigned int)p[0];
	break;
      }
    }
    return found;
  }

  unsigned int nextKeyframe(int curveInd) {
    if(curveInd<0)
      return currentFrame();
    
    assert(curveInd<(int)nbCurves());
    
    unsigned int found = nbFrames()-1;
    for(unsigned int i=0;i<_curves[curveInd]->nbPts();++i) {
      found = std::min(found,nextKeyframe(curveInd,i));
    }
    
    return found;
  }

  unsigned int nextKeyframe() {    
    unsigned int found = nbFrames()-1;
    
    for(unsigned int i=0;i<nbCurves();++i) {
      found = std::min(found,nextKeyframe(i));
    }
    
    return found;
  }

  unsigned int prevKeyframe(int curveInd,int ptInd) {
    if(curveInd<0 || ptInd<0)
      return currentFrame();
    
    assert(curveInd<(int)nbCurves());
    assert(ptInd<(int)_curves[curveInd]->nbPts());
    
    AnimatedPoint *pt = _curves[curveInd]->get(ptInd);
    unsigned int current = currentFrame();
    unsigned int found = 0;
    
    Curve1D *c;
    c = pt->getXCurve();
    for(int i=(int)c->nbPts()-1;i>=0;--i) {
      Vector2f p = c->get(i);
      if(p[0]<(float)current && p[0]>=(float)found) {
	found = (unsigned int)p[0];
	break;
      }
    }

    c = pt->getYCurve();
    for(int i=(int)c->nbPts()-1;i>=0;--i) {
      Vector2f p = c->get(i);
      if(p[0]<(float)current && p[0]>=(float)found) {
	found = (unsigned int)p[0];
	break;
      }
    }

    return found;
  }

  unsigned int prevKeyframe(int curveInd) {
    if(curveInd<0)
      return currentFrame();
    
    assert(curveInd<(int)nbCurves());
    
    unsigned int found = 0;
    for(unsigned int i=0;i<_curves[curveInd]->nbPts();++i) {
      found = std::max(found,prevKeyframe(curveInd,i));
    }
    
    return found;
  }

  unsigned int prevKeyframe() {    
    unsigned int found = 0;
    
    for(unsigned int i=0;i<nbCurves();++i) {
      found = std::max(found,prevKeyframe(i));
    }
    
    return found;
  }

  
  inline unsigned int nbFrames() const {
    return _nbFrames;
  }

  inline unsigned int fps() const {
    return _fps;
  }
  
  inline unsigned int width() const {
    return _width;
  }

  inline unsigned int height() const {
    return _height;
  }

  inline void setAnimationSettings(unsigned int nbFrames,unsigned int fps) {
    _nbFrames = nbFrames;
    _fps = fps;
  }
  
  inline void setCanevaSettings(unsigned int width,unsigned int height) {
    _width = width;
    _height = height;
  }

  inline void setDefaultSettings() {
    _isAnimated = false;
    _nbFrames = 100;
    _fps = 16;
    _currentFrame = 0;
    _width = 800;
    _height = 600;
    _currentTool = CREATE_CURVE;
    _currentCurveType = _curveBuilders.begin()->first;
    _currentKfType = _functionBuilders.begin()->first;
    _selectedCurve = -1;
    _selectedPoints.clear();
    _currentPenColor = QColor(0,0,0,255);
    _currentBrushColor = QColor(255,255,255,0);
    _currentPenWidth = 1;
    _editMode = EDIT_ADD_BEFORE;
    _kfMode = KF_MODE_PT;
  }

  QStringList curveTypes() const {
    QStringList l;
    std::map<QString,Curve2DConstructor *>::const_iterator mit(_curveBuilders.begin()),mend(_curveBuilders.end());
  
    for(;mit!=mend;++mit) { 
      l.push_back(mit->first); 
    }
    return l;
  }

  QStringList functionTypes() const {
    QStringList l;
    std::map<QString,Curve1DConstructor *>::const_iterator mit(_functionBuilders.begin()),mend(_functionBuilders.end());
  
    for(;mit!=mend;++mit) { 
      l.push_back(mit->first); 
    }
    return l;
  }

  inline QString currentCurveType() const {
    return _currentCurveType;
  }

  inline void setCurrentCurveType(const QString &t) {
    _currentCurveType = t;
    if(selectedCurve()>=0) {
      Curve2D *c = _curves[selectedCurve()];
      _curves[selectedCurve()] = createCurve(t,c);
      delete c;
    }
  }

  inline QString currentKfType() const {
    return _currentKfType;
  }

  void setCurrentKfType(int curveInd,int ptInd,const QString &t) {
    // set function type for current selected control point 
    if(curveInd<0 || ptInd<0) {
      return;
    }

    assert(curveInd<(int)nbCurves());
    assert(ptInd<(int)(getCurve(curveInd)->nbPts()));

    _currentKfType = t;
    
    getCurve(curveInd)->get(ptInd)->setType(t);
  }

  void setCurrentKfType(int curveInd,const QString &t) {
    // set function type for selected curve  
    if(curveInd<0) {
      return;
    }

    assert(curveInd<(int)nbCurves());

    _currentKfType = t;
    
    for(unsigned int i=0;i<getCurve(curveInd)->nbPts();++i) {
      setCurrentKfType(curveInd,i,t);
    }
  }

  void setCurrentKfType(const QString &t) {
    // set functin type for the whole scene 
    for(unsigned int i=0;i<nbCurves();++i) {
      setCurrentKfType(i,t);
    }
  }


  inline int currentKfMode() const {
    return _kfMode;
  }

  inline void setCurrentKfMode(int m) {
    _kfMode = m;
  }
  
  inline int currentTool() const {
    return _currentTool;
  }

  inline void setCurrentTool(int t) {
    _currentTool = t;
  }

  inline int selectedCurve() const {
    return _selectedCurve;
  }

  inline void setSelectedCurve(int i) {
    _selectedCurve = i;
  } 

  inline std::vector<int> selectedPoints() const {
    return _selectedPoints;
  }

  inline int selectedPoint() const {
    return _selectedPoints.empty() ? -1 : _selectedPoints[0];
  }
  
  inline int selectedPoint(unsigned int i) const {
    assert(i<_selectedPoints.size());
    return _selectedPoints[i];
  }
  
  inline void setSelectedPoints(const std::vector<int> &pts) {
    _selectedPoints = pts;
  }

  inline void setSelectedPoint(int pt) {
    _selectedPoints.clear();
    _selectedPoints.push_back(pt);
  }

  inline void cleanSelectedPoints() {
    _selectedPoints.clear();
  }

  inline unsigned int currentFrame() const {
    return _currentFrame;
  }

  inline void setCurrentFrame(unsigned int f) {
    _currentFrame = f;
  }

  inline bool isCurveSelected(unsigned int curveInd) {
    return _selectedCurve==(int)curveInd;
  }

  inline bool isPointSelected(unsigned int curveInd,unsigned int ptInd) {
    if(!isCurveSelected(curveInd))
      return false;

    for(unsigned int i=0;i<_selectedPoints.size();++i) {
      if((int)ptInd==_selectedPoints[i])
	return true;
    }

    return false;
  }

  inline QColor currentPenColor() const {
    return _currentPenColor;
  }

  inline QColor currentBrushColor() const {
    return _currentBrushColor;
  }

  inline int currentPenWidth() const {
    return _currentPenWidth;
  }

  inline void setCurrentPenColor(const QColor &c) {
    _currentPenColor = c;
  }

  inline void setCurrentBrushColor(const QColor &c) {
    _currentBrushColor = c;
  }

  inline void setCurrentPenWidth(int w) {
    _currentPenWidth = w;
  }

  inline bool isAnimated() const {
    return _isAnimated;
  }

  inline void setAnimated(bool a) {
    _isAnimated = a;
  }

  inline int editMode() const {
    return _editMode;
  }

  inline void setEditMode(int m) {
    _editMode = m;
  }
  
 private:
  Scene();

  // there will be only one instance of scene 
  static Scene *_instance;

  void initCurveBuilders();
  void cleanCurveBuilders();
  void initFunctionBuilders();
  void cleanFunctionBuilders();

  void addCurveBuilder(Curve2DConstructor *cc) {
    assert(cc);
    _curveBuilders[cc->type()] = cc;
  }

  void addFunctionBuilder(Curve1DConstructor *cc) {
    assert(cc);
    _functionBuilders[cc->type()] = cc;
  }

  // all the objects (2D curves)
  std::vector<Curve2D *> _curves;

  // the builder 
  std::map<QString,Curve2DConstructor *> _curveBuilders;
  std::map<QString,Curve1DConstructor *> _functionBuilders;

  // animation settings
  bool         _isAnimated;
  unsigned int _nbFrames;
  unsigned int _fps;
  unsigned int _currentFrame;
  int          _kfMode;
  QString      _currentKfType;

  // caneva settings
  unsigned int _width;
  unsigned int _height;

  // selection, user settings
  QString _currentCurveType;
  int _currentTool;
  int _editMode;
  int _selectedCurve;
  std::vector<int> _selectedPoints;

  // style settings
  QColor _currentPenColor;
  QColor _currentBrushColor;
  int    _currentPenWidth;
};

#endif // SCENE_H
