#include <iostream>

#include "header.hpp"
namespace fs = std::filesystem;
namespace {
// Reject path-traversal sequences in row filenames. Row indices become
// filenames; an attacker who can choose the index (e.g. add_todo, where the
// todo text itself is the filename) could otherwise escape the table dir
// and create or read files anywhere the process has access.
bool safeIndex(const std::string& idx) {
  if (idx.empty()) return false;
  if (idx.find('/') != std::string::npos) return false;
  if (idx.find('\\') != std::string::npos) return false;
  if (idx == "." || idx == "..") return false;
  if (idx[0] == '.') return false;  // dotfiles
  // Cap length so we never overflow any downstream buffer.
  if (idx.size() > 4096) return false;
  return true;
}
}  // namespace
Table::Table() {}
Row& Table::row(std::string index) {
  if (scheme == nullptr) {
    std::cerr << "Table::row called before schema was loaded: " << name
              << std::endl;
    std::exit(1);
  }
  if (!safeIndex(index)) {
    std::cerr << "Table::row rejected unsafe index: '" << index << "'"
              << std::endl;
    std::exit(1);
  }
  Row* ret = new Row(scheme, location + "/" + name + "/" + index);
  ret->index = index;
  return *ret;
}
Row* Table::operator[](std::string index) {
  if (scheme == nullptr) {
    std::cerr << "Table::operator[] called before schema was loaded: " << name
              << std::endl;
    return nullptr;
  }
  if (!safeIndex(index)) return nullptr;
  for (const auto& entry :
       fs::directory_iterator(location + "/" + name + "/")) {
    if (fs::is_regular_file(entry)) {
      // std::cout << "File: " << entry.path().filename() << std::endl;
      if (entry.path().filename().string() == index) {
        Row* ret = new Row(
            scheme,
            location + "/" + name + "/" +
                index);  // Row(*scheme, entry.path().filename().string());
        ret->index = index;
        if (!ret->load()) {
          // Load rejected the file as malformed. Don't hand back a Row that
          // looks loaded but isn't — the caller would then `save()` it and
          // either write garbage or, worse, surface null cell data.
          delete ret;
          return nullptr;
        }
        return ret;
      }
    }
  }
  return nullptr;
}
bool Table::load(std::string location) {
  this->location = location;
  // if (scheme == nullptr) //idk if this will cause problem
  scheme = new Schema();

  scheme->load(location + "/" + name + "/Schema");
  return true;
}
bool Table::create(std::string location, Schema* schema) {
  this->location = location;
  // this->name = "yay";
  this->scheme = schema;
  if (!std::filesystem::exists(location + "/" + name)) {
    try {
      if (std::filesystem::create_directory(location + "/" + name)) {
        std::cout << "Table created successfully: " << location + "/" + name
                  << std::endl;
      } else {
        std::cerr << "Failed to create table: " << location << std::endl;
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
  if (scheme == 0) scheme = new Schema();
  scheme->load(location + "/" + name + "/Schema");
  return true;
}
bool Table::exists(std::string str) {
  if (!safeIndex(str)) return false;
  if (fs::exists(location + "/" + name + "/" + str)) return true;

  return false;
}
bool Table::remove(std::string str) {
  if (!safeIndex(str)) return false;
  if (exists(str)) {
    std::filesystem::remove(location + "/" + name + "/" + str);
  }
  return true;
}
/* What is a table
 * table finds a row in the filesystem
 * it only finds does not open
 * index must be string for now
 */