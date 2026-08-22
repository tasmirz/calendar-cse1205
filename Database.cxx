#include <iostream>

#include "header.hpp"
Database::Database(std::string name) {
  this->name = name;
  if (!std::filesystem::exists(name)) {
    try {
      if (std::filesystem::create_directory(name)) {
        std::cout << "Database created successfully: " << name << std::endl;
      } else {
        std::cerr << "Failed to create database: " << name << std::endl;
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
  // Dir creation done

  std::string path = name;

  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_directory(entry)) {
      // load tables iteratively
      tables[entry.path().filename().string()] =
          new Table();  // entry.path().filename()
      tables[entry.path().filename().string()]->name =
          entry.path().filename().string();
      tables[entry.path().filename().string()]->load(name);
    }
  }
}
bool Database::exist(std::string tablename) {
  if (tables.find(tablename) == tables.end()) return false;
  return true;
}
void Database::add(std::string tablename, Schema* schema) {
  if (exist(tablename)) return;
  std::cout << "table : " << tablename << std::endl;
  tables[tablename] = new Table();
  tables[tablename]->name = tablename;
  tables[tablename]->create(name, schema);

  // create table, according to schema
}
Table& Database::operator[](std::string key) {
  // tables[key] silently inserts a nullptr on miss, which would segfault on
  // deref. Refuse instead so the caller learns the table is missing.
  auto it = tables.find(key);
  if (it == tables.end() || it->second == nullptr) {
    std::cerr << "Database::operator[] unknown table: '" << key
              << "'. Did you forget to call Database::add?"
              << std::endl;
    std::exit(1);
  }
  return *it->second;
}

/* table loads schema itself */