#include "item.h"

#include <string>

Item::Item(std::string title_) : title(title_) {}

void Item::changeTitle(std::string title_) {
  title = title_;
}

std::string Item::getTitle() const {
  return title;
}
