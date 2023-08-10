# Calendar

It is a project for my CSE1205 Course Project Submission.

## Project Descripttion

It is a basic calendar application with
* Event Management
  * Adding/Removing events
  * Showing events in range
  * Event plotting on calendar
* Daily Notes 
  * Read/ Write Note 
* Todo Management
  * Add/mark done todos
  * Display todos in homepage

This program is intended to run in an dwm environment, where it'll be kept open in the terminal for todo listing. It'll be used as a productivity tool. 

> Example homepage

```md
────────────────────────────────────────────
─────░░─────────────────────────────────────
──░░░▒▒░░─────────────────────────░░▒░░─────
─░░░▒▒▒▒▒▒──────────────────────░░░▒▒▒░░────
─░▒▒▒▒▒▒▒▒▒─────Calendar────────░░▒▒▒▒▒▒░░──
─░░▐▒▒▒▒▒▒────────────────────▒▒▒▒▒▒▒▒▒▒▒▒░─
────█─█─────────────────────────▀█─▒█▀▀▒░░──
─────▀█──────Team─────────────────▀▀█───────
─────▄█▄───────────27178──────────▄███▄─────
────────────────────────────────────────────
============================================
                August 2023
============================================

     Sun  Mon  Tue  Wed  Thu  Fri  Sat
                 1    2    3    4    5
       6    7   8.    9   10   11   12
      13   14   15   16   17   18   19
      20   21   22   23   24   25   26
      27   28   29   30   31
   
────────────────────────────────────────────
TodoList :
1. wefef
2. Opeator
────────────────────────────────────────────

Menu Action : 
1. Events
2. Daily Note
3. Todos
0. Exit


Choose an option : 1
```

## Database

The Codebase for the UI is just basic. However, the requirements for this project
* More than 4 Classes
* Usage of operator overloading
* Usage of friend classes 
* Usage of friend classes 
* atlest 4 features

were met by the `Database` library (coded for this project) used in this project

Here is an example usage of the libray

```cpp
Database Calendar("Calendar");


Schema EventsSchema;
EventsSchema.setIndex("events")
.addArrayFields("events",
(new ArraySchema())
  ->addField("name", ArraySchema::text)
  .addField("description", Schema::text));

Calendar.add("Events", &EventsSchema); 
//adding a schema is required only once, as the schema is written on the disk in the first time and loaded from disk afterward



// Saving an entry
Row &row = Calendar["Events"].row("210101");
  row("events").push_back(ArrayCell());
  row("events")[0]["name"].set(Schema::text, vstr("1esty"));
  row("events")[0]["description"].set(Schema::text, vstr("qqq"));
  row.save(); //saved to disk

//Retriving an entry
Row *aa = Calendar["Events"]["210101"];
std::cout << (char *)((*aa)("events")[0]["name"].get()) << std::endl;
```

If all the table schemas has been created previously the first line of the above code loads all table infomation on the memory. and is acsesible by `db["tablename"]`. Once a table has been added, it is automatically loaded the next time from the firstline.

However the rows are not opened as the table opens, it is done to save memory.
It is only cpoied to memory during retrival.

## Future plans
- [ ] Indexing the database
- [ ] Solving memory leak issues
- [ ] Allowing efficient searching for other data
- [ ] Adding a server interface, where an external progam can communicate with the database
- [ ] Making the Notes section more interactive
- [ ] Add option to view other months than current month