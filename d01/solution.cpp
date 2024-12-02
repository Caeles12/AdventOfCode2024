#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#define ul unsigned long

using namespace std;

void insertSorted(ul value, vector<ul> &v) {
  int i;
  for(i = 0; i<v.size(); i++){
    if(v.at(i) > value) break;
  }
  v.insert(v.begin() + i, value);
}

string vectorToString(vector<ul> &v) {
  string s = "{ ";
  for(ul i = 0; i<v.size(); i++){
    s +=  to_string(v.at(i)) + " ";
  }
  s += "}";
  return s;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input file>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "Error: Unable to open file " << argv[1] << endl;
    return 1;
  }

  vector<ul> first_column = {};
  vector<ul> second_column = {};

  map<ul, ul> occurences; // For part 2

  // Parsing the file to extract the two lists
  string word;
  while (file.good()) {
    file >> word;
    ul first_value = stol(word);
    insertSorted(first_value, first_column);

    file >> word;
    ul second_value = stol(word);
    insertSorted(second_value, second_column);

    if(occurences.count(second_value) == 0) {
      occurences[second_value] = 1;
    } else {
      occurences[second_value] ++;
    }
  }

  file.close();

  // PART 1 //
  ul sum = 0;
  for(int i=0; i<first_column.size(); i++) {
    ul a = first_column.at(i);
    ul b = second_column.at(i);
    if(a > b) {
      sum += a - b;
    } else {
      sum += b - a;
    }
  }

  cout << "Part 1: "  << sum << endl;

  // PART 2 //
  sum = 0;
  for(int i=0; i<first_column.size(); i++) {
    ul a = first_column.at(i);
    if(occurences.count(a) != 0) {
      sum += a * occurences[a];
    }
  }

  cout << "Part 2: "  << sum << endl;

  return 0;
}