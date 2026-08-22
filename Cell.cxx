#include "header.hpp"
// call with address of stack
Cell::Cell() : size(0), data(nullptr) {}
Cell::~Cell() {
  // Free the heap allocation made when this Cell was populated. The schema
  // type tells us whether `data` is `new char[]` (text), `new int`, or
  // `new double`.
  if (data == nullptr) return;
  switch (size) {
    case Schema::text:
      delete[] (char*)data;
      break;
    case Schema::integer:
      delete (int*)data;
      break;
    case Schema::floating:
      delete (double*)data;
      break;
    default:
      // Unknown size: best effort — leave alone to avoid mismatched delete.
      break;
  }
  data = nullptr;
  size = 0;
}
bool Cell::set(int size, void* data) {
  this->size = size;
  this->data = data;
  return true;
}

void* Cell::get() { return data; }