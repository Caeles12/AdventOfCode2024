#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <map>


#define number unsigned long


std::vector<number> split (const std::string &s, char delim) {
    std::vector<number> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (stoul(item));
    }

    return result;
}

bool isCorrectlyOrdered(std::vector<number> book, std::map<number, std::set<number>> &rules)
{
  for(int i=0; i<book.size(); i++) {
    for(int j=0; j<i; j++) {
      std::set<number> current_page_rules =  rules[book[i]];
      if(current_page_rules.find(book[j]) != current_page_rules.end()) {
        return false;
      }
    }
  }
  return true;
}

std::vector<number> correctOrder(std::vector<number> book, std::map<number, std::set<number>> &rules) {
  std::vector<number> book_corrected_order;
  for(int i=0; i<book.size(); i++) {
    int correct_position;
    std::set<number> current_page_rules =  rules[book[i]];
    for(correct_position=0; correct_position<i; correct_position++) {
      if(current_page_rules.find(book_corrected_order[correct_position]) != current_page_rules.end()) {
        break;
      }
    }
    book_corrected_order.insert(book_corrected_order.begin() + correct_position, book.at(i));
  }
  return book_corrected_order;
}

int solve(std::ifstream &file)
{

  std::map<number, std::set<number>> rules;

  std::string line;
  while (std::getline(file, line) && line.length() > 0) {
    std::vector<number> values = split(line, '|');
    rules[values[0]].insert(values[1]);
  }

  bool valid_page;
  number correctly_ordered_sum = 0;
  number incorrectly_ordered_sum = 0;
  while (std::getline(file, line)) {
    std::vector<number> book = split(line, ',');

    bool correctly_ordered = isCorrectlyOrdered(book, rules);
    if(correctly_ordered) {
      number middle_page = book.at(book.size() / 2);
      correctly_ordered_sum += middle_page;
    } else {
      std::vector<number> corrected_book = correctOrder(book, rules);

      number middle_page = corrected_book.at(corrected_book.size() / 2);
      incorrectly_ordered_sum += middle_page;
    }
  }

  std::cout << "Sum of the middle page of correctly ordered updates: " << correctly_ordered_sum << std::endl;
  std::cout << "Sum of the middle page of incorrectly ordered updates: " <<incorrectly_ordered_sum << std::endl;
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