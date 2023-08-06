#include "header.hpp"
// call with address of stack
Cell::Cell() {}
Cell::~Cell() {}
bool Cell::set(int size, void* data) {
  this->size = size;
  this->data = data;
  return true;
}

void* Cell::get() { return data; }