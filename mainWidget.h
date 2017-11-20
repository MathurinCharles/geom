#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QDir>
#include <QTimer>

#include "toolsWidget.h"
#include "drawingWidget.h"
#include "animationWidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

    public:
  MainWindow();
  ~MainWindow();

  inline void refresh() {
    _drawingWidget->refresh();
    _animationWidget->refresh();
  }
  
  inline void refreshDrawing() {
    _drawingWidget->refresh();

  }

  inline void refreshAnimation() {
    _animationWidget->refresh();
  }
  
  void sceneSizeChanged();
  void nbFramesChanged();
  void fpsChanged();

  inline void frameChanged() {
    _drawingWidget->frameChanged();
  }

  void selectionChanged() {
    _animationWidget->selectionChanged();
  }

  void playAnimation();
  void stopAnimation();

 private:
  // all the widgets should be there 
  ToolsWidget     *_toolsWidget;
  DrawingWidget   *_drawingWidget;
  AnimationWidget *_animationWidget;
  QTimer          *_timer;

 protected:
  void closeEvent(QCloseEvent *event);

  private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void settings();
  void copy();
  void paste();
  void selectAll();
  void help();
  void about();
  bool closeAppli();
  void nextFrame();
  void prevFrame();
  void firstFrame();
  void lastFrame();
  void play();
  void stop();
  void timerEvent();

 private:
  void createActions();
  void createMenus();
  bool maybeSave();
  void clearAll();
  void setCurrentFile(const QString &fileName);

  QString  _currentFile;

  QMenu   *_fileMenu;
  QMenu   *_editMenu;
  QMenu   *_helpMenu;
  QMenu   *_animMenu;

  QAction *_newAct;
  QAction *_openAct;
  QAction *_saveAct;
  QAction *_settingsAct;
  QAction *_exitAct;
  QAction *_copyAct;
  QAction *_pasteAct;
  QAction *_selectAllAct;
  QAction *_helpAct;
  QAction *_aboutAct;
  QAction *_playAct;
  QAction *_stopAct;
  QAction *_nextFrameAct;
  QAction *_prevFrameAct;
  QAction *_firstFrameAct;
  QAction *_lastFrameAct;
  
  static       QDir    _CURRENT_PATH;
  static const QString _DEFAULT_FILE_NAME;
  static const QString _APPLICATION_NAME;
  static const QString _APPLICATION_EXTENSION;
};

#endif // MAIN_WINDOW_H
