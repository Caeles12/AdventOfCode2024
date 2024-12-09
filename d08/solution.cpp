#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <climits> 

const char EMPTY_TILE = '.';

struct Position{
  int x;
  int y;
};

std::vector<Position> findAntinodes(std::vector<Position> antennas, int width, int height, int repeat = 1)
{
  std::vector<Position> antinodes_positions;
  for(int i=0; i<antennas.size(); i++) {
    for(int j=0; j<antennas.size(); j++) {
      if(i != j) {
        int dx = antennas[j].x - antennas[i].x;
        int dy = antennas[j].y - antennas[i].y;
        Position p = {antennas[i].x, antennas[i].y};
        for(int k=0; k<repeat; k++) {
          p.x -= dx;
          p.y -= dy;
          if(p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
            antinodes_positions.push_back(p);
          } else {
            break;
          }
        }
      }
    }
  }
  return antinodes_positions;
}

int solve(std::ifstream &file)
{
  std::map<char, std::vector<Position>> antennas;
  std::string line;

  int map_width = 0;
  int map_height = 0;

  int i = 0;
  while (std::getline(file, line)) {
    if(line.size() > map_width) {
      map_width = line.size();
    }
    for(int j=0; j<line.size(); j++) {
      if(line[j] != EMPTY_TILE) {
        antennas[line[j]].push_back({j, i});
      }
    }
    i++;
  }
  map_height = i;
  std::set<std::pair<int, int>> antinode_unique_positions;
  std::set<std::pair<int, int>> repeating_antinode_unique_positions;


  for (const auto& entry : antennas) {
    std::vector<Position> antinodes = findAntinodes(entry.second, map_width, map_height, 1);
    std::vector<Position> repeating_antinodes = findAntinodes(entry.second, map_width, map_height, INT_MAX);

    for (const Position& val : entry.second) {
      repeating_antinode_unique_positions.insert({val.x, val.y});
    }

    for (const Position& val : antinodes) {
      antinode_unique_positions.insert({val.x, val.y});
    }

    for (const Position& val : repeating_antinodes) {      
      repeating_antinode_unique_positions.insert({val.x, val.y});
    }
  }

  std::cout << "Part 1 solution: " << antinode_unique_positions.size() << std::endl;
  std::cout << "Part 2 solution: " << repeating_antinode_unique_positions.size() << std::endl;

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