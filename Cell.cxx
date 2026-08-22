#include <cstring>

#include "header.hpp"
// call with address of stack
Cell::Cell() : size(0), data(nullptr) {}

Cell::Cell(const Cell& other) : size(0), data(nullptr) { *this = other; }

Cell::Cell(Cell&& other) noexcept : size(other.size), data(other.data) {
  other.size = 0;
  other.data = nullptr;
}

Cell& Cell::operator=(const Cell& other) {
  if (this == &other) return *this;
  // free existing
  if (data) {
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
        break;
    }
  }
  size = other.size;
  data = nullptr;
  if (!other.data) return *this;
  switch (other.size) {
    case Schema::text: {
      size_t len = strlen((char*)other.data);
      char* p = new char[len + 1];
      strcpy(p, (char*)other.data);
      data = p;
      break;
    }
    case Schema::integer: {
      int* p = new int;
      *p = *(int*)other.data;
      data = p;
      break;
    }
    case Schema::floating: {
      double* p = new double;
      *p = *(double*)other.data;
      data = p;
      break;
    }
    default:
      data = nullptr;
      size = 0;
      break;
  }
  return *this;
}

Cell& Cell::operator=(Cell&& other) noexcept {
  if (this == &other) return *this;
  if (data) {
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
        break;
    }
  }
  size = other.size;
  data = other.data;
  other.size = 0;
  other.data = nullptr;
  return *this;
}

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
  // Free previous allocation if any (prevents leak when re-setting)
  if (this->data && this->data != data) {
    switch (this->size) {
      case Schema::text:
        delete[] (char*)this->data;
        break;
      case Schema::integer:
        delete (int*)this->data;
        break;
      case Schema::floating:
        delete (double*)this->data;
        break;
      default:
        break;
    }
  }
  this->size = size;
  this->data = data;
  return true;
}

void* Cell::get() { return data; }