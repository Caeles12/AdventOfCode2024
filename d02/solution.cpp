#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#define number unsigned long

using namespace std;

typedef enum
{
  Increase = 1,
  Decrease = -1,
  None = 0,
} EDirection;

vector<number> split (const string &s, char delim) {
    vector<number> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (stoul(item));
    }

    return result;
}

bool isSafe(vector<number> &report) {
  EDirection lastDirection = None;
  for(size_t i=0; i<report.size() - 1; i++) {
    number a = report.at(i);
    number b = report.at(i + 1);
    if(a < b && b - a <= 3) {
      if(lastDirection != None && lastDirection != Increase) {
        return false;
      }
      lastDirection = Increase;
    } else if(a > b && a - b <= 3) {
      if(lastDirection != None && lastDirection != Decrease) {
        return false;
      }
      lastDirection = Decrease;
    } else {
      return false;
    }
  }

  return true;
}

bool isSafeTolerant(vector<number> &report) {
  if(isSafe(report)) return true;

  for(int i=0; i<report.size(); i++) {
    vector<number> otherReport = report;
    otherReport.erase(otherReport.begin() + i);
    if(isSafe(otherReport)) return true;
  }
  return false;
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

  number safe_total = 0;
  number safe_with_errors_total = 0;

  string line;
  while (getline(file, line)) {
    vector<number> report = split(line, ' ');
    safe_total += isSafe(report);
    safe_with_errors_total += isSafeTolerant(report);
  }

  cout << "Part 1 result: " << safe_total << endl;
  cout << "Part 2 result: " << safe_with_errors_total << endl;

  file.close();
  return 0;
}