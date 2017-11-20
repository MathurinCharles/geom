#ifndef TOOLS_WIDGET_H
#define TOOLS_WIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QColorDialog>
#include <QColor>
#include <QString>
#include <QSpinBox>

class MainWindow;

class ToolsWidget : public QWidget {
  Q_OBJECT

    public:
  ToolsWidget(MainWindow *mainWindow,QWidget *parent=NULL);
  ~ToolsWidget();
  
  void updateFrameNumber();

  public slots:
    void createButtonClicked();
    void editButtonClicked();
    void curveTypeChanged(const QString &);
    void penColorClicked();
    void brushColorClicked();
    void penWidthChanged(int);
    void penColorChanged(const QColor &);
    void brushColorChanged(const QColor &);
    void playClicked();
    void stopClicked();
    void prevClicked();
    void nextClicked();
    void frameChanged(int);
    void addPtBeforeClicked();
    void addPtAfterClicked();
    void delPtClicked();
    void sizeChanged();
    void nbFramesChanged(int);
    void fpsChanged(int);
    void kfModePtClicked();
    void kfModeCurveClicked();
    void kfModeSceneClicked();
    void kfAddClicked();
    void kfDelClicked();
    void kfNextPtClicked();
    void kfPrevPtClicked();
    void kfApplyTypeClicked();
	
 private:
    void initTools();
    
  MainWindow *_mainWindow;

  QGroupBox   *_sceneBox;
  QGroupBox   *_toolBox;
  QGroupBox   *_editBox;
  QGroupBox   *_styleBox;
  QGroupBox   *_animBox;
  QGroupBox   *_keyframing;

  QSpinBox    *_width;
  QSpinBox    *_height;
  QSpinBox    *_nbFrames;
  QSpinBox    *_fps;

  QPushButton *_createButton;
  QPushButton *_editButton;
  QComboBox   *_curveTypes;

  QPushButton *_penColor;
  QPushButton *_brushColor;
  QSlider     *_penWidth;
  QColorDialog *_penColorPicker;
  QColorDialog *_brushColorPicker;
  
  QPushButton *_play;
  QPushButton *_stop;
  QPushButton *_next;
  QPushButton *_prev;
  QLabel      *_frame;
  QSlider     *_sframe;

  QPushButton *_addPtBefore;
  QPushButton *_addPtAfter;
  QPushButton *_delPt;

  QPushButton *_kfModePt;
  QPushButton *_kfModeCurve;
  QPushButton *_kfModeScene;
  QPushButton *_kfAdd;
  QPushButton *_kfDel;
  QPushButton *_kfNextPt;
  QPushButton *_kfPrevPt;
  QComboBox   *_kfType;
  QPushButton *_kfApplyType;
  
  static const QColor _SELECTED_COLOR;
  static const QColor _UNSELECTED_COLOR;
  
  inline void setButtonColor(QPushButton *button,const QColor &col) {
    int r = col.red();
    int g = col.green();
    int b = col.blue();
    QString c = "("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
    button->setStyleSheet("background-color: rgb"+c+"; color: rgb(255, 255, 255)");
  }
};

#endif // TOOLS_WIDGET_H
