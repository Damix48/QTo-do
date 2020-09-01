#include "reminder.h"

#include <ctime>

Reminder::Reminder(std::string title_, int d, int m, int y) : Item(title_),
                                                              date(d, m, y) {}

Date Reminder::getDate() const {
  return date;
}

void Reminder::changeDate(int d, int m, int y) {
  date = Date(d, m, y);
}

bool Reminder::passed() const {
  return date <= Date();
}

Item::Type Reminder::getType() const {
  return Item::Type::Reminder;
}

Reminder::~Reminder() = default;

/* DATE */
Date::Date(int day_, int month_, int year_) {
  if (isValid(day_, month_, year_)) {
    day = day_;
    month = month_;
    year = year_;
  } else {
    day = 1;
    month = 1;
    year = 1970;
  }
}

Date::Date() {
  std::time_t now = std::time(nullptr);
  struct tm* time = localtime(&now);

  day = time->tm_mday;
  month = time->tm_mon + 1;
  year = time->tm_year + 1900;
}

bool Date::isLeapYear(int year_) {
  return (year_ % 4 == 0 && year_ % 100 != 0) || year_ % 400 == 0;
}

bool Date::isValid(int day_, int month_, int year_) {
  if (year_ > 0) {
    if (month_ == 1 || month_ == 3 || month_ == 5 || month_ == 7 || month_ == 8 || month_ == 10 || month_ == 12) {
      return (day_ > 0 && day_ <= 31);
    } else if (month_ == 2) {
      if (isLeapYear(year_)) {
        return (day_ > 0 && day_ <= 29);
      }
      return (day_ > 0 && day_ <= 28);
    } else if (month_ > 0 && month_ <= 12) {
      return (day_ > 0 && day_ <= 30);
    }
  } else {
    return false;
  }
  return false;
}

int Date::Day() const {
  return day;
}

int Date::Month() const {
  return month;
}

int Date::Year() const {
  return year;
}

bool Date::operator==(const Date& date_) const {
  return (day == date_.day && month == date_.month && year == date_.year);
}

bool Date::operator!=(const Date& date_) const {
  return (day != date_.day || month != date_.month || year != date_.year);
}

bool Date::operator>(const Date& date_) const {
  if (year > date_.year) {
    return true;
  } else if (year == date_.year) {
    if (month > date_.month) {
      return true;
    } else if (month == date_.month) {
      if (day > date_.day) {
        return true;
      }
    }
    return false;
  }
  return false;
}

bool Date::operator<(const Date& date_) const {
  if (year < date_.year) {
    return true;
  } else if (year == date_.year) {
    if (month < date_.month) {
      return true;
    } else if (month == date_.month) {
      if (day < date_.day) {
        return true;
      }
    }
    return false;
  }
  return false;
}

bool Date::operator>=(const Date& date_) const {
  return *this > date_ || *this == date_;
}

bool Date::operator<=(const Date& date_) const {
  return *this < date_ || *this == date_;
}
