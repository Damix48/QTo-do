#include "listitemwidget.h"

#include <QButtonGroup>
#include <QDebug>
#include <QList>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "./model/item.h"
#include "itemwidget.h"

ListItemWidget::ListItemWidget(QWidget *parent) : QWidget(parent),
                                                  mainLayout(new QVBoxLayout()),
                                                  area(new QScrollArea()),
                                                  widget(new QWidget()),
                                                  listLayout(new QVBoxLayout()),
                                                  listItems(new QButtonGroup()) {
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);

  listItems = new QButtonGroup();

  area->setWidgetResizable(true);
  area->setWidget(widget);

  widget->setObjectName("listArea2");
  widget->setLayout(listLayout);

  listLayout->setAlignment(Qt::AlignTop);
  listLayout->setMargin(4);
  listLayout->setSpacing(4);
  listLayout->setObjectName("listLayout2");

  listItems->setExclusive(true);

  mainLayout->addWidget(area);

  setLayout(mainLayout);
}

void ListItemWidget::addItemWidget(int index_, ItemWidget *item) {
  item->setParent(widget);

  connect(item, SIGNAL(itemSelected(Item::Type)), this, SLOT(onItemWidgetClicked(Item::Type)));

  if (dynamic_cast<ItemToDoWidget *>(item) != nullptr) {
    connect(item, SIGNAL(checkBoxToggled()), this, SLOT(onItemWidgetCheckBoxToggled()));
  }

  if (listItems->button(index_) != nullptr) {
    QAbstractButton *b = listItems->button(index_);
    listItems->removeButton(b);
    delete b;
  }

  listItems->addButton(item, index_);
  listLayout->insertWidget(listItems->buttons().size() - 1 - index_, item);
  listItems->button(index_)->setChecked(true);
}

void ListItemWidget::onItemWidgetClicked(Item::Type type_) {
  qInfo() << "id: " << listItems->checkedId();
  emit sendItemIndexSelected(listItems->checkedId());

  emit sendItemType(type_);
}

void ListItemWidget::onItemWidgetCheckBoxToggled() {
  QList<QAbstractButton *> list_ = listItems->buttons();

  for (QList<QAbstractButton *>::iterator it = list_.begin(); it != list_.end(); ++it) {
    ItemToDoWidget *temp = dynamic_cast<ItemToDoWidget *>(*it);
    if (temp != nullptr && temp->isCheckBoxChanged()) {
      // qInfo() << listItems->id((*it));
      emit sendItemIndexChechBoxToggled(listItems->id((*it)));
    }
  }
}

void ListItemWidget::receivedToDoInformation(int index_, QString title_, bool checked_) {
  addItemWidget(index_, new ItemToDoWidget(title_, checked_));
}

void ListItemWidget::receivedMemoInformation(int index_, QString title_, QString description_) {
  addItemWidget(index_, new ItemMemoWidget(title_, description_));
}

void ListItemWidget::receivedReminderInformation(int index_, QString title_, QDate date_) {
  addItemWidget(index_, new ItemReminderWidget(title_, date_));
}

void ListItemWidget::receivedMemoToDoInformation(int index_, QString title_, QString description_, bool checked_) {
  addItemWidget(index_, new ItemMemoToDoWidget(title_, description_, checked_));
}

void ListItemWidget::receivedToDoReminderInformation(int index_, QString title_, QDate date_, bool checked_) {
  addItemWidget(index_, new ItemToDoReminderWidget(title_, date_, checked_));
}

void ListItemWidget::update() {
}

void ListItemWidget::onReceiveDeselect() {
  if (listItems->checkedButton()) {
    listItems->setExclusive(false);
    listItems->checkedButton()->setChecked(false);
    listItems->setExclusive(true);
  }
}

void ListItemWidget::onDeleteItem(int index_) {
  QAbstractButton *b = listItems->button(index_);
  listItems->removeButton(b);
  delete b;

  // Update id after delete
  QList<QAbstractButton *> list_ = listItems->buttons();
  for (QList<QAbstractButton *>::iterator it = list_.begin() + index_; it != list_.end(); ++it) {
    listItems->setId(*it, listItems->id(*it) - 1);
  }
}