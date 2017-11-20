#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <vector>

#include <iostream>
#include "mainWidget.h"
#include "toolsWidget.h"
#include "scene.h"

using namespace std;

const QColor ToolsWidget::_SELECTED_COLOR = QColor(175,80,80);
const QColor ToolsWidget::_UNSELECTED_COLOR = QColor(150,150,150);


ToolsWidget::ToolsWidget(MainWindow *mainWindow,QWidget *parent)
  : QWidget(parent),
    _mainWindow(mainWindow) {
  
  Scene *sce = Scene::get();
  QStringList ct = sce->curveTypes();
  QStringList cf = sce->functionTypes();
  
  QHBoxLayout *hl;
  QVBoxLayout *vl;
  QLabel *label;
  QVBoxLayout *l = new QVBoxLayout();
  

  // box for scene 
  _sceneBox = new QGroupBox("Scene");
  vl = new QVBoxLayout();
  hl = new QHBoxLayout();
  label = new QLabel("Size");
  _width = new QSpinBox();
  _width->setMinimum(100);
  _width->setMaximum(2000);
  _height = new QSpinBox();
  _height->setMinimum(100);
  _height->setMaximum(2000);
  hl->addWidget(label);
  hl->addWidget(_width);
  hl->addWidget(_height);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  label = new QLabel("Nb Frames");
  _nbFrames = new QSpinBox();
  _nbFrames->setMinimum(1);
  _nbFrames->setMaximum(5000);
  hl->addWidget(label);
  hl->addWidget(_nbFrames);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  label = new QLabel("FPS");
  _fps = new QSpinBox();
  _fps->setMinimum(1);
  _fps->setMaximum(100);
  hl->addWidget(label);
  hl->addWidget(_fps);
  vl->addItem(hl);
  _sceneBox->setLayout(vl);
  l->addWidget(_sceneBox);



  // box for tools 
  _toolBox = new QGroupBox("Tools");
  _createButton = new QPushButton("Create");
  _createButton->setAutoFillBackground(true);
  _editButton = new QPushButton("Edit");
  _editButton->setAutoFillBackground(true);
  hl = new QHBoxLayout();
  hl->addWidget(_createButton);
  hl->addWidget(_editButton);
  _curveTypes = new QComboBox();
  _curveTypes->addItems(ct);
  vl = new QVBoxLayout();
  vl->addItem(hl);
  vl->addWidget(_curveTypes);
  _toolBox->setLayout(vl);
  l->addWidget(_toolBox);

  // box for edit modes 
  _editBox = new QGroupBox("Options");
  label = new QLabel("Edit:");
  _addPtBefore = new QPushButton("Add <-");
  _addPtAfter = new QPushButton("Add ->");
  _delPt = new QPushButton("Del");
  _addPtBefore->setAutoFillBackground(true);
  _addPtAfter->setAutoFillBackground(true);
  _delPt->setAutoFillBackground(true);
  hl = new QHBoxLayout();
  hl->addWidget(label);
  hl->addWidget(_addPtBefore);
  hl->addWidget(_addPtAfter);
  hl->addWidget(_delPt);
  vl = new QVBoxLayout();
  vl->addItem(hl);
  _editBox->setLayout(vl);
  l->addWidget(_editBox);

  // box for style 
  _styleBox = new QGroupBox("Style");
  label = new QLabel("Pen Color");
  _penColor = new QPushButton();
  _penColor->setAutoFillBackground(true);
  hl = new QHBoxLayout();
  hl->addWidget(label);
  hl->addWidget(_penColor);
  vl = new QVBoxLayout();
  vl->addItem(hl);
  label = new QLabel("Fill Color");
  _brushColor = new QPushButton();
  _brushColor->setAutoFillBackground(true);
  hl = new QHBoxLayout();
  hl->addWidget(label);
  hl->addWidget(_brushColor);
  vl->addItem(hl);
  label = new QLabel("Pen Width");
  _penWidth = new QSlider();
  _penWidth->setMinimum(1);
  _penWidth->setMaximum(50);
  _penWidth->setOrientation(Qt::Horizontal);
  hl = new QHBoxLayout();
  hl->addWidget(label);
  hl->addWidget(_penWidth);
  vl->addItem(hl);
  _styleBox->setLayout(vl);
  l->addWidget(_styleBox);
  _penColorPicker = new QColorDialog(this);
  _penColorPicker->setOptions(QColorDialog::ShowAlphaChannel);
  _penColorPicker->setHidden(true);
  _brushColorPicker = new QColorDialog(this);
  _brushColorPicker->setOptions(QColorDialog::ShowAlphaChannel);
  _brushColorPicker->setHidden(true);
  
  // box for animation
  _animBox = new QGroupBox("Animation");
  vl = new QVBoxLayout();
  _play = new QPushButton("Play");
  _stop = new QPushButton("Stop");
  _play->setAutoFillBackground(true);
  _stop->setAutoFillBackground(true);
  _next = new QPushButton("Next");
  _prev = new QPushButton("Prev");
  hl = new QHBoxLayout();
  hl->addWidget(_play);
  hl->addWidget(_stop);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  hl->addWidget(_prev);
  hl->addWidget(_next);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  _sframe = new QSlider();
  _sframe->setOrientation(Qt::Horizontal);
  _sframe->setMinimum(0);
  _sframe->setMaximum(100);
  _frame = new QLabel();
  hl->addWidget(_sframe);
  hl->addWidget(_frame);
  vl->addItem(hl);
  _animBox->setLayout(vl);
  l->addWidget(_animBox);

  // box for keyframes
  _keyframing = new QGroupBox("Keyframing");
  vl = new QVBoxLayout();
  hl = new QHBoxLayout();
  _kfModePt = new QPushButton("Point");
  _kfModeCurve = new QPushButton("Curve");
  _kfModeScene = new QPushButton("Scene");
  _kfModePt->setAutoFillBackground(true);
  _kfModeCurve->setAutoFillBackground(true);
  _kfModeScene->setAutoFillBackground(true);
  hl->addWidget(_kfModePt);
  hl->addWidget(_kfModeCurve);
  hl->addWidget(_kfModeScene);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  _kfAdd = new QPushButton("Add");
  _kfDel = new QPushButton("Del");
  _kfAdd->setAutoFillBackground(true);
  _kfDel->setAutoFillBackground(true);
  hl->addWidget(_kfAdd);
  hl->addWidget(_kfDel);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  _kfNextPt = new QPushButton("Next kf");
  _kfPrevPt = new QPushButton("Prev kf");
  _kfNextPt->setAutoFillBackground(true);
  _kfPrevPt->setAutoFillBackground(true);
  hl->addWidget(_kfPrevPt);
  hl->addWidget(_kfNextPt);
  vl->addItem(hl);
  hl = new QHBoxLayout();
  _kfType = new QComboBox();
  _kfType->addItems(cf);
  _kfApplyType = new QPushButton("Apply");
  _kfApplyType->setAutoFillBackground(true);
  hl->addWidget(_kfType);
  hl->addWidget(_kfApplyType);
  vl->addItem(hl);
  _keyframing->setLayout(vl);
  l->addWidget(_keyframing);
  
  initTools();

  connect(_createButton,SIGNAL(clicked()),this,SLOT(createButtonClicked()));
  connect(_editButton,SIGNAL(clicked()),this,SLOT(editButtonClicked()));
  connect(_curveTypes,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(curveTypeChanged(const QString &)));
  connect(_penColor,SIGNAL(clicked()),this,SLOT(penColorClicked()));
  connect(_brushColor,SIGNAL(clicked()),this,SLOT(brushColorClicked()));
  connect(_penWidth,SIGNAL(valueChanged(int)),this,SLOT(penWidthChanged(int)));
  connect(_penColorPicker,SIGNAL(colorSelected(const QColor &)),this,SLOT(penColorChanged(const QColor &)));
  connect(_brushColorPicker,SIGNAL(colorSelected(const QColor &)),this,SLOT(brushColorChanged(const QColor &)));
  connect(_play,SIGNAL(clicked()),this,SLOT(playClicked()));
  connect(_stop,SIGNAL(clicked()),this,SLOT(stopClicked()));
  connect(_prev,SIGNAL(clicked()),this,SLOT(prevClicked()));
  connect(_next,SIGNAL(clicked()),this,SLOT(nextClicked()));
  connect(_addPtBefore,SIGNAL(clicked()),this,SLOT(addPtBeforeClicked()));
  connect(_addPtAfter,SIGNAL(clicked()),this,SLOT(addPtAfterClicked()));
  connect(_delPt,SIGNAL(clicked()),this,SLOT(delPtClicked()));
  connect(_width,SIGNAL(valueChanged(int)),this,SLOT(sizeChanged()));
  connect(_height,SIGNAL(valueChanged(int)),this,SLOT(sizeChanged()));
  connect(_nbFrames,SIGNAL(valueChanged(int)),this,SLOT(nbFramesChanged(int)));
  connect(_fps,SIGNAL(valueChanged(int)),this,SLOT(fpsChanged(int)));
  connect(_sframe,SIGNAL(valueChanged(int)),this,SLOT(frameChanged(int)));
  connect(_kfModePt,SIGNAL(clicked()),this,SLOT(kfModePtClicked()));
  connect(_kfModeCurve,SIGNAL(clicked()),this,SLOT(kfModeCurveClicked()));
  connect(_kfModeScene,SIGNAL(clicked()),this,SLOT(kfModeSceneClicked()));
  connect(_kfAdd,SIGNAL(clicked()),this,SLOT(kfAddClicked()));
  connect(_kfDel,SIGNAL(clicked()),this,SLOT(kfDelClicked()));
  connect(_kfNextPt,SIGNAL(clicked()),this,SLOT(kfNextPtClicked()));
  connect(_kfPrevPt,SIGNAL(clicked()),this,SLOT(kfPrevPtClicked()));
  connect(_kfApplyType,SIGNAL(clicked()),this,SLOT(kfApplyTypeClicked()));
  
  setLayout(l);
}

ToolsWidget::~ToolsWidget() {
  
}

void ToolsWidget::createButtonClicked() {
  Scene *sce = Scene::get();

  if(sce->currentTool()==Scene::CREATE_CURVE)
    return;

  setButtonColor(_createButton,_SELECTED_COLOR);
  setButtonColor(_editButton,_UNSELECTED_COLOR);
  
  sce->setSelectedCurve(-1);
  sce->setSelectedPoints(vector<int>());
  sce->setCurrentTool(Scene::CREATE_CURVE);
  _mainWindow->selectionChanged();
  _mainWindow->refresh();
}

void ToolsWidget::editButtonClicked() {
  Scene *sce = Scene::get();

  if(sce->currentTool()==Scene::EDIT_CURVE)
    return;

  sce->setSelectedCurve((int)sce->nbCurves()-1);
  
  setButtonColor(_createButton,_UNSELECTED_COLOR);
  setButtonColor(_editButton,_SELECTED_COLOR);
  sce->setCurrentTool(Scene::EDIT_CURVE);
  _mainWindow->selectionChanged();
  _mainWindow->refresh();
}

void ToolsWidget::curveTypeChanged(const QString &text) {
  Scene *sce = Scene::get();
  sce->setCurrentCurveType(text);
  _mainWindow->refresh();
}


void ToolsWidget::penColorClicked() {
  _penColorPicker->setHidden(false);
}

void ToolsWidget::brushColorClicked() {
  _brushColorPicker->setHidden(false);
}

void ToolsWidget::penWidthChanged(int width) {
  Scene *sce = Scene::get();
  sce->setCurrentPenWidth(width);
  if(sce->selectedCurve()>=0) {
    sce->getCurve(sce->selectedCurve())->setPenWidth(width);
    _mainWindow->refresh();
  }
}

void ToolsWidget::penColorChanged(const QColor &col) {
  Scene *sce = Scene::get();

  sce->setCurrentPenColor(col);
  setButtonColor(_penColor,col);

  if(sce->selectedCurve()>=0) {
    sce->getCurve(sce->selectedCurve())->setPenColor(col);
    _mainWindow->refresh();
  }
}

void ToolsWidget::brushColorChanged(const QColor &col) {
  Scene *sce = Scene::get();
  
  sce->setCurrentBrushColor(col);
  setButtonColor(_brushColor,col);

  if(sce->selectedCurve()>=0) {
    sce->getCurve(sce->selectedCurve())->setBrushColor(col);
    _mainWindow->refresh();
  }
}

void ToolsWidget::playClicked() {
  Scene *sce = Scene::get();
  
  if(sce->isAnimated()) 
    return;

  setButtonColor(_play,_SELECTED_COLOR);
  setButtonColor(_stop,_UNSELECTED_COLOR);
  _mainWindow->playAnimation();
}

void ToolsWidget::stopClicked() {
  Scene *sce = Scene::get();

  if(!sce->isAnimated()) 
    return;
  
  setButtonColor(_play,_UNSELECTED_COLOR);
  setButtonColor(_stop,_SELECTED_COLOR);
  _mainWindow->stopAnimation();
}

void ToolsWidget::prevClicked() {
  Scene *sce = Scene::get();

  if(sce->isAnimated() || sce->currentFrame()==0) 
    return;

  sce->setCurrentFrame(sce->currentFrame()-1);
  updateFrameNumber();
  _mainWindow->frameChanged();
  _mainWindow->refresh();
}

void ToolsWidget::nextClicked() {
  Scene *sce = Scene::get();

  if(sce->isAnimated() || sce->currentFrame()==(sce->nbFrames()-1)) 
    return;
  
  sce->setCurrentFrame(sce->currentFrame()+1);
  updateFrameNumber();
  _mainWindow->frameChanged();
  _mainWindow->refresh();
}

void ToolsWidget::frameChanged(int f) {
  Scene *sce = Scene::get();

  if(sce->isAnimated())
    return;
  
  sce->setCurrentFrame(f);
  updateFrameNumber();
  _mainWindow->frameChanged();
  _mainWindow->refresh();
}


void ToolsWidget::addPtBeforeClicked() {
  Scene *sce = Scene::get();
  if(sce->editMode()==Scene::EDIT_ADD_BEFORE)
    return;
  sce->setEditMode(Scene::EDIT_ADD_BEFORE);
  setButtonColor(_addPtBefore,_SELECTED_COLOR);
  setButtonColor(_addPtAfter,_UNSELECTED_COLOR);
  setButtonColor(_delPt,_UNSELECTED_COLOR);
  _mainWindow->selectionChanged();
}

void ToolsWidget::addPtAfterClicked() {
  Scene *sce = Scene::get();
  if(sce->editMode()==Scene::EDIT_ADD_AFTER)
    return;
  sce->setEditMode(Scene::EDIT_ADD_AFTER);
  setButtonColor(_addPtBefore,_UNSELECTED_COLOR);
  setButtonColor(_addPtAfter,_SELECTED_COLOR);
  setButtonColor(_delPt,_UNSELECTED_COLOR);
  _mainWindow->selectionChanged();
}

void ToolsWidget::delPtClicked() {
  Scene *sce = Scene::get();
   if(sce->editMode()==Scene::EDIT_DEL_PT)
    return;
  sce->setEditMode(Scene::EDIT_DEL_PT);
  setButtonColor(_addPtBefore,_UNSELECTED_COLOR);
  setButtonColor(_addPtAfter,_UNSELECTED_COLOR);
  setButtonColor(_delPt,_SELECTED_COLOR);
  _mainWindow->selectionChanged();
}

void ToolsWidget::sizeChanged() {
  Scene *sce = Scene::get();
  sce->setCanevaSettings(_width->value(),_height->value());
  _mainWindow->sceneSizeChanged();
  _mainWindow->refresh();
}

void ToolsWidget::nbFramesChanged(int v) {
  Scene *sce = Scene::get();
  sce->setAnimationSettings(v,sce->fps());
  _sframe->setMaximum(v-1);
  _mainWindow->nbFramesChanged();
  _mainWindow->refresh();
}

void ToolsWidget::fpsChanged(int v) {
  Scene *sce = Scene::get();
  sce->setAnimationSettings(sce->nbFrames(),v);
  _mainWindow->fpsChanged();
}


void ToolsWidget::kfModePtClicked() {
  Scene *sce = Scene::get();
  sce->setCurrentKfMode(Scene::KF_MODE_PT);
  setButtonColor(_kfModePt,_SELECTED_COLOR);
  setButtonColor(_kfModeCurve,_UNSELECTED_COLOR);
  setButtonColor(_kfModeScene,_UNSELECTED_COLOR);
  _mainWindow->selectionChanged();
  _mainWindow->refresh();
}

void ToolsWidget::kfModeCurveClicked() {
  Scene *sce = Scene::get();
  sce->setCurrentKfMode(Scene::KF_MODE_CURVE);
  setButtonColor(_kfModePt,_UNSELECTED_COLOR);
  setButtonColor(_kfModeCurve,_SELECTED_COLOR);
  setButtonColor(_kfModeScene,_UNSELECTED_COLOR);
  _mainWindow->selectionChanged();
  _mainWindow->refresh();
}

void ToolsWidget::kfModeSceneClicked() {
  Scene *sce = Scene::get();
  sce->setCurrentKfMode(Scene::KF_MODE_SCENE);
  setButtonColor(_kfModePt,_UNSELECTED_COLOR);
  setButtonColor(_kfModeCurve,_UNSELECTED_COLOR);
  setButtonColor(_kfModeScene,_SELECTED_COLOR);
  _mainWindow->selectionChanged();
  _mainWindow->refresh();
}

void ToolsWidget::kfAddClicked() {
  Scene *sce = Scene::get();
  if(sce->isAnimated())
    return;
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    sce->addKeyframe(sce->selectedCurve(),sce->selectedPoint());
    break;
  case Scene::KF_MODE_CURVE:
    sce->addKeyframe(sce->selectedCurve());
    break;
  case Scene::KF_MODE_SCENE:
    sce->addKeyframe();
    break;
  default: break;
  }
  _mainWindow->refresh();
}

void ToolsWidget::kfDelClicked() {
  Scene *sce = Scene::get();
  if(sce->isAnimated())
    return;
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    sce->delKeyframe(sce->selectedCurve(),sce->selectedPoint());
    break;
  case Scene::KF_MODE_CURVE:
    sce->delKeyframe(sce->selectedCurve());
    break;
  case Scene::KF_MODE_SCENE:
    sce->delKeyframe();
    break;
  default: break;
  }
  _mainWindow->frameChanged();
  _mainWindow->refresh();
}

void ToolsWidget::kfNextPtClicked() {
  Scene *sce = Scene::get();
  unsigned int f = sce->currentFrame();
  unsigned int t = f;
  
  if(sce->isAnimated())
    return;
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    f = sce->nextKeyframe(sce->selectedCurve(),sce->selectedPoint());
    break;
  case Scene::KF_MODE_CURVE:
    f = sce->nextKeyframe(sce->selectedCurve());
    break;
  case Scene::KF_MODE_SCENE:
    f = sce->nextKeyframe();
    break;
  default: break;
  }

  if(t!=f) {
    sce->setCurrentFrame(f);
    updateFrameNumber();
    _mainWindow->frameChanged();
    _mainWindow->refresh();
  }
}

void ToolsWidget::kfPrevPtClicked() {
  Scene *sce = Scene::get();
  if(sce->isAnimated())
    return;
  
  unsigned int f = sce->currentFrame();
  unsigned int t = f;
  
  if(sce->isAnimated())
    return;
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    f = sce->prevKeyframe(sce->selectedCurve(),sce->selectedPoint());
    break;
  case Scene::KF_MODE_CURVE:
    f = sce->prevKeyframe(sce->selectedCurve());
    break;
  case Scene::KF_MODE_SCENE:
    f = sce->prevKeyframe();
    break;
  default: break;
  }

  if(t!=f) {
    sce->setCurrentFrame(f);
    updateFrameNumber();
    _mainWindow->frameChanged();
    _mainWindow->refresh();
  }
}

void ToolsWidget::kfApplyTypeClicked() {
  Scene *sce = Scene::get();
  QString t = _kfType->currentText();
  
  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    sce->setCurrentKfType(sce->selectedCurve(),sce->selectedPoint(),t);
    break;
  case Scene::KF_MODE_CURVE:
    sce->setCurrentKfType(sce->selectedCurve(),t);
    break;
  case Scene::KF_MODE_SCENE:
    sce->setCurrentKfType(t);
    break;
  default: break;
  }
  _mainWindow->refresh();
}

void ToolsWidget::initTools() {
  Scene *sce = Scene::get();

  _width->setValue(sce->width());
  _height->setValue(sce->height());
  _nbFrames->setValue(sce->nbFrames());
  _fps->setValue(sce->fps());

  setButtonColor(_createButton,_UNSELECTED_COLOR);
  setButtonColor(_editButton,_UNSELECTED_COLOR);

  switch(sce->currentTool()) {
  case Scene::EDIT_CURVE: 
    setButtonColor(_editButton,_SELECTED_COLOR);
    break;
  case Scene::CREATE_CURVE: 
  default: 
    setButtonColor(_createButton,_SELECTED_COLOR);
    Scene::get()->setSelectedCurve(-1);
    Scene::get()->setSelectedPoints(vector<int>());
    break;
  }

  setButtonColor(_addPtBefore,_UNSELECTED_COLOR);
  setButtonColor(_addPtAfter,_UNSELECTED_COLOR);
  setButtonColor(_delPt,_UNSELECTED_COLOR);

  switch(sce->editMode()) {
  case Scene::EDIT_ADD_AFTER: 
    setButtonColor(_addPtAfter,_SELECTED_COLOR);
    break;
  case Scene::EDIT_DEL_PT: 
    setButtonColor(_delPt,_SELECTED_COLOR);
    break;
  case Scene::EDIT_ADD_BEFORE: 
  default: 
    setButtonColor(_addPtBefore,_SELECTED_COLOR);
    break;
  }

  setButtonColor(_penColor,sce->currentPenColor());
  setButtonColor(_brushColor,sce->currentBrushColor());
  _penWidth->setValue(sce->currentPenWidth());

  setButtonColor(_next,_UNSELECTED_COLOR);
  setButtonColor(_prev,_UNSELECTED_COLOR);
  
  if(sce->isAnimated()) {
    setButtonColor(_play,_SELECTED_COLOR);
    setButtonColor(_stop,_UNSELECTED_COLOR);
  } else {
    setButtonColor(_play,_UNSELECTED_COLOR);
    setButtonColor(_stop,_SELECTED_COLOR);
  }

  _sframe->setMaximum(sce->nbFrames()-1);
  updateFrameNumber();

  setButtonColor(_kfModePt,_UNSELECTED_COLOR);
  setButtonColor(_kfModeCurve,_UNSELECTED_COLOR);
  setButtonColor(_kfModeScene,_UNSELECTED_COLOR);
  setButtonColor(_kfAdd,_UNSELECTED_COLOR);
  setButtonColor(_kfDel,_UNSELECTED_COLOR);
  setButtonColor(_kfNextPt,_UNSELECTED_COLOR);
  setButtonColor(_kfPrevPt,_UNSELECTED_COLOR);
  setButtonColor(_kfApplyType,_UNSELECTED_COLOR);

  switch(sce->currentKfMode()) {
  case Scene::KF_MODE_PT:
    setButtonColor(_kfModePt,_SELECTED_COLOR);
    break;
  case Scene::KF_MODE_CURVE:
    setButtonColor(_kfModeCurve,_SELECTED_COLOR);
    break;
  case Scene::KF_MODE_SCENE:
    setButtonColor(_kfModeScene,_SELECTED_COLOR);
    break;
  default: break;
  }
}

void ToolsWidget::updateFrameNumber() {
  _frame->setText("("+QString::number(Scene::get()->currentFrame())+")");
}
