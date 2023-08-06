#include "header.hpp"
ArraySchema& ArraySchema::addField(std::string fieldname, int type) {
  scheme[fieldname] = type;
  totalField++;
  return *this;
}
inline int ArraySchema::getTotalFields() { return totalField; }