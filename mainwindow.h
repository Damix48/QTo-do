#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QIcon>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"
#include "./widget/editpagewidget.h"
#include "./widget/listitemwidget.h"

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  QVBoxLayout* mainLayout;
  QHBoxLayout* layout;

  ListItemWidget* listItems;
  EditPageWidget* editPage;

 public:
  ListItemWidget* getListItemWidget() const;
  EditPageWidget* getEditPageWidget() const;

 public slots:
  void onReminderDialog(QString, QDate);
};
#endif  // MAINWINDOW_H
