#include <iostream>

#include "header.hpp"

Date::Date(int month, int day, int year) {
  this->month = month;
  this->day = day;
  this->year = year;
}
std::string Date::string() {
  int num = year * 10000 + month * 100 + day;
  std::string ret = std::to_string(num);
  return ret;
}
Date Date::operator+(int days) {
  Date newDate = *this;  // cpied itself
  newDate.day += days;

  while (newDate.day > daysInMonth(newDate.month, newDate.year)) {
    newDate.day -= daysInMonth(newDate.month, newDate.year);
    newDate.month++;
  }

  if (newDate.month > 12) {
    newDate.month = 1;
    newDate.year++;
  }

  return newDate;
}
Date Date::operator-(int days) {
  Date newDate = *this;
  newDate.day -= days;

  while (newDate.day < 1) {
    newDate.day += daysInMonth(newDate.month, newDate.year);
    newDate.month--;
  }

  if (newDate.month < 1) {
    newDate.month = 12;
    newDate.year--;
  }

  return newDate;
}

int Date::daysInMonth(int month, int year) {
  int arr[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 &&
      ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0))))
    arr[2]++;
  return arr[month - 1];
};
