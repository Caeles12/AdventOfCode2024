#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#define number unsigned long

number add(number x, number y) { return x + y; }
number multiply(number x, number y) { return x * y; }
number concat(number x, number y) { return std::stoul(std::to_string(x) + std::to_string(y)); }

std::vector<number> possibleEquationsResults(number target, std::vector<number> values, std::vector<std::function<number(number, number)>> operations)
{
  std::vector<number> results;
  int current_value = values.back();
  values.pop_back();

  if(values.size() == 0) {
    results.push_back(current_value);
  }
  else {
    std::vector<number> possibles_results = possibleEquationsResults(target, values, operations);

    for(int i=0; i<possibles_results.size(); i++) {
      for(int j=0; j<operations.size(); j++) {
        results.push_back(operations[j](possibles_results[i], current_value));
      }
    }
  }
  return results;
}

int solve(std::ifstream &file)
{

  number sum = 0;
  number sum_with_concat = 0;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream lineStream(line);
    number key;
    lineStream >> key;

    char colon;
    lineStream >> colon;

    std::vector<number> values;
    number value;

    while (lineStream >> value) {
        values.push_back(value);
    }

    std::vector<number> possible_results = possibleEquationsResults(key, values, {&add, &multiply});
    if(std::find(possible_results.begin(), possible_results.end(), key) != possible_results.end()) {
      sum += key;
    }

    std::vector<number> possible_results_with_concat = possibleEquationsResults(key, values, {&add, &multiply, &concat});
    if(std::find(possible_results_with_concat.begin(), possible_results_with_concat.end(), key) != possible_results_with_concat.end()) {
      sum_with_concat += key;
    }
  }

  std::cout << sum << std::endl;
  std::cout << sum_with_concat << std::endl;

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
    return 1;
  }

  solve(file);

  file.close();
  return 0;
}