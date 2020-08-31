#ifndef MEMOTODO_H
#define MEMOTODO_H

#include <string>

#include "memo.h"
#include "todo.h"

class MemoToDo : public Memo, public ToDo {
 private:
 public:
  MemoToDo(std::string, std::string, bool = false);

  MemoToDo* clone() override;

  Item::Type getType() const override;

  ~MemoToDo() override;
};

#endif  // MEMOTODO_H