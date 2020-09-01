#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

class Item {
 private:
  std::string title;

 protected:
  Item(std::string);

 public:
  enum Type {
    None = 0,
    ToDo = 1,
    Memo = 2,
    Reminder = 3,
    ToDoReminder = 4,
    MemoToDo = 5
  };

  void changeTitle(std::string);
  std::string getTitle() const;

  virtual Item::Type getType() const = 0;

  Item& operator=(const Item&) = default;  //warning: defaulted move assignment for 'ToDo' calls a non-trivial move assignment operator for virtual base 'Item' [-Wvirtual-move-assign]

  virtual ~Item() = default;
};

#endif  //ITEM_H
