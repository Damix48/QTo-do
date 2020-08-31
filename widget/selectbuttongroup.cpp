#include "selectbuttongroup.h"

#include <QHBoxLayout>
#include <QWidget>

#include "./model/item.h"

SelectButtonGroup::SelectButtonGroup(QWidget *parent) : QWidget(parent),
                                                        mainLayout(new QVBoxLayout()),
                                                        layout(new QHBoxLayout()),
                                                        toDoButton(new SelectButton("To Do", QIcon(":/resource/todo_outline.png"), QIcon(":/resource/todo.png"))),
                                                        reminderButton(new SelectButton("Reminder", QIcon(":/resource/reminder_outline.png"), QIcon(":/resource/reminder.png"))),
                                                        memoButton(new SelectButton("Memo", QIcon(":/resource/memo_outline.png"), QIcon(":/resource/memo.png"))),
                                                        errorLabel(new QLabel()) {
  layout->addWidget(toDoButton);
  layout->addWidget(reminderButton);
  layout->addWidget(memoButton);

  layout->setAlignment(Qt::AlignCenter);

  errorLabel->setText("Select a type");
  errorLabel->setObjectName("errorLabel");
  errorLabel->setAlignment(Qt::AlignCenter);

  mainLayout->addLayout(layout);
  mainLayout->addWidget(errorLabel);

  connect(toDoButton, SIGNAL(clicked()), this, SLOT(update()));
  connect(reminderButton, SIGNAL(clicked()), this, SLOT(update()));
  connect(memoButton, SIGNAL(clicked()), this, SLOT(update()));

  setLayout(mainLayout);
}

void SelectButtonGroup::update() {
  Item::Type temp = Item::Type::None;
  errorLabel->hide();

  if (toDoButton->isChecked()) {
    temp = Item::Type::ToDo;
  }

  if (reminderButton->isChecked()) {
    memoButton->setEnabled(false);
    temp = Item::Type::Reminder;
    if (toDoButton->isChecked()) {
      temp = Item::Type::ToDoReminder;
    }
  } else {
    memoButton->setEnabled(true);
  }

  if (memoButton->isChecked()) {
    reminderButton->setEnabled(false);
    temp = Item::Type::Memo;
    if (toDoButton->isChecked()) {
      temp = Item::Type::MemoToDo;
    }
  } else {
    reminderButton->setEnabled(true);
  }

  if (temp == Item::Type::None) {
    errorLabel->show();
  }
  // if (toDoButton->isChecked() || memoButton->isChecked() || reminderButton->isChecked()) {
  emit onSelectionChanged(temp);
  // }
}

void SelectButtonGroup::setSelection(Item::Type type_) {
  switch (type_) {
    case Item::Type::ToDo:
      reset();
      toDoButton->setChecked(true);
      break;

    case Item::Type::Memo:
      reset();
      memoButton->setChecked(true);
      break;

    case Item::Type::Reminder:
      reset();
      reminderButton->setChecked(true);
      break;

    case Item::Type::MemoToDo:
      reset();
      memoButton->setChecked(true);
      toDoButton->setChecked(true);
      break;

    case Item::Type::ToDoReminder:
      reset();
      reminderButton->setChecked(true);
      toDoButton->setChecked(true);
      break;

    default:
      reset();
      // toDoButton->setChecked(true);

      break;
  }

  update();
}

void SelectButtonGroup::reset() {
  toDoButton->setChecked(false);
  memoButton->setChecked(false);
  reminderButton->setChecked(false);

  toDoButton->setEnabled(true);
  memoButton->setEnabled(true);
  reminderButton->setEnabled(true);
}

void SelectButtonGroup::onReceivedItemType(Item::Type type_) {
  setSelection(type_);
}
