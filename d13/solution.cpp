#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cmath>

#define number long long

typedef struct {
  number x;
  number y;
} Vector2D;

std::vector<number> extractNumbers(std::string text)
{
  std::regex number_regex(R"([+-]?\d+)");
  std::smatch matches;
  std::vector<number> numbers;

  auto begin = std::sregex_iterator(text.begin(), text.end(), number_regex);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; ++it) {
      numbers.push_back(std::stoll(it->str()));
  }
  return numbers;
}

number findB(Vector2D P, Vector2D a, Vector2D b)
{

  number den = (a.y*b.x) - (a.x*b.y);
  number num = (P.x*a.y) - (P.y*a.x);

  number k = num / den;

  if(den * k != num) {
    return -1;
  } else {
    return k;
  }
}

number findA(double B, Vector2D P, Vector2D a, Vector2D b)
{
  number num = P.x - (B*b.x);
  number den = a.x;

  number k = num / den;

  if(den * k != num) {
    return -1;
  } else {
    return k;
  }
}

number solve(std::ifstream &file)
{
  std::string line;

  number tokens = 0;
  number tokens_wo_error = 0;

  while (std::getline(file, line)) {
    std::vector<number> buttonA = extractNumbers(line);
    Vector2D a{buttonA[0], buttonA[1]};

    std::getline(file, line);
    std::vector<number> buttonB = extractNumbers(line);
    Vector2D b{buttonB[0], buttonB[1]};

    std::getline(file, line);
    std::vector<number> prize = extractNumbers(line);

    Vector2D P{prize[0] , prize[1]};
    Vector2D P_wo_error{prize[0] + 10000000000000 , prize[1] + 10000000000000};

    number B = findB(P, a, b);
    if(B != -1) {
      number A = findA(B, P, a, b);
      if(A != -1) {
        tokens += (A * 3) + (B * 1);
      }
    }

    number B_wo_error = findB(P_wo_error, a, b);
    if(B_wo_error != -1) {
      number A_wo_error = findA(B_wo_error, P_wo_error, a, b);
      if(A_wo_error != -1) {
        tokens_wo_error += (A_wo_error * 3) + (B_wo_error * 1);
      }
    }

    std::getline(file, line);
  }

  std::cout << "Fewest tokens to spend: " << tokens << std::endl;
  std::cout << "Fewest tokens to spend, after error correction: " << tokens_wo_error << std::endl;

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