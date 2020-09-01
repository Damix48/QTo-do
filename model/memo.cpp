#include "memo.h"

Memo::Memo(std::string title_, std::string description_) : Item(title_), description(description_) {}

void Memo::changeDescription(std::string description_) {
  description = description_;
}

std::string Memo::getBriefDescription() const {
  std::string temp = description.substr(0, 30);
  std::size_t found = temp.find("\n");
  if (found != std::string::npos) {
    return temp.substr(0, found);
  } else {
    return temp;
  }
}

std::string Memo::getFullDescription() const {
  return description;
}

Item::Type Memo::getType() const {
  return Item::Type::Memo;
}

Memo::~Memo() = default;
