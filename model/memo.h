#ifndef MEMO_H
#define MEMO_H

#include <string>

#include "item.h"

class Memo : virtual public Item {
 private:
  std::string description;

 public:
  Memo(std::string, std::string);
  void changeDescription(std::string);
  std::string getBriefDescription() const;
  std::string getFullDescription() const;

  virtual Item::Type getType() const override;
};

#endif  // MEMO_H