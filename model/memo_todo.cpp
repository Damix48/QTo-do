#include "memo_todo.h"

#include "item.h"
#include "memo.h"
#include "todo.h"

MemoToDo::MemoToDo(std::string title_, std::string description_, bool checked_) : Item(title_),
                                                                                  Memo(title_, description_),
                                                                                  ToDo(title_, checked_) {}

Item::Type MemoToDo::getType() const {
  return Item::Type::MemoToDo;
}

MemoToDo::~MemoToDo() = default;
