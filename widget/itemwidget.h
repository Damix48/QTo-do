#ifndef ITEM_WIDGET_H
#define ITEM_WIDGET_H

#include <QAbstractButton>
#include <QCheckBox>
#include <QDate>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

#include "./model/item.h"
// #include <QWidget>

class ItemWidget : public QAbstractButton {
  Q_OBJECT
 private:
  void paintEvent(QPaintEvent *event) override;

 protected:
  QString title;

  QLabel *titleLabel;
  QHBoxLayout *mainLayout;
  QVBoxLayout *layout;

  void setCommon();
  void setTitle();

  ItemWidget(QString, QWidget *parent = nullptr);

 public:
  void setSelected(bool = true);  //?

 signals:
  void itemSelected(Item::Type);

 private slots:
  virtual void onItemSelected() = 0;
};

/* TODO */
class ItemToDoWidget : public ItemWidget {
  Q_OBJECT
 protected:
  QCheckBox *checkbox;
  bool checked;
  bool changed;

  virtual void strikethrough();

 public:
  ItemToDoWidget(QString, bool = false, QWidget *parent = nullptr);

  bool isCheckBoxChecked();
  bool isCheckBoxChanged();

  QString getTitle() const;

 signals:
  void checkBoxToggled();

 private slots:
  void onItemSelected();

 protected slots:
  void onCheckBoxClicked();
};

/* MEMO */
class ItemMemoWidget : public ItemWidget {
  Q_OBJECT
 private:
  QString description;

  QLabel *descriptionLabel;

 public:
  ItemMemoWidget(QString, QString, QWidget *parent = nullptr);

 private slots:
  void onItemSelected();
};

/* REMINDER */
class ItemReminderWidget : public ItemWidget {
  Q_OBJECT
 private:
  QDate date;

  QLabel *dateLabel;

 public:
  ItemReminderWidget(QString, QDate, QWidget *parent = nullptr);

 private slots:
  void onItemSelected();
};

/* TODOREMINDER */
class ItemToDoReminderWidget : public ItemToDoWidget {
  Q_OBJECT
 private:
  QDate date;

  QLabel *dateLabel;

  void strikethrough();

 public:
  ItemToDoReminderWidget(QString, QDate, bool = false, QWidget *parent = nullptr);

 private slots:
  void onItemSelected();
};

/* MEMOTODO */
class ItemMemoToDoWidget : public ItemToDoWidget {
  Q_OBJECT
 private:
  QString description;

  QLabel *descriptionLabel;

  void strikethrough();

 public:
  ItemMemoToDoWidget(QString, QString, bool = false, QWidget *parent = nullptr);

 private slots:
  void onItemSelected();
};

#endif  // ITEM_WIDGET_H