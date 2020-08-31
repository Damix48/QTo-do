#include "todo.h"

ToDo::ToDo(std::string title_, bool checked_) : Item(title_), checked(checked_) {}

void ToDo::toggle() {
  checked = !checked;
}

bool ToDo::getChecked() const {
  return checked;
}

ToDo* ToDo::clone() {
  return new ToDo(*this);
}

Item::Type ToDo::getType() const {
  return Item::Type::ToDo;
}

ToDo::~ToDo() = default;
