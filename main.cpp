#include <QApplication>

#include "controller.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  Controller* controller = new Controller(&w);
  w.show();

  controller->checkReminder();

  return a.exec();
}
