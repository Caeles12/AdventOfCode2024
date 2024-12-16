#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef enum {
	Wall = 0,
	Ground = 1,
	Box = 2,
	LBigBox = 3,
	RBigBox = 4,
} Tile;

struct Map {
  int width;
  int height;
  std::vector<std::vector<Tile>> grid;
};

typedef struct {
	int x;
	int y;
} Vector2D;

typedef struct {
	Vector2D pos;
} Robot;

Tile charToTile(char c)
{
	switch (c) {
		case '#':
			return Tile::Wall;
		case '.':
			return Tile::Ground;
		case 'O':
			return Tile::Box;
		default:
			return Tile::Ground;
    }
}

Vector2D charToDirection(char c)
{
	Vector2D direction;
	switch (c) {
		case '<':
			direction.x = -1;
			direction.y = 0;
			break;
		case '>':
			direction.x = 1;
			direction.y = 0;
			break;
		case '^':
			direction.x = 0;
			direction.y = -1;
			break;
		case 'v':
			direction.x = 0;
			direction.y = 1;
			break;
		default:
			direction.x = 0;
			direction.y = 0;
			break;
	}
	return direction;
}

void printMap(Map m, Robot r)
{
	for(int i=0; i<m.height; i++) {
		for(int j=0; j<m.width; j++) {
			if(r.pos.x == j && r.pos.y == i){
				std::cout << '@';
			} else {
				switch (m.grid[i][j]) {
					case Wall:
						std::cout << '#';
						break;
					case Ground:
						std::cout << '.';
						break;
					case Box:
						std::cout << 'O';
						break;
					case LBigBox:
						std::cout << '[';
						break;
					case RBigBox:
						std::cout << ']';
						break;
				}
			}
		}
		std::cout << std::endl;
	}
}

int GPSCoordinatesSum(Map &m)
{
	int sum = 0;
	for(int i=0; i<m.height; i++) {
		for(int j=0; j<m.width; j++) {
			if(m.grid[i][j] == Tile::Box || m.grid[i][j] == Tile::LBigBox) {
				sum += (i * 100) + j;
			}
		}
	}
	return sum;
}

bool canPushBox(Map &m, Vector2D boxPos, Vector2D direction)
{
	if(m.grid[boxPos.y][boxPos.x] != Tile::Box) {
		return false;
	}

	Vector2D nextTilePos{boxPos.x + direction.x, boxPos.y + direction.y};
	Tile nextTile = m.grid[nextTilePos.y][nextTilePos.x];
	if(nextTile == Tile::Ground) {
		return true; 
	}

	if(nextTile == Tile::Box && canPushBox(m, nextTilePos, direction)) {
		return true;
	}

	return false;
}

void pushBox(Map &m, Vector2D boxPos, Vector2D direction)
{
	Vector2D nextTilePos{boxPos.x + direction.x, boxPos.y + direction.y};
	Tile nextTile = m.grid[nextTilePos.y][nextTilePos.x];
	if(m.grid[boxPos.y][boxPos.x] != Tile::Box) {
		return;
	}
	if(nextTile == Tile::Box) {
		pushBox(m, nextTilePos, direction);
	}
	m.grid[nextTilePos.y][nextTilePos.x] = Tile::Box;
	m.grid[boxPos.y][boxPos.x] = Tile::Ground;
}

bool canPushBigBox(Map &m, Vector2D boxPos, Vector2D direction)
{
	if(m.grid[boxPos.y][boxPos.x] != Tile::LBigBox && m.grid[boxPos.y][boxPos.x] != Tile::RBigBox) {
		return false;
	}
	Vector2D nextTilePos{boxPos.x + direction.x, boxPos.y + direction.y};
	Tile nextTile = m.grid[nextTilePos.y][nextTilePos.x];

	if(direction.y == 0) {
		if(nextTile == Tile::Ground) {
			return true; 
		}
		if((nextTile == Tile::LBigBox || nextTile == Tile::RBigBox)) {
			return canPushBigBox(m, nextTilePos, direction);
		}
	} else {
		Vector2D secondHalfPos;
		Vector2D secondHalfNextTilePos;
		Tile secondHalfNextTile;
		if(m.grid[boxPos.y][boxPos.x] == Tile::LBigBox) {
			secondHalfPos = {boxPos.x + 1, boxPos.y};
			secondHalfNextTilePos = {nextTilePos.x + 1, nextTilePos.y};
		} else {
			secondHalfPos = {boxPos.x - 1, boxPos.y};
			secondHalfNextTilePos = {nextTilePos.x - 1, nextTilePos.y};
		}
		secondHalfNextTile = m.grid[secondHalfNextTilePos.y][secondHalfNextTilePos.x];
		bool a = false;
		bool b = false;
		if(nextTile == Tile::Ground) {
			a = true;
		} else if(nextTile == Tile::RBigBox || nextTile == Tile::LBigBox) {
			a = canPushBigBox(m, nextTilePos, direction);
		}

		if(secondHalfNextTile == Tile::Ground) {
			b = true;
		} else if(secondHalfNextTile == Tile::RBigBox || secondHalfNextTile == Tile::LBigBox) {
			b = canPushBigBox(m, secondHalfNextTilePos, direction);
		}
		
		return a && b;
	}
	return false;
}

void pushBigBox(Map &m, Vector2D boxPos, Vector2D direction)
{
	if(m.grid[boxPos.y][boxPos.x] != Tile::LBigBox && m.grid[boxPos.y][boxPos.x] != Tile::RBigBox) {
		return;
	}
	Vector2D nextTilePos{boxPos.x + direction.x, boxPos.y + direction.y};
	Tile nextTile = m.grid[nextTilePos.y][nextTilePos.x];

	if(direction.y == 0) {
		if((nextTile == Tile::LBigBox || nextTile == Tile::RBigBox)) {
			pushBigBox(m, nextTilePos, direction);
		}
		m.grid[nextTilePos.y][nextTilePos.x] = m.grid[boxPos.y][boxPos.x];
		m.grid[boxPos.y][boxPos.x] = Tile::Ground;
	} else {
		Vector2D secondHalfPos;
		Vector2D secondHalfNextTilePos;
		Tile secondHalfNextTile;
		if(m.grid[boxPos.y][boxPos.x] == Tile::LBigBox) {
			secondHalfPos = {boxPos.x + 1, boxPos.y};
			secondHalfNextTilePos = {nextTilePos.x + 1, nextTilePos.y};
		} else {
			secondHalfPos = {boxPos.x - 1, boxPos.y};
			secondHalfNextTilePos = {nextTilePos.x - 1, nextTilePos.y};
		}
		secondHalfNextTile = m.grid[secondHalfNextTilePos.y][secondHalfNextTilePos.x];
		if(nextTile == m.grid[boxPos.y][boxPos.x]) {
			pushBigBox(m, nextTilePos, direction);
		} else {
			pushBigBox(m, nextTilePos, direction);
			pushBigBox(m, secondHalfNextTilePos, direction);
		}
		m.grid[nextTilePos.y][nextTilePos.x] = m.grid[boxPos.y][boxPos.x];
		m.grid[secondHalfNextTilePos.y][secondHalfNextTilePos.x] = m.grid[secondHalfPos.y][secondHalfPos.x];

		m.grid[boxPos.y][boxPos.x] = Tile::Ground;
		m.grid[secondHalfPos.y][secondHalfPos.x] = Tile::Ground;
	}
}

void moveRobot(Map &m, Robot &r, Vector2D direction)
{
	Vector2D nextPos{r.pos.x + direction.x, r.pos.y + direction.y};
	if(m.grid[nextPos.y][nextPos.x] == Tile::Ground) {
		r.pos.y = nextPos.y;
		r.pos.x = nextPos.x;
	}

	if(m.grid[nextPos.y][nextPos.x] == Tile::Box ){
		bool boxPushed = canPushBox(m, nextPos, direction);
		if(boxPushed) {
			pushBox(m, nextPos, direction);
			r.pos.y = nextPos.y;
			r.pos.x = nextPos.x;
		}
	}
}

void moveRobotBigBox(Map &m, Robot &r, Vector2D direction)
{
	Vector2D nextPos{r.pos.x + direction.x, r.pos.y + direction.y};
	if(m.grid[nextPos.y][nextPos.x] == Tile::Ground) {
		r.pos.y = nextPos.y;
		r.pos.x = nextPos.x;
	}

	if(m.grid[nextPos.y][nextPos.x] == Tile::LBigBox || m.grid[nextPos.y][nextPos.x] == Tile::RBigBox) {
		bool bigBoxPushed = canPushBigBox(m, nextPos, direction);
		if(bigBoxPushed) {
			pushBigBox(m, nextPos, direction);
			r.pos.y = nextPos.y;
			r.pos.x = nextPos.x;
		}
	}
}

int solve(std::ifstream &file)
{
  Map map;
	Map bigBoxMap;
	Robot r;
	Robot rBigBox;
  map.width = 0;
  map.height = 0;
  std::string line;
  int i=0;

  while (std::getline(file, line) && line != "") {
    map.grid.push_back({});
		bigBoxMap.grid.push_back({});
    if(map.width < line.size()) {
      map.width = line.size();
    }
    for(int j=0; j<line.size(); j++) {
				if(line[j] == '@') {
					r.pos.x = j;
					r.pos.y = i;

					rBigBox.pos.x = j * 2;
					rBigBox.pos.y = i;
				}
				switch (line[j]) {
					case '#':
						map.grid[i].push_back(Tile::Wall);
						bigBoxMap.grid[i].push_back(Tile::Wall);
						bigBoxMap.grid[i].push_back(Tile::Wall);
						break;
					case '.':
						map.grid[i].push_back(Tile::Ground);
						bigBoxMap.grid[i].push_back(Tile::Ground);
						bigBoxMap.grid[i].push_back(Tile::Ground);
						break;
					case 'O':
						map.grid[i].push_back(Tile::Box);
						bigBoxMap.grid[i].push_back(Tile::LBigBox);
						bigBoxMap.grid[i].push_back(Tile::RBigBox);
						break;
					default:
						map.grid[i].push_back(Tile::Ground);
						bigBoxMap.grid[i].push_back(Tile::Ground);
						bigBoxMap.grid[i].push_back(Tile::Ground);
						break;
				}
    }
    i++;
  }
	map.height = i;

	bigBoxMap.width = 2*map.width;
	bigBoxMap.height = map.height;

	printMap(map, r);
	printMap(bigBoxMap, rBigBox);


	// Robot pattern
	while (std::getline(file, line)){
		for(char c: line){
			Vector2D direction = charToDirection(c);

			moveRobot(map, r, direction);
			moveRobotBigBox(bigBoxMap, rBigBox, direction);
		}
	}
	printMap(map, r);
	printMap(bigBoxMap, rBigBox);

	std::cout << GPSCoordinatesSum(map) << std::endl;
	std::cout << GPSCoordinatesSum(bigBoxMap) << std::endl;
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