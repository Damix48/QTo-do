#include "editpagewidget.h"

#include <QDateEdit>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"
#include "selectbuttongroup.h"

EditPageWidget::EditPageWidget(QWidget *parent) : QWidget(parent),
                                                  type(Item::None),
                                                  layout(new QVBoxLayout()),
                                                  headerLayout(new QHBoxLayout()),
                                                  headerLabel(new QLabel()),
                                                  addButton(new QPushButton()),
                                                  titleLabel(new QLabel()),
                                                  titleEdit(new QLineEdit()),
                                                  selectGroup(new SelectButtonGroup()),
                                                  descriptionLabel(new QLabel()),
                                                  descriptionEdit(new QTextEdit()),
                                                  dateLabel(new QLabel()),
                                                  dateEdit(new QDateEdit()),
                                                  buttonLayout(new QHBoxLayout()),
                                                  saveButton(new QPushButton()),
                                                  deleteButton(new QPushButton()) {
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  headerLabel->setObjectName("header");
  headerLabel->setText("Edit Item");
  headerLayout->addWidget(headerLabel);

  addButton->setObjectName("add_b");
  addButton->setText("+  Add new");
  headerLayout->addWidget(addButton);

  headerLayout->setStretch(0, 1);
  headerLayout->setStretch(1, 0);

  layout->addLayout(headerLayout);

  titleLabel->setObjectName("title");
  titleLabel->setText("Title");
  layout->addWidget(titleLabel);

  titleEdit->setObjectName("titleEdit");
  titleEdit->setPlaceholderText("Title");
  layout->addWidget(titleEdit);

  layout->addWidget(selectGroup);

  descriptionLabel->setObjectName("description");
  descriptionLabel->setText("Description");
  layout->addWidget(descriptionLabel);

  descriptionEdit->setObjectName("descriptionEdit");
  descriptionEdit->setPlaceholderText("Description");
  layout->addWidget(descriptionEdit);

  dateLabel->setObjectName("date");
  dateLabel->setText("Date");
  layout->addWidget(dateLabel);

  dateEdit->setCalendarPopup(true);
  dateEdit->setDisplayFormat("dd/MM/yyyy");
  dateEdit->setDate(QDate::currentDate());
  layout->addWidget(dateEdit);

  saveButton->setObjectName("save_b");
  saveButton->setText("Save");
  saveButton->setEnabled(false);
  buttonLayout->addWidget(saveButton);

  deleteButton->setObjectName("delete_b");
  deleteButton->setText("Delete");
  deleteButton->setEnabled(false);
  buttonLayout->addWidget(deleteButton);

  layout->addLayout(buttonLayout);

  hideAll();

  connect(addButton, SIGNAL(clicked()), this, SIGNAL(addNewItem()));
  connect(addButton, SIGNAL(clicked()), this, SLOT(onAdd_DeleteItem()));

  connect(saveButton, SIGNAL(clicked()), this, SLOT(onSaveButtonClicked()));

  connect(deleteButton, SIGNAL(clicked()), this, SIGNAL(deleteItem()));
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(onAdd_DeleteItem()));

  connect(this, SIGNAL(passItemType(Item::Type)), selectGroup, SLOT(onReceivedItemType(Item::Type)));
  connect(selectGroup, SIGNAL(onSelectionChanged(Item::Type)), this, SLOT(onReceivedType(Item::Type)));

  connect(titleEdit, SIGNAL(textChanged(QString)), this, SLOT(onEditTitleChanged()));
}

void EditPageWidget::onReceivedType(Item::Type type_) {
  type = type_;

  saveButton->setEnabled(true);
  onEditTitleChanged();

  qInfo() << type_;

  switch (type) {
    case Item::Type::ToDo:
      hideAll();
      break;

    case Item::Type::Memo:
      hideAll();
      descriptionLabel->show();
      descriptionEdit->show();
      break;

    case Item::Type::Reminder:
      hideAll();
      dateLabel->show();
      dateEdit->show();
      break;

    case Item::Type::MemoToDo:
      hideAll();
      descriptionLabel->show();
      descriptionEdit->show();
      break;

    case Item::Type::ToDoReminder:
      hideAll();
      dateLabel->show();
      dateEdit->show();
      break;

    default:
      hideAll();
      saveButton->setEnabled(false);
      break;
  }
}

void EditPageWidget::receivedTitleInformation(QString title_) {
  reset();
  deleteButton->setEnabled(true);

  titleEdit->setText(title_);
}

void EditPageWidget::receivedDescriptionInformation(QString description_) {
  descriptionEdit->setText(description_);
}

void EditPageWidget::receivedDateInformation(QDate date_) {
  dateEdit->setDate(date_);
}

void EditPageWidget::hideAll() {
  descriptionLabel->hide();
  descriptionEdit->hide();

  dateLabel->hide();
  dateEdit->hide();
}

void EditPageWidget::reset() {
  deleteButton->setEnabled(false);
  titleEdit->setText("");
  descriptionEdit->setText("");
  dateEdit->setDate(QDate::currentDate());
}

void EditPageWidget::onSaveButtonClicked() {
  if (titleEdit->text().trimmed() != "") {
    switch (type) {
      case Item::Type::ToDo:
        emit saveToDo(titleEdit->text().trimmed());
        break;

      case Item::Type::Memo:
        emit saveMemo(titleEdit->text().trimmed(), descriptionEdit->toPlainText().trimmed());
        break;

      case Item::Type::Reminder:
        emit saveReminder(titleEdit->text().trimmed(), dateEdit->date());
        break;

      case Item::Type::MemoToDo:
        emit saveMemoToDo(titleEdit->text().trimmed(), descriptionEdit->toPlainText().trimmed());
        break;

      case Item::Type::ToDoReminder:
        emit saveToDoReminder(titleEdit->text().trimmed(), dateEdit->date());
        break;

      default:
        break;
    }
  }
}

void EditPageWidget::onAdd_DeleteItem() {
  reset();
  onReceivedType(Item::Type::None);
  emit passItemType(Item::Type::None);
}

void EditPageWidget::onEditTitleChanged() {
  if (titleEdit->text().trimmed() == "") {
    saveButton->setEnabled(false);
  } else if (type != Item::Type::None) {
    saveButton->setEnabled(true);
  }
}
