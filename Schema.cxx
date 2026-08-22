#include <filesystem>
#include <iostream>

#include "header.hpp"

Schema::Schema() { std::cout << "New Schema" << std::endl; }
Schema::~Schema() {
  // save();
  // if (opened) file.close();
}
Schema& Schema::addField(std::string fieldname, int size) {
  scheme[fieldname] = size;
  return *this;
}
Schema& Schema::addArrayFields(std::string fieldname, ArraySchema& array) {
  array_scheme[fieldname] = array;
  return *this;
}
bool Schema::exists() {
  std::cout << filename << std::endl;
  if (std::filesystem::exists(filename)) return true;
  return false;
}
bool Schema::save() {
  // Use out|trunc, NOT in|out|trunc: the latter fails to create the file if
  // it does not already exist, silently dropping the schema.
  std::fstream file(filename, std::fstream::binary | std::fstream::out |
                                  std::fstream::trunc);
  if (!file.is_open()) return false;
  if (!writable) return false;
  if (!index_set) return false;
  file.write(index.data(), index.size() + 1);
  for (auto i : scheme) {
    file.write((char*)&i.second, 4);
    file.write(i.first.data(), i.first.size() + 1);
  }
  for (auto i : array_scheme) {
    file.write((char*)&Schema::array, sizeof(int));
    file.write(i.first.data(), i.first.size() + 1);
    file.write((char*)&i.second.totalField, sizeof(int));

    for (auto j : i.second.scheme) {
      file.write((char*)&j.second, 4);
      file.write(j.first.data(), j.first.size() + 1);
    }
  }
  file.close();
  return true;
}
Schema& Schema::setIndex(std::string index) {
  this->index = index;
  index_set = true;
  return *this;
}
bool Schema::load(std::string filename) {
  this->filename = filename;
  std::fstream file;
  std::cout << "Reach" << std::endl;
  // Always allow re-saves once the schema is in memory.
  writable = true;
  if (exists() == false) {
    save();
  } else {
    file.open(filename, std::fstream::binary | std::fstream::in);
    if (!file.is_open()) return false;

    // Hard caps to keep an attacker-controlled file from making us allocate
    // or scan huge amounts of memory.
    constexpr size_t kMaxIndexLen = 256;
    constexpr size_t kMaxFieldNameLen = 256;
    constexpr int kMaxFieldsCount = 4096;

    auto readCStr = [&](std::string& out, size_t maxLen) -> bool {
      out.clear();
      char c;
      while (out.size() < maxLen && file.get(c)) {
        if (c == 0) return true;
        out += c;
      }
      // Reached maxLen or EOF without hitting NUL: either truncated or attacker.
      return false;
    };

    char c;

    // Read index (until NUL).
    index = "";
    while (file.get(c)) {
      if (c == 0) break;
      if (index.size() >= kMaxIndexLen) {
        // Index too long — refuse to load this schema.
        return false;
      }
      index += c;
    }

    while (true) {
      int type;
      file.read((char*)&type, sizeof(int));
      if (file.eof() || file.fail()) break;

      if (type == Schema::array) {
        std::string s;
        if (!readCStr(s, kMaxFieldNameLen)) return false;

        int fields;
        file.read((char*)&(fields), sizeof(int));
        if (file.eof() || file.fail()) break;
        if (fields < 0 || fields > kMaxFieldsCount) return false;

        ArraySchema arr;
        for (int i = 0; i < fields; i++) {
          int atype;
          file.read((char*)&atype, sizeof(int));
          if (file.eof() || file.fail()) break;

          std::string si;
          if (!readCStr(si, kMaxFieldNameLen)) return false;

          if (si.size() > 0) arr.addField(si, atype);
        }
        addArrayFields(s, arr);
      } else {
        std::string s;
        if (!readCStr(s, kMaxFieldNameLen)) return false;
        if (s.size() > 0) addField(s, type);
      }
    }
  }
  return true;
}
