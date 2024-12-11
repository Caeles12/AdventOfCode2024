#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <cmath>
#include <map>

#define number unsigned long long

std::map<number, number> blink(std::map<number, number> precedent)
{
  std::map<number, number> next;

  for(std::pair<number, number> stone: precedent) {
    std::string string_repr = std::to_string(stone.first);
    int n = string_repr.size();
    if(stone.first == 0) {
      next[1] += stone.second;
    }else if(n % 2 == 0) {
      number first_half = std::stoull(string_repr.substr(0, n / 2));
      number second_half = std::stoull(string_repr.substr(n / 2, n / 2));
      next[first_half] += stone.second;
      next[second_half] += stone.second;
    } else {
      next[stone.first * 2024] += stone.second;
    }
  }

  return next;
}

void printState(std::vector<number> state)
{
  for(int j=0; j<state.size(); j++) {
    std::cout << state[j] << " ";
  }
  std::cout << std::endl;
}

int solve(std::ifstream &file)
{
  std::map<number, number> state;

  std::string word;
  while (file >> word) {
    number value = std::stoull(word);
    state[value] += 1;
  }

  for(int i=0; i<75; i++) {
    state = blink(state);
    number total = 0;
    for(std::pair<number, number> stone: state) {
      total += stone.second;
    }

    std::cout << "After " << i+1 << " blink" << (i > 0 ? "s" : "") << ": " << total << " stone" <<  (total > 1 ? "s" : "") << std::endl;
  }
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