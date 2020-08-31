#ifndef EDITPAGEWIDGET_H
#define EDITPAGEWIDGET_H

#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"
#include "selectbuttongroup.h"

class EditPageWidget : public QWidget {
  Q_OBJECT
 private:
  Item::Type type;

  QVBoxLayout *layout;
  QHBoxLayout *headerLayout;
  QLabel *headerLabel;
  QPushButton *addButton;
  QLabel *titleLabel;
  QLineEdit *titleEdit;
  SelectButtonGroup *selectGroup;
  QLabel *descriptionLabel;
  QTextEdit *descriptionEdit;
  QLabel *dateLabel;
  QDateEdit *dateEdit;
  QHBoxLayout *buttonLayout;
  QPushButton *saveButton;
  QPushButton *deleteButton;

  void hideAll();
  void reset();

 public:
  explicit EditPageWidget(QWidget *parent = nullptr);

 signals:
  void passItemType(Item::Type);

  void addNewItem();
  void deleteItem();

  void saveToDo(QString);
  void saveMemo(QString, QString);
  void saveReminder(QString, QDate);
  void saveMemoToDo(QString, QString);
  void saveToDoReminder(QString, QDate);

 public slots:
  void onReceivedType(Item::Type);

  void receivedTitleInformation(QString);
  void receivedDescriptionInformation(QString);
  void receivedDateInformation(QDate);

  void onSaveButtonClicked();
  void onAdd_DeleteItem();

  void onEditTitleChanged();
};

#endif  // EDITPAGEWIDGET_H
