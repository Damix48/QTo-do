#include "todo.h"

ToDo::ToDo(std::string title_, bool checked_) : Item(title_),
                                                checked(checked_) {}

void ToDo::toggle() {
  checked = !checked;
}

bool ToDo::getChecked() const {
  return checked;
}

Item::Type ToDo::getType() const {
  return Item::Type::ToDo;
}

// ToDo::~ToDo() = default;
