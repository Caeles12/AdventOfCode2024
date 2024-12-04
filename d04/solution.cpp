#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string TARGET = "XMAS";

struct Vector2D{            
  int x;
  int y;
};

struct Word {
  Vector2D position;
  Vector2D direction;
  std::string text;
};

struct WordGrid {
  std::vector<std::vector<char>> grid;
  int width;
  int height;
};

// Check if the word is present at the position and with the direction in the grid
bool isRealSolution(WordGrid &word_grid, Word &word)
{
  Vector2D current_position = word.position;
  std::string text = word.text;
  for(int i=0; i<text.length(); i++) {
    int x = current_position.x;
    int y = current_position.y;

    // Out of bound position
    if(x < 0 || x >= word_grid.width || y < 0 || y >= word_grid.height) {
      return false;
    }

    // Letter does not correspond
    if(word_grid.grid[y][x] != text.at(i)) {
      return false;
    }

    current_position.x += word.direction.x;
    current_position.y += word.direction.y;

  }
  return true;
}

// Find a word in the grid
std::vector<Word> findInWordGrid(WordGrid &word_grid, std::string word)
{
  std::vector<Word> words;
  // Checking all tiles
  for(int i=0; i<word_grid.height; i++) {
    for(int j=0; j<word_grid.width; j++) {
      // Checking all directions
      for(int dx=-1; dx<=1; dx++) {
        for(int dy=-1; dy<=1; dy++) {
          // Checking if the word exists here
          if(dx != 0 || dy != 0) {
            Word w;
            w.position = {i, j};
            w.direction = {dx, dy};
            w.text = word;
            if(isRealSolution(word_grid, w)) {
              words.push_back(w);
            }
          }
        }
      }
    }
  }
  return words;
}


// Check if there is a valid X-MAS at the position
bool isX_MAS(WordGrid &word_grid, Vector2D pos)
{
  if(word_grid.grid[pos.y][pos.x] != 'A') {
    return false;
  }

  Vector2D cross_positions[] = {
    {1, 1}, 
    {1, -1}, 
    {-1, -1}, 
    {-1, 1}
  };

  std::string target = "MMSS";

  for(int i=0; i<4; i++) {
    bool isCorrect = true;
    for(int j=0; j<4; j++) {
      Vector2D current_relative_position = cross_positions[(i + j) % 4];

      int x =  pos.x + current_relative_position.x;
      int y = pos.y + current_relative_position.y;

      // Out of bound position
      if(x < 0 || x >= word_grid.width || y < 0 || y >= word_grid.height) {
        return false;
      }

      if(word_grid.grid[y][x] != target[j]) {
        isCorrect = false;
        break;
      }
    }

    if(isCorrect) {
      return true;
    }
  }
  return false;
}

// Find all X-MAS crosses in the grid
std::vector<Vector2D> findAllX_MAS(WordGrid &word_grid)
{
  std::vector<Vector2D> xmas_pos;
  for(int i=0; i<word_grid.height; i++) {
      for(int j=0; j<word_grid.width; j++) {
        Vector2D pos = {j, i};
        if(isX_MAS(word_grid, pos)) {
          xmas_pos.push_back(pos);
        }
      }
  }
  return xmas_pos;
}

int solve(std::ifstream &file)
{
  int i = 0;

  WordGrid word_grid;

  std::string line;
  while (std::getline(file, line)) {
    if(word_grid.width < line.size()) {
      word_grid.width = line.size();
    }

    word_grid.grid.push_back( std::vector<char>() );
    for(int j=0; j<line.size(); j++) {
      word_grid.grid[i].push_back(line.at(j));
    }
    i ++;
  }
  word_grid.height = i;

  std::vector<Word> words = findInWordGrid(word_grid, TARGET);
  std::cout << "The word " << TARGET << " is present " << words.size() << " times. "<< std::endl;

  std::vector<Vector2D> xmas = findAllX_MAS(word_grid);
  std::cout << "There are " << xmas.size() << " X-MAS in the grid."<< std::endl;
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