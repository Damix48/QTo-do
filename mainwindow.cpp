#include "mainwindow.h"

#include <QFile>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>

#include "./model/item.h"
#include "./widget/editpagewidget.h"
#include "./widget/listitemwidget.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent),
                                          mainLayout(new QVBoxLayout(this)),
                                          layout(new QHBoxLayout()),
                                          listItems(new ListItemWidget()),
                                          editPage(new EditPageWidget()) {
  QFile file(":/resource/style.css");
  file.open(QFile::ReadOnly);
  QString style = QLatin1String(file.readAll());
  setStyleSheet(style);

  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);

  layout->setMargin(0);

  mainLayout->addLayout(layout);

  layout->addWidget(listItems);
  layout->addWidget(editPage);

  layout->setStretch(0, 3);
  layout->setStretch(1, 2);

  setMinimumSize(QSize(640, 640));

  setWindowTitle("QTo-do");
}

ListItemWidget* MainWindow::getListItemWidget() const {
  return listItems;
}

EditPageWidget* MainWindow::getEditPageWidget() const {
  return editPage;
}

void MainWindow::onReminderDialog(QString title_, QDate date_) {
  QMessageBox msgBox;
  msgBox.setWindowTitle("Reminder expired");
  msgBox.setText(title_ + " is expired" + (date_ == QDate::currentDate() ? " today" : " " + QString::number(date_.daysTo(QDate::currentDate())) + " days ago"));
  msgBox.exec();
}

MainWindow::~MainWindow() {
}
