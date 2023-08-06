/* #include <iostream>
#include <map>
#include <unordered_map>
using namespace std;
int main() {
  map<int, string> mp1;
  mp1[324] = "1";
  mp1[343] = "2";
  mp1[111] = "3000000000000000";
  for (auto i : mp1) cout << i.first << endl;
  map<int, string> mp2;
  mp2[324] = "1";
  mp2[343] = "2";
  mp2[111] = "3";
  for (auto i : mp2) cout << i.first << endl;
}

// conclusion - for same string in map - order same always
// unordered_map order same always */