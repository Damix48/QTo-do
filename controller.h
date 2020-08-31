#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <time.h>

#include <QDate>
#include <QObject>
#include <QString>
#include <QTimer>
#include <vector>

#include "./model/model.h"
#include "mainwindow.h"
#include "myvector.h"

class Controller : public QObject {
  Q_OBJECT
 private:
  enum SendTo {
    Edit_List = 0,
    Edit = 1,
    List = 2
  };

  MainWindow *view;

  // std::vector<Item *> items;
  MyVector<Item *> items;
  int selectedIndex;
  QTimer *timer;

  void
  itemSender(Item *, int, SendTo);
  void sendItemSelected();
  void sendItems();

  QDate convertDate(Date);
  QString convertType(Item::Type);
  Item::Type convertType(QString);

  void saveXML();
  void openXML();

 public:
  explicit Controller(MainWindow *, QObject *parent = nullptr);

 signals:
  void sendItemType(Item::Type);

  void sendTitleInformation(QString);
  void sendDescriptionInformation(QString);
  void sendDateInformation(QDate);

  void sendToDoInformation(int, QString, bool);
  void sendMemoInformation(int, QString, QString);
  void sendReminderInformation(int, QString, QDate);
  void sendMemoToDoInformation(int, QString, QString, bool);
  void sendToDoReminderInformation(int, QString, QDate, bool);
  void sendReminderDialog(QString, QDate);

  void sendDeselect();

  void deleteItem(int);

 public slots:
  void checkReminder();

  void onReceiveItemIndexSelected(int);
  void onReceiveItemIndexChechBoxToggled(int);

  void onAddNewItemButtonClicked();
  void onDeleteItemButtonClicked();

  void onReceivedSavedToDo(QString);
  void onReceivedSavedMemo(QString, QString);
  void onReceivedSavedReminder(QString, QDate);
  void onReceivedSavedMemoToDo(QString, QString);
  void onReceivedSavedToDoReminder(QString, QDate);
};

#endif  // CONTROLLER_H
