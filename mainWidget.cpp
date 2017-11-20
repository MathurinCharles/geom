#include "mainWidget.h"

#include <iostream>
#include <QDebug>
#include <QMenuBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSplitter>
#include "scene.h"

using namespace std;

const QString MainWindow::_DEFAULT_FILE_NAME = "untitled";
const QString MainWindow::_APPLICATION_NAME = "yourAppName";
const QString MainWindow::_APPLICATION_EXTENSION = ".yan";
QDir MainWindow::_CURRENT_PATH = QDir::currentPath();

MainWindow::MainWindow()
  : QMainWindow(),
    _timer(new QTimer(this)) {

   createActions();
   createMenus();

   _toolsWidget = new ToolsWidget(this);
   _drawingWidget = new DrawingWidget(this);
   _animationWidget = new AnimationWidget(this);

   QSplitter *sp1 = new QSplitter(Qt::Horizontal);
   QSplitter *sp2 = new QSplitter(Qt::Vertical);

   sp2->addWidget(_drawingWidget);
   sp2->addWidget(_animationWidget);
   sp1->addWidget(_toolsWidget);
   sp1->addWidget(sp2);

   sp1->setStretchFactor(0,1);
   sp1->setStretchFactor(1,10);
   sp2->setStretchFactor(0,8);
   sp2->setStretchFactor(1,1);
   
  connect(_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));

   setCentralWidget(sp1);
   
   setCurrentFile("");
}

MainWindow::~MainWindow() {
  // delete widgets here 
}

void MainWindow::timerEvent() {
  Scene *sce = Scene::get();
  sce->setCurrentFrame((sce->currentFrame()+1)%sce->nbFrames());
  _drawingWidget->frameChanged();
  _toolsWidget->updateFrameNumber();
  refresh();
}


void MainWindow::createActions() {
  _newAct = new QAction(tr("&New"), this);
  _newAct->setShortcuts(QKeySequence::New);
  _newAct->setStatusTip(tr("Create a new file"));
  connect(_newAct,SIGNAL(triggered()),this,SLOT(newFile()));

  _openAct = new QAction(tr("&Open..."), this);
  _openAct->setShortcuts(QKeySequence::Open);
  _openAct->setStatusTip(tr("Open an existing file"));
  connect(_openAct,SIGNAL(triggered()),this,SLOT(open()));

  _saveAct = new QAction(tr("&Save"), this);
  _saveAct->setShortcuts(QKeySequence::Save);
  _saveAct->setStatusTip(tr("Save the document to disk"));
  connect(_saveAct,SIGNAL(triggered()),this,SLOT(save()));

  _exitAct = new QAction(tr("E&xit"), this);
  _exitAct->setShortcuts(QKeySequence::Quit);
  _exitAct->setStatusTip(tr("Exit the application"));
  connect(_exitAct,SIGNAL(triggered()),this,SLOT(closeAppli()));

  _copyAct = new QAction(tr("Copy"), this);
  _copyAct->setShortcut(QKeySequence::Copy);
  _copyAct->setStatusTip(tr("Copy selection"));
  connect(_copyAct,SIGNAL(triggered()),this,SLOT(copy()));

  _pasteAct = new QAction(tr("Paste"), this);
  _pasteAct->setShortcut(QKeySequence::Paste);
  _pasteAct->setStatusTip(tr("Paste selection"));
  connect(_pasteAct,SIGNAL(triggered()),this,SLOT(paste()));

  _selectAllAct = new QAction(tr("Select all"), this);
  _selectAllAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
  _selectAllAct->setStatusTip(tr("Select/unselect all nodes"));
  connect(_selectAllAct,SIGNAL(triggered()),this,SLOT(selectAll()));

#ifdef __APPLE__
  // Workaround on Mac to avoid Qt bug (copy, paste, selectAll disabled) when opening QFileDialog
  // https://forum.qt.io/topic/51950/qt-mac-osx-few-menu-items-are-disabled-after-qfiledialog
  _copyAct->setMenuRole(QAction::NoRole);
  _pasteAct->setMenuRole(QAction::NoRole);
  _selectAllAct->setMenuRole(QAction::NoRole);
#endif

  _helpAct = new QAction(tr("&Help"), this);
  _helpAct->setStatusTip(tr("Show the application's Help"));
  connect(_helpAct,SIGNAL(triggered()),this,SLOT(help()));

  _aboutAct = new QAction(tr("&About"), this);
  _aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(_aboutAct,SIGNAL(triggered()),this,SLOT(about()));

  Qt::Modifier frameNavModifier;
#ifdef __APPLE__
  // specific shortcut on OS X
  frameNavModifier = Qt::CTRL;
#else
  frameNavModifier = Qt::SHIFT;
#endif

  _nextFrameAct = new QAction(tr("Next Frame"), this);
  _nextFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Right));
  _nextFrameAct->setStatusTip(tr("Increment current frame"));
  connect(_nextFrameAct,SIGNAL(triggered()),this,SLOT(nextFrame()));

  _prevFrameAct = new QAction(tr("Prev Frame"), this);
  _prevFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Left));
  _prevFrameAct->setStatusTip(tr("Decrement current frame"));
  connect(_prevFrameAct,SIGNAL(triggered()),this,SLOT(prevFrame()));

  _firstFrameAct = new QAction(tr("First Frame"), this);
  _firstFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Up));
  _firstFrameAct->setStatusTip(tr("Cursor at the first frame"));
  connect(_firstFrameAct,SIGNAL(triggered()),this,SLOT(firstFrame()));

  _lastFrameAct = new QAction(tr("Last Frame"), this);
  _lastFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Down));
  _lastFrameAct->setStatusTip(tr("Cursor at the last frame"));
  connect(_lastFrameAct,SIGNAL(triggered()),this,SLOT(lastFrame()));

  _playAct = new QAction(tr("Play"), this);
  _playAct->setShortcut(QKeySequence(Qt::Key_P));
  _playAct->setStatusTip(tr("Start animation"));
  connect(_playAct,SIGNAL(triggered()),this,SLOT(play()));

  _stopAct = new QAction(tr("Stop"), this);
  _stopAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_P));
  _stopAct->setStatusTip(tr("Stop animation"));
  connect(_stopAct,SIGNAL(triggered()),this,SLOT(stop()));

  _settingsAct = new QAction(tr("Anim. Settings"), this);
  _settingsAct->setStatusTip(tr("Set range and framerate"));
  connect(_settingsAct,SIGNAL(triggered()),this,SLOT(settings()));
}

void MainWindow::createMenus() {
  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(_newAct);
  _fileMenu->addAction(_openAct);
  _fileMenu->addAction(_saveAct);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_exitAct);
  
  _editMenu = menuBar()->addMenu(tr("&Edit"));
  _editMenu->addAction(_copyAct);
  _editMenu->addAction(_pasteAct);
  _editMenu->addAction(_selectAllAct);
  _editMenu->addSeparator();
  _editMenu->addAction(_settingsAct);

  _animMenu = menuBar()->addMenu(tr("&Animate"));
  _animMenu->addAction(_playAct);
  _animMenu->addAction(_stopAct);
  _animMenu->addSeparator();
  _animMenu->addAction(_nextFrameAct);
  _animMenu->addAction(_prevFrameAct);
  _animMenu->addAction(_firstFrameAct);
  _animMenu->addAction(_lastFrameAct);

  _helpMenu = menuBar()->addMenu(tr("&Help"));
  _helpMenu->addAction(_helpAct);
  _helpMenu->addAction(_aboutAct);
 }

void MainWindow::newFile() {
  if(maybeSave()) {
    clearAll();
    setCurrentFile("");
  }
}

void MainWindow::open() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

bool MainWindow::save() {
  if(_currentFile.isEmpty()) {
    return saveAs();
  } else {
    cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
    return false;
  }
}

bool MainWindow::saveAs() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;

  return false;
}

void MainWindow::copy() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;  
}

void MainWindow::paste() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::selectAll() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::help() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::about() {
  QString h = tr("<center><font size='12'> YOUR APPLICATION NAME (VERSION...)</font></center><br>"
		 "Description<br><br>"
		 "Copyright (C) YEAR: <br>"
		 "EXAMPLE:<br>"
		 "<a href='http://maverick.inria.fr/~Romain.Vergne/'>Romain Vergne</a> "
		 "( <a href='mailto:romain.vergne@inria.fr'>romain.vergne@inria.fr</a> ) <br>"
		 "OTHER STUDENTS...<br>"
		 );
		 
  QMessageBox::about(this,"About",h);
}

bool MainWindow::closeAppli() {
  return close();
}

void MainWindow::nextFrame() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::prevFrame() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::firstFrame() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::lastFrame() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::play() {

}

void MainWindow::stop() {

}

void MainWindow::settings() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
  // AnimationSettings *w = new AnimationSettings(_graphWidget);
  // w->show();
}

bool MainWindow::maybeSave() {
  // QMessageBox::StandardButton ret;
  // ret = QMessageBox::warning(this,tr("Question"),tr("Do you want to save your scene?"),
  // 			     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  // if(ret==QMessageBox::Save)
  //   return save();
  // else //if(ret==QMessageBox::Cancel)
  //   return false;
  return true;
}

void MainWindow::clearAll() {
  cout << __FILE__ << " - " << __FUNCTION__ << ": TODO!" << endl;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if(maybeSave()) {
    clearAll();
    event->accept();
  } else {
    event->ignore();
  }
}

void MainWindow::setCurrentFile(const QString &fileName) {
  _currentFile = fileName;
  QString shownName = _currentFile;
  if(_currentFile.isEmpty())
    shownName = _DEFAULT_FILE_NAME+_APPLICATION_EXTENSION;
  setWindowFilePath(shownName);
}

void MainWindow::playAnimation() {
  if(!Scene::get()->isAnimated()) {
    _timer->setInterval((int)(1000.0f/(float)Scene::get()->fps()));
    _timer->start();
    Scene::get()->setAnimated(true);
  }
}

void MainWindow::stopAnimation() {
  if(Scene::get()->isAnimated()) {
    _timer->stop();
    Scene::get()->setAnimated(false);
  }
}

void MainWindow::sceneSizeChanged() {
  Scene *sce = Scene::get();
  _drawingWidget->setResol(sce->width(),sce->height());
}

void MainWindow::nbFramesChanged() {
  _animationWidget->resetResol();
  _animationWidget->refresh();
}

void MainWindow::fpsChanged() {
  _timer->setInterval((int)(1000.0f/(float)Scene::get()->fps()));
}
