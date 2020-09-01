#ifndef TODO_REMINDER_H
#define TODO_REMINDER_H

#include <string>

#include "reminder.h"
#include "todo.h"

class ToDoReminder : public ToDo, public Reminder {
 private:
 public:
  ToDoReminder(std::string, int, int, int, bool = false);

  Item::Type getType() const override;

  ~ToDoReminder() override;
};

#endif  // TODO_REMINDER_H