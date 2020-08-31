#include "itemwidget.h"

#include <QAbstractButton>
#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSizePolicy>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"

ItemWidget::ItemWidget(QString title_, QWidget *parent) : title(title_),
                                                          titleLabel(new QLabel),
                                                          mainLayout(new QHBoxLayout(parent)),
                                                          layout(new QVBoxLayout()) {}

void ItemWidget::setCommon() {
  QAbstractButton::setCheckable(true);
  mainLayout->addLayout(layout);

  mainLayout->setStretch(0, 0);
  mainLayout->setStretch(1, 1);

  connect(this, SIGNAL(clicked()), this, SLOT(onItemSelected()));

  setLayout(mainLayout);
}

void ItemWidget::setTitle() {
  titleLabel->setObjectName("title");
  titleLabel->setText(title);
  layout->addWidget(titleLabel);
}

void ItemWidget::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.init(this);
  opt.state |= isChecked() ? QStyle::State_On : QStyle::State_Off;

  QPainter p(this);

  setContentsMargins(4, 4, 4, 4);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  QWidget::paintEvent(event);
}

void ItemWidget::setSelected(bool sel) {
  setChecked(sel);
  onItemSelected();
}

/* TODO */
ItemToDoWidget::ItemToDoWidget(QString title_, bool checked_, QWidget *parent) : ItemWidget(title_, parent),
                                                                                 checkbox(new QCheckBox()),
                                                                                 checked(checked_),
                                                                                 changed(false) {
  setTitle();

  connect(checkbox, SIGNAL(clicked()), this, SLOT(onCheckBoxClicked()));

  checkbox->setChecked(checked);  //CONTROLLARE
  mainLayout->addWidget(checkbox);

  strikethrough();

  setCommon();
}

void ItemToDoWidget::onItemSelected() {
  if (isChecked()) {
    emit itemSelected(Item::Type::ToDo);
  }
}

void ItemToDoWidget::onCheckBoxClicked() {
  changed = true;
  checked = checkbox->isChecked();
  strikethrough();
  emit checkBoxToggled();
}

void ItemToDoWidget::strikethrough() {
  QFont f = titleLabel->font();
  f.setStrikeOut(checked);
  titleLabel->setFont(f);
}

bool ItemToDoWidget::isCheckBoxChecked() {
  return checked;
}

bool ItemToDoWidget::isCheckBoxChanged() {
  bool temp = changed;
  changed = false;
  return temp;
}

/* MEMO */
ItemMemoWidget::ItemMemoWidget(QString title_, QString description_, QWidget *parent) : ItemWidget(title_, parent),
                                                                                        description(description_),
                                                                                        descriptionLabel(new QLabel()) {
  setTitle();

  descriptionLabel->setObjectName("description");
  if (description.trimmed() == "") {
    descriptionLabel->setText("<i>No description</i>");
  } else {
    descriptionLabel->setText(description);
  }
  layout->addWidget(descriptionLabel);

  setCommon();
}

void ItemMemoWidget::onItemSelected() {
  if (isChecked()) {
    emit itemSelected(Item::Type::Memo);
  }
}

/* REMINDER */
ItemReminderWidget::ItemReminderWidget(QString title_, QDate date_, QWidget *parent) : ItemWidget(title_, parent),
                                                                                       date(date_),
                                                                                       dateLabel(new QLabel) {
  setTitle();

  dateLabel->setObjectName("date");
  dateLabel->setText(date.toString("dddd dd MMMM yyyy"));
  layout->addWidget(dateLabel);

  setCommon();
}

void ItemReminderWidget::onItemSelected() {
  if (isChecked()) {
    emit itemSelected(Item::Type::Reminder);
  }
}

/* TODOREMINDER */
ItemToDoReminderWidget::ItemToDoReminderWidget(QString title_, QDate date_, bool checked_, QWidget *parent) : ItemToDoWidget(title_, checked_, parent),
                                                                                                              date(date_),
                                                                                                              dateLabel(new QLabel()) {
  dateLabel->setObjectName("date");
  dateLabel->setText(date.toString("dddd dd MMMM yyyy"));
  layout->addWidget(dateLabel);

  checkbox->setChecked(checked_);
  strikethrough();
}

void ItemToDoReminderWidget::onItemSelected() {
  if (isChecked()) {
    emit itemSelected(Item::Type::ToDoReminder);
  }
}

void ItemToDoReminderWidget::strikethrough() {
  ItemToDoWidget::strikethrough();

  QFont f = dateLabel->font();
  f.setStrikeOut(checked);
  dateLabel->setFont(f);
}

/* MEMOTODO */
ItemMemoToDoWidget::ItemMemoToDoWidget(QString title_, QString description_, bool checked_, QWidget *parent) : ItemToDoWidget(title_, checked_, parent),
                                                                                                               description(description_),
                                                                                                               descriptionLabel(new QLabel()) {
  setTitle();

  descriptionLabel->setObjectName("description");
  if (description.trimmed() == "") {
    descriptionLabel->setText("<i>No description</i>");
  } else {
    descriptionLabel->setText(description);
  }
  layout->addWidget(descriptionLabel);

  setCommon();
}

void ItemMemoToDoWidget::onItemSelected() {
  if (isChecked()) {
    emit itemSelected(Item::Type::MemoToDo);
  }
}

void ItemMemoToDoWidget::strikethrough() {
  ItemToDoWidget::strikethrough();

  QFont f = descriptionLabel->font();
  f.setStrikeOut(checked);
  descriptionLabel->setFont(f);
}
