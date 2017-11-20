#include <QApplication>
#include <QDesktopWidget>
#include "mainWidget.h"

int main(int argc,char *argv[]) {
  QApplication a(argc, argv);
  MainWindow window;
  
  const int div = 5;
  const QRect screen = QApplication::desktop()->screenGeometry();
  int w = screen.width()-screen.width()/div;
  int h = screen.height()-screen.height()/div;
  window.setGeometry(0,0,w,h);
  window.move(screen.center()-window.rect().center());
  window.show();
  
  return a.exec();
}
