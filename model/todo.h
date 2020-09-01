#ifndef TODO_H
#define TODO_H

#include <string>

#include "item.h"

class ToDo : virtual public Item {
 private:
  bool checked;

 public:
  ToDo(std::string, bool = false);
  void toggle();

  bool getChecked() const;

  virtual Item::Type getType() const override;

  virtual ~ToDo() override;
};

#endif  // TODO_H