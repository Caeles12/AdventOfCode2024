#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>

struct Map {
  int width;
  int height;
  std::vector<std::vector<int>> grid;
};

struct Position {
  int x;
  int y;
};

bool inBounds(int x, int y, Map map)
{
  return x >= 0 && x < map.width && y >= 0 && y < map.height;
}

std::vector<Position>
trailScore(Map m, Position p)
{
  int current_height = m.grid[p.y][p.x];
  if(current_height == 9) {
    return {p};
  }
  std::vector<Position> scores;

  int directions[4][2] = {
    {-1, 0}, 
    {1, 0},
    {0, -1}, 
    {0, 1}
  };

  for(int i=0; i<4; i++) {
    Position np = {p.x + directions[i][0], p.y+directions[i][1]};
    if(inBounds(np.x, np.y, m) && m.grid[np.y][np.x] == current_height + 1) {
      std::vector<Position> recursion = trailScore(m, np);
      scores.insert( scores.end(), recursion.begin(), recursion.end() );
    }
  }

  return scores;
}

int solve(std::ifstream &file)
{
  Map map;
  std::vector<Position> trailheads;
  map.width = 0;
  map.height = 0;
  std::string line;
  int i=0;

  while (std::getline(file, line)) {
    map.grid.push_back({});
    if(map.width < line.size()) {
      map.width = line.size();
    }
    for(int j=0; j<line.size(); j++) {
      int h = line[j] - '0';
      map.grid[i].push_back(h);
      if(h == 0) {
        trailheads.push_back({j, i});
      }
    }
    i++;
  }
  map.height = i;

  int sum_scores = 0;
  int sum_ratings = 0;
  for(int i=0; i<trailheads.size(); i++) {
    std::vector<Position> end_positions = trailScore(map, trailheads[i]);
    std::set<std::pair<int, int>> unique_end_positions;
    for(Position p: end_positions){
      unique_end_positions.insert({p.x, p.y});
    }
    sum_scores += unique_end_positions.size();
    sum_ratings += end_positions.size();
  }
  std::cout << "Sum of the scores: " << sum_scores << std::endl;
  std::cout << "Sum of the ratings: " << sum_ratings << std::endl;
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