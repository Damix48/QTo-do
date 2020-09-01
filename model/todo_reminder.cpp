#include "todo_reminder.h"

#include <string>

#include "item.h"
#include "reminder.h"
#include "todo.h"

ToDoReminder::ToDoReminder(std::string title_, int d, int m, int y, bool checked_) : Item(title_), ToDo(title_, checked_), Reminder(title_, d, m, y) {}

Item::Type ToDoReminder::getType() const {
  return Item::Type::ToDoReminder;
}

ToDoReminder::~ToDoReminder() = default;
