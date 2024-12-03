#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

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

  std::string mul_regex = "mul\\([0-9]{1,3},[0-9]{1,3}\\)";
  std::string do_regex = "do\\(\\)";
  std::string dont_regex = "don\'t\\(\\)";

  std::string full_regex = "(" + mul_regex + ")|(" + do_regex + ")|(" + dont_regex +")";

  std::regex re(full_regex);
  

  number sum_all = 0;
  number sum_with_activation = 0;

  bool activated = true;

  std::string line;
  while (std::getline(file, line)) {
    for (std::sregex_iterator it = std::sregex_iterator(line.begin(), line.end(), re);
        it != std::sregex_iterator(); it++) {
        std::smatch match;
        match = *it;
        std::string s = match.str(0);
        std::string prefix = s.substr(0, 3);
        
        if(prefix == "mul") {
          size_t position = match.position(0);

          std::string stripped_string = s.substr(4, s.length() - 1 - 4);
          std::vector<number> numbers = split(stripped_string, ',');
          number mul = numbers[0] * numbers[1];
          sum_all += mul;
          if(activated) sum_with_activation += mul;
        } else if(prefix == "do(") {
          activated = true;
        } else if(prefix == "don") {
          activated = false;
        }
    }
  }

  std::cout << "Part 1 result: " << sum_all << std::endl;
  std::cout << "Part 2 result: " << sum_with_activation << std::endl;


  file.close();
  return 0;
}