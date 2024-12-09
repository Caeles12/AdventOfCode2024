#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>

typedef enum
{
  OBSTRUCTION = 1,
  GROUND = 0,
} ETileType;

std::map<char, ETileType> tiles_repr = {
  {'#', OBSTRUCTION},
  {'.', GROUND},
};

struct LabMap {
  int width;
  int height;
  std::vector<std::vector<ETileType>> grid;
};

struct Vector2D {
  int x;
  int y;
};

struct Guard {
  Vector2D p;
  Vector2D d;
};

bool inBounds(int x, int y, LabMap map)
{
  return x >= 0 && x < map.width && y >= 0 && y < map.height;
}

void step(Guard &guard, LabMap &map)
{
  if(guard.d.y == 0 
     && inBounds(guard.p.x + guard.d.x, guard.p.y + guard.d.y, map)
     && map.grid[guard.p.y][guard.p.x + guard.d.x] == ETileType::OBSTRUCTION) { // Vertically aligned obstacle
      guard.d.y = guard.d.x;
      guard.d.x = 0;
  } else if(guard.d.x == 0 
     && inBounds(guard.p.x + guard.d.x, guard.p.y + guard.d.y, map)
     && map.grid[guard.p.y + guard.d.y][guard.p.x] == ETileType::OBSTRUCTION) { // Horizontally aligned obstacle
      guard.d.x = -guard.d.y;
      guard.d.y = 0;
  } else {
    guard.p.x += guard.d.x;
    guard.p.y += guard.d.y;
  }
}

int solve(std::ifstream &file)
{
  LabMap lab_map;
  lab_map.width = 0;
  lab_map.height = 0;

  Guard guard;
  Guard guard_initial_state;
  std::string line;
  int i = 0;

  while (std::getline(file, line)) {
    if(lab_map.width < line.size()) {
      lab_map.width = line.size();
    }

    lab_map.grid.push_back( std::vector<ETileType>() );
    for(int j=0; j<line.size(); j++) {
      if(line.at(j) == '^'){
        guard_initial_state.p.x = j;
        guard_initial_state.p.y = i;
        guard_initial_state.d.x = 0;
        guard_initial_state.d.y = -1;
        lab_map.grid[i].push_back(ETileType::GROUND);
      } else {
        lab_map.grid[i].push_back(tiles_repr[line.at(j)]);
      }
    }
    i ++;
  }
  lab_map.height = i;

  std::set<std::pair<int, int>> visited_positions;

  guard.p = guard_initial_state.p;
  guard.d = guard_initial_state.d;

  while(inBounds(guard.p.x, guard.p.y, lab_map)) {
    int gx = guard.p.x;
    int gy = guard.p.y;
    visited_positions.insert({gx, gy});
    step(guard, lab_map);
  }
  std::cout << visited_positions.size() << std::endl;

  int count = 0;
  i = 0;

  for (auto itr = visited_positions.rbegin(); 
    itr != visited_positions.rend(); itr++) 
  {
    int obstacle_x = itr->first;
    int obstacle_y = itr->second;

    i++;
    std::cout << (i / (visited_positions.size() * 1.)) * 100. << "%" << std::endl;


    if(obstacle_x != guard_initial_state.p.x || obstacle_y != guard_initial_state.p.y) {
      lab_map.grid[obstacle_y][obstacle_x] = ETileType::OBSTRUCTION;
      
      guard.p = guard_initial_state.p;
      guard.d = guard_initial_state.d;

      bool isLoop = false;

      std::map<std::pair<int, int>, std::set<std::pair<int, int>>> seen_positions_directions;

      while(inBounds(guard.p.x, guard.p.y, lab_map)) {
        int gx = guard.p.x;
        int gy = guard.p.y;
        if(seen_positions_directions[{guard.p.x, guard.p.y}].count({guard.d.x, guard.d.y}) != 0) {
          isLoop = true;
          break;
        }
        seen_positions_directions[{guard.p.x, guard.p.y}].insert({guard.d.x, guard.d.y});
        step(guard, lab_map);
      }

      if(isLoop) {
        //std::cout << "FOUND A LOOP: " << obstacle_x << " " << obstacle_y << std::endl;
        count ++;
      }

      lab_map.grid[obstacle_y][obstacle_x] = ETileType::GROUND;
    }
  }

  std::cout << "Found " << count << " ways to make the guard loop." << std::endl;

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