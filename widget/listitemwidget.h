#ifndef LISTITEMWIDGET_H
#define LISTITEMWIDGET_H

#include <QButtonGroup>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "itemwidget.h"

class ListItemWidget : public QWidget {
  Q_OBJECT
 private:
  QVBoxLayout *mainLayout;
  QScrollArea *area;
  QWidget *widget;
  QVBoxLayout *listLayout;
  QButtonGroup *listItems;

  void update();

 public:
  explicit ListItemWidget(QWidget *parent = nullptr);
  void addItemWidget(int, ItemWidget *);

 signals:
  void sendItemType(Item::Type);
  void sendItemIndexSelected(int);
  void sendItemIndexChechBoxToggled(int);

 private slots:
  void onItemWidgetClicked(Item::Type);
  void onItemWidgetCheckBoxToggled();

 public slots:
  void receivedToDoInformation(int, QString, bool);
  void receivedMemoInformation(int, QString, QString);
  void receivedReminderInformation(int, QString, QDate);
  void receivedMemoToDoInformation(int, QString, QString, bool);
  void receivedToDoReminderInformation(int, QString, QDate, bool);
  void onReceiveDeselect();

  void onDeleteItem(int);
};

#endif  // LISTITEMWIDGET_H
