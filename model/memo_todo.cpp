#include "memo_todo.h"

#include "item.h"
#include "memo.h"
#include "todo.h"

MemoToDo::MemoToDo(std::string title_, std::string description_, bool checked_) : Item(title_),
                                                                                  Memo(title_, description_),
                                                                                  ToDo(title_, checked_) {}

MemoToDo* MemoToDo::clone() {
  return new MemoToDo(*this);
}

Item::Type MemoToDo::getType() const {
  return Item::Type::MemoToDo;
}

MemoToDo::~MemoToDo() = default;
