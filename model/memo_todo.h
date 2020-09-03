#ifndef MEMOTODO_H
#define MEMOTODO_H

#include <string>

#include "memo.h"
#include "todo.h"

class MemoToDo : public Memo, public ToDo {
 private:
 public:
  MemoToDo(std::string, std::string, bool = false);

  Item::Type getType() const override;
};

#endif  // MEMOTODO_H