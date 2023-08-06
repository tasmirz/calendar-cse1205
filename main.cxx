
#include <stdio.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#define W 5
#include "header.hpp"
Database Calendar("Calendar");

#include <ctime>
#include <iomanip>
#include <iostream>
namespace ui {
void print_choices();
void title() {
  std::cout << "────────────────────────────────────────────" << std::endl;
  std::cout << "─────░░─────────────────────────────────────" << std::endl;
  std::cout << "──░░░▒▒░░─────────────────────────░░▒░░─────" << std::endl;
  std::cout << "─░░░▒▒▒▒▒▒──────────────────────░░░▒▒▒░░────" << std::endl;
  std::cout << "─░▒▒▒▒▒▒▒▒▒─────Calendar────────░░▒▒▒▒▒▒░░──" << std::endl;
  std::cout << "─░░▐▒▒▒▒▒▒────────────────────▒▒▒▒▒▒▒▒▒▒▒▒░─" << std::endl;
  std::cout << "────█─█─────────────────────────▀█─▒█▀▀▒░░──" << std::endl;
  std::cout << "─────▀█──────Team─────────────────▀▀█───────" << std::endl;
  std::cout << "─────▄█▄───────────27178──────────▄███▄─────" << std::endl;
  std::cout << "────────────────────────────────────────────" << std::endl;
  /*
   * @ Artwork by tasmirz(2107071)
   */
  // std::cout << std::endl << std::endl;
}
void displayCalendar(int year, int month, const std::vector<int> &eventDays,
                     int startDay) {
  std::string monthNames[] = {"January",   "February", "March",    "April",
                              "May",       "June",     "July",     "August",
                              "September", "October",  "November", "December"};
  int daysInMonth = Date::daysInMonth(month, year);

  std::cout << "============================================" << std::endl;
  std::cout << "                " << monthNames[month - 1] << " " << year
            << std::endl;
  std::cout << "============================================" << std::endl;
  std::cout << "\n   ";
  std::cout << std::setw(W) << "Sun" << std::setw(W) << "Mon" << std::setw(W)
            << "Tue" << std::setw(W) << "Wed" << std::setw(W) << "Thu"
            << std::setw(W) << "Fri" << std::setw(W) << "Sat" << std::endl;

  int dayCount = 1;
  for (int i = 0; i < 6; i++) {
    std::cout << "\   ";
    for (int j = 0; j < 7; j++) {
      if (i == 0 && j < startDay + 1) {
        std::cout << std::setw(W) << " ";
      } else if (dayCount <= daysInMonth) {
        if (std::find(eventDays.begin(), eventDays.end(), dayCount) !=
            eventDays.end()) {
          std::cout << std::setw(W - 1) << dayCount << ".";
        } else {
          std::cout << std::setw(W) << dayCount;
        }
        dayCount++;
      } else {
        break;
      }
    }
    std::cout << std::endl;
  }
  std::cout << "────────────────────────────────────────────" << std::endl;
}
void pause();
void print_event_choices();
int choice() {
  std::cout << std::endl << std::endl;
  std::cout << "Choose an option : ";
  int n;
  std::cin >> n;
  return n;
}
void clear() { system("clear"); }
void calendar() {
  std::time_t now = std::time(nullptr);
  std::tm *localTime = std::localtime(&now);
  int year = localTime->tm_year + 1900;
  int month = localTime->tm_mon + 1;
  int day = localTime->tm_mday + 1;
  int weekday = localTime->tm_wday;

  int a = year * 10000 + month * 100;
  std::vector<int> eventDays;
  for (int i = 1; i <= Date::daysInMonth(month, year); i++) {
    int b = a + i;
    if (std::filesystem::exists("Calendar/Events/" + std::to_string(b))) {
      eventDays.push_back(i);
    }
  }

  displayCalendar(year, month, eventDays, weekday);
}
void todo_list() {
  std::cout << "TodoList :\n";
  std::string folderPath = "Calendar/Todo";  // Replace with the actual path
  // std::vector<std::string> s;
  int k = 0;
  for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
    if (std::filesystem::is_regular_file(entry.path()) &&
        entry.path().filename() != "Schema") {
      // s.push_back(entry.path().filename().string());
      std::cout << ++k << ". " << entry.path().filename().string() << std::endl;
    }
  }
  std::cout << "────────────────────────────────────────────" << std::endl;
  std::cout << "\n";
}

void persistent() {
  clear();
  title();
  calendar();
  todo_list();
}
void list_in_range() {
  std::time_t now = std::time(nullptr);
  std::tm *localTime = std::localtime(&now);
  int year = localTime->tm_year + 1900;
  int month = localTime->tm_mon + 1;
  int day = localTime->tm_mday;
  Date qq(month, day, year);

  // std::cout << qq.string();
  std::cout << "How many days from today? ";
  int q;
  std::cin >> q;
  for (int i = 0; i < q; i++) {
    std::string key = (qq + i).string();
    if (Calendar["Events"].exists(key)) {
      std::cout << (qq + i).string() << std::endl;
      Row *row = Calendar["Events"][key];
      for (int i = 0; i < (*row)("events").size(); i++) {
        std::cout << i + 1 << ". Name        : "
                  << (char *)((*row)("events")[i]["name"].get()) << std::endl;
        std::cout << "   Description : "
                  << (char *)((*row)("events")[i]["description"].get())
                  << std::endl
                  << std::endl;
      }
    }
  }
  pause();
}
void delete_event() {
  persistent();
  std::cout << "Enter" << std::endl << "Day Month Year : ";
  int master = 0;
  int temp;
  std::cin >> temp;
  master += temp;

  std::cin >> temp;
  master += temp * 100;

  std::cin >> temp;
  master += temp * 10000;
  // std::cout << master << std::endl;
  std::string key = std::to_string(master);
  // std::cout << key << std::endl;
  if (Calendar["Events"].exists(key)) {
    Row *row = Calendar["Events"][key];
    for (int i = 0; i < (*row)("events").size(); i++) {
      std::cout << i + 1 << ". Name        : "
                << (char *)((*row)("events")[i]["name"].get()) << std::endl;
      std::cout << "   Description : "
                << (char *)((*row)("events")[i]["description"].get())
                << std::endl
                << std::endl;
    }

    int what;
    std::cout << "Which One : ";
    std::cin >> what;
    if (what >= 1 && what <= (*row)("events").size()) {
      (*row)("events").erase((*row)("events").begin() + what - 1);
      if ((*row)("events").size() > 0)
        row->save();
      else
        std::filesystem::remove("Calendar/Events/" + key);
    }
    pause();
  } else {
    std::cout << "No event on this partcular day\n";
  }
}
void display_event() {
  persistent();
  std::cout << "Enter" << std::endl << "Day Month Year : ";
  int master = 0;
  int temp;
  std::cin >> temp;
  master += temp;

  std::cin >> temp;
  master += temp * 100;

  std::cin >> temp;
  master += temp * 10000;
  // std::cout << master << std::endl;
  std::string key = std::to_string(master);
  // std::cout << key << std::endl;
  if (Calendar["Events"].exists(key)) {
    Row *row = Calendar["Events"][key];
    for (int i = 0; i < (*row)("events").size(); i++) {
      std::cout << i + 1 << ". Name        : "
                << (char *)((*row)("events")[i]["name"].get()) << std::endl;
      std::cout << "   Description : "
                << (char *)((*row)("events")[i]["description"].get())
                << std::endl
                << std::endl;
    }
    pause();
  } else {
    std::cout << "No event on this partcular day\n";
  }
  pause();
}
void addEvent() {
  std::cout << "Enter" << std::endl << "Day Month Year : ";
  int master = 0;
  int temp;
  std::cin >> temp;
  master += temp;

  std::cin >> temp;
  master += temp * 100;

  std::cin >> temp;
  master += temp * 10000;
  std::string key = std::to_string(master);
  std::cout << "Event Name : ";
  std::string event_name;
  std::getline(std::cin, event_name);
  std::getline(std::cin, event_name);
  std::cout << "Event Description : ";
  std::string desc;
  std::getline(std::cin, desc);
  if (!Calendar["Events"].exists(key)) {
    Row &row = Calendar["Events"].row(key);
    row("events").push_back(ArrayCell());
    row("events")[0]["name"].set(Schema::text, vstr(&event_name[0]));
    row("events")[0]["description"].set(Schema::text, vstr(&desc[0]));
    row.save();
    delete &row;
  } else {
    Row *aa = Calendar["Events"][key];
    (*aa)("events").push_back(ArrayCell());
    (*aa)("events")[(*aa)("events").size() - 1]["name"].set(
        Schema::text, vstr(&event_name[0]));
    (*aa)("events")[(*aa)("events").size() - 1]["description"].set(
        Schema::text, vstr(&desc[0]));
    aa->save();
  }
  std::cout << "Successfully saved" << std::endl;
  pause();
}
void print_event_choices() {
  persistent();
  std::cout << "Menu Action : " << std::endl;
  std::cout << "1. Display event" << std::endl;
  std::cout << "2. List events in range" << std::endl;
  std::cout << "3. Add event" << std::endl;
  std::cout << "4. Delete event" << std::endl;
  std::cout << "5. Back to previous menu" << std::endl;
  int chce = choice();
  switch (chce) {
    case 1:
      display_event();
      break;
    case 2:
      list_in_range();
      break;
    case 3:
      addEvent();
      break;
    case 4:
      delete_event();
      break;
    case 5:
      return;
    default:
      break;
  }
  print_event_choices();
}
void pause() { system("read -n1 -r -p \"Press any key to continue...\" key"); }
void remove_todo() {
  std::string folderPath = "Calendar/Todo";  // Replace with the actual path
  std::vector<std::string> s;
  for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
    if (std::filesystem::is_regular_file(entry.path()) &&
        entry.path().filename() != "Schema") {
      s.push_back(entry.path().filename().string());
      //   std::cout << s.size() << " . " << entry.path().filename().string()
      // << std::endl;
    }
  }

  std::cout << "Enter id to remove todo : ";
  int aa;
  std::cin >> aa;
  if (std::filesystem::exists("Calendar/Todo/" + s[aa - 1])) {
    std::filesystem::remove("Calendar/Todo/" + s[aa - 1]);
    std::cout << "Successfully done\n";
  } else
    std::cout << "Wrong input, try again\n";
  pause();
}
void add_todo() {
  std::cout << "Todo : ";
  std::string to;
  getline(std::cin, to);
  getline(std::cin, to);
  Row &row = Calendar["Todo"].row(to);
  row["todo"].set(Schema::text, &to[0]);
  row.save();
  std::cout << "Saved\n";
}
void print_todo_choices() {
  persistent();
  std::cout << "1. Add todo" << std::endl;
  std::cout << "2. Done todo" << std::endl;
  std::cout << "3. Back to previous menu" << std::endl;
  int chce = choice();
  switch (chce) {
    case 1:
      add_todo();
      break;
    case 2:
      remove_todo();
      break;
    case 3:
      break;
    default:
      break;
  }
}
void daily_note() {
  persistent();
  std::cout << "Write : " << std::endl;

  std::string s = "";
  getline(std::cin, s);
  getline(std::cin, s);
  std::time_t now = std::time(nullptr);
  std::tm *localTime = std::localtime(&now);
  int year = localTime->tm_year + 1900;
  int month = localTime->tm_mon + 1;
  int day = localTime->tm_mday;

  int a = year * 10000 + month * 100 + day;
  Row &q = Calendar["Diary"].row(std::to_string(a));
  q["note"].set(Schema::text, (void *)&s[0]);
  q.save();
  std::cout << "Diary written" << std::endl;
  pause();
  int weekday = localTime->tm_wday;
}
void view_note() {
  std::cout << "Enter" << std::endl << "Day Month Year : ";
  int master = 0;
  int temp;
  std::cin >> temp;
  master += temp;

  std::cin >> temp;
  master += temp * 100;

  std::cin >> temp;
  master += temp * 10000;
  std::string key = std::to_string(master);
  if (Calendar["Diary"].exists(key)) {
    Row *rr = Calendar["Diary"][key];
    std::cout << (char *)(*rr)["note"].get() << std::endl;
    // rr->save();
    pause();
  } else {
    std::cout << "Not found" << std::endl;
    pause();
  }
}
void print_daily_notes_choices() {
  persistent();
  std::cout << "Press the corresponding option for specific action"
            << std::endl;
  std::cout << "1. View of" << std::endl;
  std::cout << "2. Write today's note" << std::endl;
  std::cout << "3. Back to previous menu" << std::endl;
  int chce = choice();
  switch (chce) {
    case 1:
      view_note();
      break;
    case 2:
      daily_note();
      // exit(0);
      break;
    default:
      break;
  }
  print_choices();
}

void print_choices() {
  persistent();
  std::cout << "Menu Action : " << std::endl;
  std::cout << "1. Events" << std::endl;
  std::cout << "2. Daily Note" << std::endl;
  std::cout << "3. Todos" << std::endl;
  std::cout << "0. Exit" << std::endl;
  int chce = choice();
  switch (chce) {
    case 1:
      print_event_choices();
      break;
    case 2:
      print_daily_notes_choices();
      break;
    case 3:
      print_todo_choices();
      break;
    case 0:
      exit(0);
      break;
    default:
      break;
  }
  print_choices();
}

}  // namespace ui

int main() {
  ui::title();

  Schema EventsSchema;
  EventsSchema.setIndex("events").addArrayFields(
      "events", (new ArraySchema())
                    ->addField("name", ArraySchema::text)
                    .addField("description", Schema::text));

  Calendar.add("Events", &EventsSchema);
  Schema Todo;
  Todo.setIndex("nill")
      .addField("todo", Schema::text)
      .addArrayFields("events", (new ArraySchema())
                                    ->addField("name", ArraySchema::text)
                                    .addField("description", Schema::text));
  ;
  Schema Diary;
  Diary.setIndex("nill")
      .addField("note", Schema::text)
      .addArrayFields("events",
                      (new ArraySchema())->addField("name", ArraySchema::text));
  Calendar.add("Todo", &Todo);
  Calendar.add("Diary", &Diary);
  ui::print_choices();
  // ui::print_todo_choices();
  /*
  // switch (ui::print_choices()) {}




  Row &row = Calendar["Events"].row("210101");

  row["data"].set(Schema::text, vstr("Hello"));
  row["data2"].set(Schema::text, vstr("hi"));

  row("events").push_back(ArrayCell());
  row("events")[0]["name"].set(Schema::text, vstr("1esty"));
  row("events")[0]["description"].set(Schema::text, vstr("qqq"));

  row("events").push_back(ArrayCell());
  row("events")[1]["name"].set(Schema::text, vstr("testy"));
  row("events")[1]["description"].set(Schema::text, vstr("qqq"));

  row.save();
  Row *aa = Calendar["Events"]["210101"];

  std::cout << (char *)(*aa)["data"].get() << std::endl;
  std::cout << (char *)(*aa)["data2"].get() << std::endl;
  std::cout << (char *)((*aa)("events")[0]["name"].get()) << std::endl;
  std::cout << "reach" << std::endl; */
  ///
  // Calendar["Events"]["20230321"]["index"];  // this returns a row.
  // Calendar["Events"]["20230321"](
  //    "events")["name"];  // this returns a row array. */
  //
  /*  Schema a("hihi", Schema::create);
   a.load()
       .addField("hi", Schema::integer)
       .addField("hello", Schema::integer)
       .save();
  */
  /* Schema a("hihi", Schema::open);
  a.load(); */
}

/*
Calendar
i.  Date
ii.*/

/// implement delete , , delete from disk,
///  erase elememt form array, re save