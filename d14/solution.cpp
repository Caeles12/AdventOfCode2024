#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include <regex>

typedef struct {
  int NW;
  int NE;
  int SW;
  int SE;
} Quadrants;

typedef struct {
  int x;
  int y;
} Vector2D;

typedef struct {
  Vector2D p;
  Vector2D v;
} Robot;

typedef struct {
  int width = 0;
  int height = 0;
} Map;


std::vector<int> extractNumbers(std::string text)
{
  std::regex number_regex(R"([+-]?\d+)");
  std::smatch matches;
  std::vector<int> numbers;

  auto begin = std::sregex_iterator(text.begin(), text.end(), number_regex);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; ++it) {
      numbers.push_back(std::stoi(it->str()));
  }
  return numbers;
}


void moveRobot(Robot &r, Map m)
{
  r.p.x = (r.p.x + r.v.x) % m.width;
  r.p.y = (r.p.y + r.v.y) % m.height;
  if(r.p.x < 0) r.p.x += m.width;
  if(r.p.y < 0)  r.p.y += m.height;
}

bool noRobotOverlap(std::vector<Robot> robots, Map m)
{
  int map[m.height][m.width];
  memset( map, 0, m.height*m.width*sizeof(int) );
  for(Robot r: robots)
  {
    if(map[r.p.y][r.p.x] != 0) {
      return false;
    }
    map[r.p.y][r.p.x] ++;
  }
  return true;
}

void printMap(std::vector<Robot> robots, Map m)
{
  int map[m.height][m.width];
  memset( map, 0, m.height*m.width*sizeof(int) );
  for(Robot r: robots)
  {
    map[r.p.y][r.p.x] ++;
  }
  for(int i=0; i<m.height; i++) {
    for(int j=0; j<m.width; j++) {
      if(map[i][j] == 0) {
        std::cout << '.';
      } else {
        std::cout << map[i][j];
      }
    }
    std::cout << std::endl;
  }
}
Quadrants countQuadrants(std::vector<Robot> robots, Map m)
{
  Quadrants q = {0, 0, 0, 0};

  int xLimit = m.width / 2;
  int yLimit = m.height / 2;

  for(Robot r: robots) {
    if(r.p.y < yLimit) {
      if(r.p.x < xLimit) {
        q.NW ++;
      } else if(r.p.x > xLimit) {
        q.NE ++;
      }
    }else if (r.p.y > yLimit) {
      if(r.p.x < xLimit) {
        q.SW ++;
      } else if(r.p.x > xLimit) {
        q.SE ++;
      }
    }
  }

  return q;
}

int solve(std::ifstream &file, Map m)
{
  std::vector<Robot> robots;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<int> numbers = extractNumbers(line);
    Robot r{
      {numbers[0], numbers[1]},
      {numbers[2], numbers[3]}
    };
    robots.push_back(r);
  }
  //printMap(robots, m);
  Quadrants q;
  std::cout << std::endl;
  int i = 0;
  bool foundEasterEgg = false;
  while(true){
    for(Robot &r: robots) {
      moveRobot(r, m);
    }
    i++;
    if(i == 100) {
      q = countQuadrants(robots, m);
      std::cout << "After 100 seconds, the safety factor is: " << q.NW * q.NE * q.SW * q.SE << std::endl;
    }
    if(noRobotOverlap(robots, m)) {
      printMap(robots, m);
      std::cout << "Found the easter egg after " << i << " seconds." << std::endl;
      foundEasterEgg = true;
    };
    if(foundEasterEgg && i >= 100) {
      break;
    }
  }
  
  

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <input file> <width> <height>" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
    return 1;
  }

  Map m{
    std::stoi(argv[2]),
    std::stoi(argv[3])
  };

  solve(file, m);

  file.close();
  return 0;
}