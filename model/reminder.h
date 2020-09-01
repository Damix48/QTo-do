#ifndef REMINDER_H
#define REMINDER_H

#include <ctime>
#include <string>

#include "item.h"

class Date {
 private:
  int day;
  int month;
  int year;

  static bool isLeapYear(int);
  static bool isValid(int, int, int);

 public:
  Date(int, int, int);
  Date();

  int Day() const;
  int Month() const;
  int Year() const;

  bool operator==(const Date&) const;
  bool operator!=(const Date&) const;
  bool operator>(const Date&) const;
  bool operator<(const Date&) const;
  bool operator>=(const Date&) const;
  bool operator<=(const Date&) const;
};

class Reminder : virtual public Item {
 private:
  Date date;

 public:
  Reminder(std::string, int, int, int);
  bool passed() const;
  void changeDate(int, int, int);
  Date getDate() const;

  virtual Item::Type getType() const override;

  virtual ~Reminder() override;
};

#endif  // REMINDER_H
