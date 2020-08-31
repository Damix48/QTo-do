#include "controller.h"

#include <QDate>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <typeinfo>
#include <vector>

#include "./model/model.h"
#include "mainwindow.h"
#include "myvector.h"

Controller::Controller(MainWindow* view_, QObject* parent) : QObject(parent),
                                                             view(view_),
                                                             selectedIndex(-1),
                                                             items(MyVector<Item*>()),
                                                             timer(new QTimer()) {
  openXML();

  connect(this, SIGNAL(sendDeselect()), view->getListItemWidget(), SLOT(onReceiveDeselect()));

  connect(view->getListItemWidget(), SIGNAL(sendItemIndexSelected(int)), this, SLOT(onReceiveItemIndexSelected(int)));
  connect(view->getListItemWidget(), SIGNAL(sendItemIndexChechBoxToggled(int)), this, SLOT(onReceiveItemIndexChechBoxToggled(int)));

  connect(view->getEditPageWidget(), SIGNAL(addNewItem()), this, SLOT(onAddNewItemButtonClicked()));
  connect(view->getEditPageWidget(), SIGNAL(deleteItem()), this, SLOT(onDeleteItemButtonClicked()));

  connect(this, SIGNAL(deleteItem(int)), view->getListItemWidget(), SLOT(onDeleteItem(int)));

  connect(this, SIGNAL(sendTitleInformation(QString)), view->getEditPageWidget(), SLOT(receivedTitleInformation(QString)));
  connect(this, SIGNAL(sendDescriptionInformation(QString)), view->getEditPageWidget(), SLOT(receivedDescriptionInformation(QString)));
  connect(this, SIGNAL(sendDateInformation(QDate)), view->getEditPageWidget(), SLOT(receivedDateInformation(QDate)));

  connect(this, SIGNAL(sendToDoInformation(int, QString, bool)), view->getListItemWidget(), SLOT(receivedToDoInformation(int, QString, bool)));
  connect(this, SIGNAL(sendMemoInformation(int, QString, QString)), view->getListItemWidget(), SLOT(receivedMemoInformation(int, QString, QString)));
  connect(this, SIGNAL(sendReminderInformation(int, QString, QDate)), view->getListItemWidget(), SLOT(receivedReminderInformation(int, QString, QDate)));
  connect(this, SIGNAL(sendMemoToDoInformation(int, QString, QString, bool)), view->getListItemWidget(), SLOT(receivedMemoToDoInformation(int, QString, QString, bool)));
  connect(this, SIGNAL(sendToDoReminderInformation(int, QString, QDate, bool)), view->getListItemWidget(), SLOT(receivedToDoReminderInformation(int, QString, QDate, bool)));

  connect(this, SIGNAL(sendReminderDialog(QString, QDate)), view, SLOT(onReminderDialog(QString, QDate)));

  connect(view->getEditPageWidget(), SIGNAL(saveToDo(QString)), this, SLOT(onReceivedSavedToDo(QString)));
  connect(view->getEditPageWidget(), SIGNAL(saveMemo(QString, QString)), this, SLOT(onReceivedSavedMemo(QString, QString)));
  connect(view->getEditPageWidget(), SIGNAL(saveReminder(QString, QDate)), this, SLOT(onReceivedSavedReminder(QString, QDate)));
  connect(view->getEditPageWidget(), SIGNAL(saveMemoToDo(QString, QString)), this, SLOT(onReceivedSavedMemoToDo(QString, QString)));
  connect(view->getEditPageWidget(), SIGNAL(saveToDoReminder(QString, QDate)), this, SLOT(onReceivedSavedToDoReminder(QString, QDate)));

  connect(this, SIGNAL(sendItemType(Item::Type)), view->getEditPageWidget(), SIGNAL(passItemType(Item::Type)));

  sendItems();
  emit sendDeselect();

  connect(timer, SIGNAL(timeout()), this, SLOT(checkReminder()));
  timer->start(QDateTime::currentDateTime().msecsTo(QDateTime(QDate::currentDate().addDays(1), QTime(0, 1))));
}

void Controller::onReceiveItemIndexSelected(int index_) {
  selectedIndex = index_;
  if (index_ != -1) {
    sendItemSelected();
  }
}

void Controller::itemSender(Item* item_, int index_, SendTo edit) {
  switch (item_->getType()) {
    case Item::Type::ToDo: {
      ToDo* temp_ = dynamic_cast<ToDo*>(item_);
      if (edit == SendTo::Edit || edit == SendTo::Edit_List) {
        emit sendTitleInformation(QString::fromStdString(temp_->getTitle()));

        emit sendItemType(temp_->getType());
      }
      if (edit == SendTo::List || edit == SendTo::Edit_List) {
        emit sendToDoInformation(index_, QString::fromStdString(temp_->getTitle()), temp_->getChecked());
      }
      break;
    }

    case Item::Type::Memo: {
      Memo* temp_ = dynamic_cast<Memo*>(item_);
      if (edit == SendTo::Edit || edit == SendTo::Edit_List) {
        emit sendTitleInformation(QString::fromStdString(temp_->getTitle()));
        emit sendDescriptionInformation(QString::fromStdString(temp_->getFullDescription()));

        emit sendItemType(temp_->getType());
      }
      if (edit == SendTo::List || edit == SendTo::Edit_List) {
        emit sendMemoInformation(index_, QString::fromStdString(temp_->getTitle()), QString::fromStdString(temp_->getBriefDescription()));
      }
      break;
    }

    case Item::Type::Reminder: {
      Reminder* temp_ = dynamic_cast<Reminder*>(item_);
      if (edit == SendTo::Edit || edit == SendTo::Edit_List) {
        emit sendTitleInformation(QString::fromStdString(temp_->getTitle()));
        emit sendDateInformation(convertDate(temp_->getDate()));

        emit sendItemType(temp_->getType());
      }
      if (edit == SendTo::List || edit == SendTo::Edit_List) {
        emit sendReminderInformation(index_, QString::fromStdString(temp_->getTitle()), convertDate(temp_->getDate()));
      }
      break;
    }

    case Item::Type::MemoToDo: {
      MemoToDo* temp_ = dynamic_cast<MemoToDo*>(item_);
      if (edit == SendTo::Edit || edit == SendTo::Edit_List) {
        emit sendTitleInformation(QString::fromStdString(temp_->getTitle()));
        emit sendDescriptionInformation(QString::fromStdString(temp_->getFullDescription()));

        emit sendItemType(temp_->getType());
      }
      if (edit == SendTo::List || edit == SendTo::Edit_List) {
        emit sendMemoToDoInformation(index_, QString::fromStdString(temp_->getTitle()), QString::fromStdString(temp_->getBriefDescription()), temp_->getChecked());
      }
      break;
    }

    case Item::Type::ToDoReminder: {
      ToDoReminder* temp_ = dynamic_cast<ToDoReminder*>(item_);
      if (edit == SendTo::Edit || edit == SendTo::Edit_List) {
        emit sendTitleInformation(QString::fromStdString(temp_->getTitle()));
        emit sendDateInformation(convertDate(temp_->getDate()));

        emit sendItemType(temp_->getType());
      }
      if (edit == SendTo::List || edit == SendTo::Edit_List) {
        emit sendToDoReminderInformation(index_, QString::fromStdString(temp_->getTitle()), convertDate(temp_->getDate()), temp_->getChecked());
      }
      break;
    }

    default:
      break;
  }
}

void Controller::sendItems() {
  // for (std::vector<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
  //   itemSender(*it, it - items.begin(), SendTo::List);
  // }
  int index = 0;
  for (MyVector<Item*>::Iterator it = items.begin(); it != items.end(); ++it) {
    itemSender(*it, index, SendTo::List);
    ++index;
  }
}

void Controller::sendItemSelected() {
  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit);
}

void Controller::onReceivedSavedToDo(QString title_) {
  if (selectedIndex == -1) {
    // items.push_back(new ToDo(title_.toStdString()));
    // selectedIndex = items.size() - 1;
    items.Push(new ToDo(title_.toStdString()));
    selectedIndex = items.GetSize() - 1;
  } else {
    if (items[selectedIndex]->getType() != Item::ToDo) {
      delete items[selectedIndex];
      items[selectedIndex] = new ToDo(title_.toStdString());

    } else {
      items[selectedIndex]->changeTitle(title_.toStdString());
    }
  }

  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit_List);
  saveXML();
}

void Controller::onReceivedSavedMemo(QString title_, QString description_) {
  if (selectedIndex == -1) {
    // items.push_back(new Memo(title_.toStdString(), description_.toStdString()));
    // selectedIndex = items.size() - 1;
    items.Push(new Memo(title_.toStdString(), description_.toStdString()));
    selectedIndex = items.GetSize() - 1;
  } else {
    if (items[selectedIndex]->getType() != Item::Memo) {
      delete items[selectedIndex];
      items[selectedIndex] = new Memo(title_.toStdString(), description_.toStdString());

    } else {
      items[selectedIndex]->changeTitle(title_.toStdString());
      dynamic_cast<Memo*>(items[selectedIndex])->changeDescription(description_.toStdString());
    }
  }

  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit_List);
  saveXML();
}

void Controller::onReceivedSavedReminder(QString title_, QDate date_) {
  if (selectedIndex == -1) {
    // items.push_back(new Reminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
    // selectedIndex = items.size() - 1;
    items.Push(new Reminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
    selectedIndex = items.GetSize() - 1;
  } else {
    if (items[selectedIndex]->getType() != Item::Reminder) {
      delete items[selectedIndex];
      items[selectedIndex] = new Reminder(title_.toStdString(), date_.day(), date_.month(), date_.year());

    } else {
      items[selectedIndex]->changeTitle(title_.toStdString());
      dynamic_cast<Reminder*>(items[selectedIndex])->changeDate(date_.day(), date_.month(), date_.year());
    }
  }

  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit_List);
  saveXML();
}

void Controller::onReceivedSavedMemoToDo(QString title_, QString description_) {
  if (selectedIndex == -1) {
    // items.push_back(new MemoToDo(title_.toStdString(), description_.toStdString()));
    // selectedIndex = items.size() - 1;
    items.Push(new MemoToDo(title_.toStdString(), description_.toStdString()));
    selectedIndex = items.GetSize() - 1;
  } else {
    if (items[selectedIndex]->getType() != Item::MemoToDo) {
      delete items[selectedIndex];
      items[selectedIndex] = new MemoToDo(title_.toStdString(), description_.toStdString());

    } else {
      items[selectedIndex]->changeTitle(title_.toStdString());
      dynamic_cast<MemoToDo*>(items[selectedIndex])->changeDescription(description_.toStdString());
    }
  }

  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit_List);
  saveXML();
}

void Controller::onReceivedSavedToDoReminder(QString title_, QDate date_) {
  if (selectedIndex == -1) {
    // items.push_back(new ToDoReminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
    // selectedIndex = items.size() - 1;
    items.Push(new ToDoReminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
    selectedIndex = items.GetSize() - 1;
  } else {
    if (items[selectedIndex]->getType() != Item::ToDoReminder) {
      delete items[selectedIndex];
      items[selectedIndex] = new ToDoReminder(title_.toStdString(), date_.day(), date_.month(), date_.year());

    } else {
      items[selectedIndex]->changeTitle(title_.toStdString());
      dynamic_cast<ToDoReminder*>(items[selectedIndex])->changeDate(date_.day(), date_.month(), date_.year());
    }
  }

  itemSender(items[selectedIndex], selectedIndex, SendTo::Edit_List);
  saveXML();
}

void Controller::onReceiveItemIndexChechBoxToggled(int index_) {
  dynamic_cast<ToDo*>(items[index_])->toggle();
  saveXML();
}

QDate Controller::convertDate(Date date_) {
  return QDate(date_.Year(), date_.Month(), date_.Day());
}

void Controller::onAddNewItemButtonClicked() {
  selectedIndex = -1;
  emit sendDeselect();
}

void Controller::onDeleteItemButtonClicked() {
  // delete items[selectedIndex];
  // items.erase(items.begin() + selectedIndex);
  items.Pop(selectedIndex);
  emit deleteItem(selectedIndex);
  selectedIndex = -1;
  saveXML();
}

void Controller::saveXML() {
  QFile file("./data.xml");

  if (!file.open(QFile::WriteOnly | QFile::Text))
    return;

  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();

  xmlWriter.writeStartElement("Items");

  // for (std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
  for (MyVector<Item*>::Iterator it = items.begin(); it != items.end(); ++it) {
    xmlWriter.writeStartElement("Item");
    xmlWriter.writeAttribute("type", convertType((*it)->getType()));

    xmlWriter.writeTextElement("title", QString::fromStdString((*it)->getTitle()));

    if (ToDo* temp_ = dynamic_cast<ToDo*>(*it)) {
      xmlWriter.writeTextElement("checked", temp_->getChecked() ? "true" : "false");
    }

    if (Memo* temp_ = dynamic_cast<Memo*>(*it)) {
      xmlWriter.writeTextElement("description", QString::fromStdString(temp_->getFullDescription()));
    }

    if (Reminder* temp_ = dynamic_cast<Reminder*>(*it)) {
      xmlWriter.writeTextElement("date", convertDate(temp_->getDate()).toString());
    }

    xmlWriter.writeEndElement();
  }

  xmlWriter.writeEndDocument();

  file.close();
}

void Controller::openXML() {
  QFile file("./data.xml");
  if (!file.open(QFile::ReadOnly | QFile::Text))
    return;

  QXmlStreamReader xmlReader(&file);

  if (xmlReader.readNextStartElement()) {
    if (xmlReader.name() == "Items") {
      while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == "Item") {
          switch (convertType(xmlReader.attributes().value("type").toString())) {
            case Item::Type::ToDo: {
              QString title_;
              bool checked_;

              while (xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "title") {
                  title_ = xmlReader.readElementText();
                } else if (xmlReader.name() == "checked") {
                  checked_ = xmlReader.readElementText() == "true";
                } else {
                  xmlReader.skipCurrentElement();
                }
              }
              // items.push_back(new ToDo(title_.toStdString(), checked_));
              items.Push(new ToDo(title_.toStdString(), checked_));
              break;
            }

            case Item::Type::Memo: {
              QString title_;
              QString description_;

              while (xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "title") {
                  title_ = xmlReader.readElementText();
                } else if (xmlReader.name() == "description") {
                  description_ = xmlReader.readElementText();
                } else {
                  xmlReader.skipCurrentElement();
                }
              }
              // items.push_back(new Memo(title_.toStdString(), description_.toStdString()));
              items.Push(new Memo(title_.toStdString(), description_.toStdString()));
              break;
            }

            case Item::Type::Reminder: {
              QString title_;
              QDate date_;

              while (xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "title") {
                  title_ = xmlReader.readElementText();
                } else if (xmlReader.name() == "date") {
                  date_ = QDate::fromString(xmlReader.readElementText());
                } else {
                  xmlReader.skipCurrentElement();
                }
              }
              // items.push_back(new Reminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
              items.Push(new Reminder(title_.toStdString(), date_.day(), date_.month(), date_.year()));
              break;
            }

            case Item::Type::MemoToDo: {
              QString title_;
              QString description_;
              bool checked_;

              while (xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "title") {
                  title_ = xmlReader.readElementText();
                } else if (xmlReader.name() == "checked") {
                  checked_ = xmlReader.readElementText() == "true";
                } else if (xmlReader.name() == "description") {
                  description_ = xmlReader.readElementText();
                } else {
                  xmlReader.skipCurrentElement();
                }
              }
              // items.push_back(new MemoToDo(title_.toStdString(), description_.toStdString(), checked_));
              items.Push(new MemoToDo(title_.toStdString(), description_.toStdString(), checked_));
              break;
            }

            case Item::Type::ToDoReminder: {
              QString title_;
              bool checked_;
              QDate date_;

              while (xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "title") {
                  title_ = xmlReader.readElementText();
                } else if (xmlReader.name() == "checked") {
                  checked_ = xmlReader.readElementText() == "true";
                } else if (xmlReader.name() == "date") {
                  date_ = QDate::fromString(xmlReader.readElementText());
                } else {
                  xmlReader.skipCurrentElement();
                }
              }
              // items.push_back(new ToDoReminder(title_.toStdString(), date_.day(), date_.month(), date_.year(), checked_));
              items.Push(new ToDoReminder(title_.toStdString(), date_.day(), date_.month(), date_.year(), checked_));
              break;
            }
            default:
              xmlReader.skipCurrentElement();
          }
        } else {
          xmlReader.skipCurrentElement();
        }
      }
    } else {
      xmlReader.raiseError(QObject::tr("Incorrect file"));
    }
  }

  file.close();
}

QString Controller::convertType(Item::Type type_) {
  switch (type_) {
    case Item::Type::ToDo:
      return "ToDo";

    case Item::Type::Memo:
      return "Memo";

    case Item::Type::Reminder:
      return "Reminder";

    case Item::Type::MemoToDo:
      return "MemoToDo";

    case Item::Type::ToDoReminder:
      return "ToDoReminder";

    default:
      return "None";
  }
}

Item::Type Controller::convertType(QString type_) {
  if (type_ == "ToDo")
    return Item::Type::ToDo;

  if (type_ == "Memo")
    return Item::Type::Memo;

  if (type_ == "Reminder")
    return Item::Type::Reminder;

  if (type_ == "MemoToDo")
    return Item::Type::MemoToDo;

  if (type_ == "ToDoReminder")
    return Item::Type::ToDoReminder;

  return Item::Type::None;
}

void Controller::checkReminder() {
  // for (std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
  for (MyVector<Item*>::Iterator it = items.begin(); it != items.end(); ++it) {
    if (Reminder* temp_ = dynamic_cast<Reminder*>(*it)) {
      if (temp_->passed()) {
        ToDo* temp__ = dynamic_cast<ToDo*>(*it);
        if (temp__) {
          if (!temp__->getChecked()) {
            emit sendReminderDialog(QString::fromStdString(temp_->getTitle()), convertDate(temp_->getDate()));
          }
        } else {
          emit sendReminderDialog(QString::fromStdString(temp_->getTitle()), convertDate(temp_->getDate()));
        }
      }
    }
  }

  timer->stop();
  timer->start(QDateTime::currentDateTime().msecsTo(QDateTime(QDate::currentDate().addDays(1), QTime(0, 1))));
}
