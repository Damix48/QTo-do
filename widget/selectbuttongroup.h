#ifndef SELECTBUTTONGROUP_H
#define SELECTBUTTONGROUP_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"
#include "selectbutton.h"

class SelectButtonGroup : public QWidget {
  Q_OBJECT
 private:
  QVBoxLayout* mainLayout;
  QHBoxLayout* layout;
  SelectButton* toDoButton;
  SelectButton* reminderButton;
  SelectButton* memoButton;
  QLabel* errorLabel;

  void reset();
  void setSelection(Item::Type);

 public:
  explicit SelectButtonGroup(QWidget* parent = nullptr);

 private slots:
  void update();

 public slots:
  void onReceivedItemType(Item::Type);

 signals:
  void onSelectionChanged(Item::Type);
};

#endif  // SELECTBUTTONGROUP_H
