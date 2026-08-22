#include <cstring>
#include <fstream>
#include <iostream>

#include "header.hpp"
Row::Row(Schema* scheme, std::string location)
    : scheme(scheme), location(location) {}
Row::~Row() {
  // deallocate all cells pointer
}
bool Row::load() {
  this->location = location;
  std::fstream file(location, std::fstream::in | std::fstream::binary);
  if (!file.is_open()) return false;

  // Hard caps. A row file is attacker-controlled (e.g. user-supplied todo
  // text becomes the filename; its content can be anything on disk).
  constexpr size_t kMaxTextLen = 64 * 1024;       // 64 KB per cell
  constexpr int kMaxTotalField = 100000;          // per array entry
  constexpr int kMaxScalarFields = 1024;

  auto readCStr = [&](std::string& out, size_t maxLen) -> bool {
    out.clear();
    char c;
    while (out.size() < maxLen && file.get(c)) {
      if (c == 0) return true;
      out += c;
    }
    return false;  // EOF / overflow / no terminator
  };

  int scalarCount = 0;
  for (auto i : scheme->scheme) {
    if (++scalarCount > kMaxScalarFields) return false;
    switch (i.second) {
      case Schema::integer: {
        cells[i.first].data = new int;
        cells[i.first].size = Schema::integer;
        file.read((char*)cells[i.first].data, sizeof(int));
      } break;
      case Schema::text: {
        std::string s;
        if (!readCStr(s, kMaxTextLen)) {
          // Truncated or attacker-supplied text without NUL — refuse rather
          // than risk a runaway allocation.
          return false;
        }
        char* qqq = new char[s.size() + 1];
        strcpy(qqq, s.c_str());

        cells[i.first].set(Schema::text, qqq);
      } break;
    }
  }
  for (auto i : scheme->array_scheme) {
    if (i.first == "") continue;
    int totalField = 0;
    file.read((char*)&totalField, sizeof(int));
    if (totalField < 0 || totalField > kMaxTotalField) return false;
    for (int k = 0; k < totalField; k++) {
      ArrayCell qq;
      for (auto j : i.second.scheme) {
        switch (j.second) {
          case Schema::text: {
            std::string s;
            if (!readCStr(s, kMaxTextLen)) return false;
            char* qqq = new char[s.size() + 1];
            strcpy(qqq, s.c_str());
            Cell cc;
            cc.set(j.second, qqq);
            qq[j.first] = cc;
          } break;
          case Schema::integer: {
            // needs correction
            int* data = (int*)cells[j.first].data;
            file.read((char*)data, sizeof(int));
          } break;
          case Schema::floating: {
            // needs correction
            double* data = (double*)cells[j.first].data;
            file.read((char*)data, sizeof(double));
          } break;
        }
      }
      array_cells[i.first].push_back(qq);
    }
  }
  file.close();
  loaded = true;
  return true;
};
Cell& Row::operator[](std::string fieldname) {
  // Returning a default-constructed Cell on miss means callers can silently
  // read garbage. We instead look the key up and, if missing, hand back a
  // reference to a thread-local sentinel cell so the caller gets a defined
  // (empty) Cell rather than an inserted one that gets saved on round-trip.
  static Cell sentinel;
  auto it = cells.find(fieldname);
  if (it == cells.end()) return sentinel;
  return it->second;
}
std::vector<ArrayCell>& Row::operator()(std::string fieldname) {
  // Same idea: don't insert a new vector on miss.
  static std::vector<ArrayCell> sentinel;
  auto it = array_cells.find(fieldname);
  if (it == array_cells.end()) return sentinel;
  return it->second;
}

bool Row::save() {
  std::cout << location << std::endl;
  std::cout << index << std::endl;
  std::fstream file(
      location, std::fstream::binary | std::fstream::out | std::fstream::trunc);

  std::cout << location << std::endl;
  for (auto i : scheme->scheme) {
    if (i.first == "") continue;
    switch (i.second) {
      case Schema::text: {
        char* data = (char*)cells[i.first].data;
        if (data == 0) break;
        while (1) {
          file.write((char*)data, sizeof(char));
          if (!*data) break;
          data++;
        }
      } break;
      case Schema::integer: {
        int* data = (int*)cells[i.first].data;
        if (data == 0) break;
        file.write((char*)data, sizeof(int));
      } break;
      case Schema::floating: {
        double* data = (double*)cells[i.first].data;
        if (data == 0) break;
        file.write((char*)data, sizeof(double));
      }
    }
  }
  for (auto i : scheme->array_scheme) {
    if (i.first == "") continue;
    int to = array_cells[i.first].size();
    std::cout << to << std::endl;
    file.write((char*)&(to), sizeof(int));
    for (int k = 0; k < to; k++) {
      for (auto j : i.second.scheme) {
        switch (j.second) {
          case Schema::text: {
            char* data = (char*)array_cells[i.first][k][j.first].data;
            if (data == 0) break;
            while (1) {
              file.write((char*)data, sizeof(char));
              if (!*data) break;
              data++;
            }
          } break;
          case Schema::integer: {
            int* data = (int*)cells[j.first].data;
            file.write((char*)data, sizeof(int));
          } break;
          case Schema::floating: {
            double* data = (double*)cells[j.first].data;
            file.write((char*)data, sizeof(double));
          } break;
        }
      }
    }
  }
  file.close();
  return true;
}

/* What is a row?
 * Row opens a data
 */
