#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct {
  int parent;
  int x;
  int y;
  char label;
  int perimeter;
  int area;
  int sides;
  bool fences[4]; // N S E W
} Plot;

struct Map {
  std::vector<Plot> fields;
  int width;
  int height;
};

int find(int i, std::vector<Plot> fields) 
{ 
  if (fields[i].parent == i) { 
      return i; 
  } 
  else { 
      int result = find(fields[i].parent, fields);
      fields[i].parent = result;
      return  result;
  } 
}

void uni(int i, int j, std::vector<Plot> &fields)
{   
  int irep = find(i, fields);
  int jrep = find(j, fields); 

  if(fields[irep].parent != jrep) {
    fields[irep].parent = jrep;
    fields[jrep].area += fields[irep].area;
    fields[jrep].perimeter += fields[irep].perimeter;
    fields[jrep].sides += fields[irep].sides;
  }
}

void plotPerimeter(int i, Map &map)
{
  Plot &p = map.fields[i];
  int perimeter = 0;
  if(p.y <= 0 || map.fields[i - map.width].label != p.label) {
    perimeter += 1;
    p.fences[0] = true;
  }
  if(p.y >= map.height - 1 || map.fields[i + map.width].label != p.label) {
    perimeter += 1;
    p.fences[1] = true;
  }
  if(p.x >= map.width - 1 || map.fields[i + 1].label != p.label) {
    perimeter += 1;
    p.fences[2] = true;
  }
  if(p.x <= 0 || map.fields[i - 1].label != p.label) {
    perimeter += 1;
    p.fences[3] = true;
  }
  p.perimeter = perimeter;
}

void plotSides(int i, Map &map)
{
  Plot &p = map.fields[i];
  int sides = 0;
  if(p.fences[0]) {
    if(!(p.x > 0 && map.fields[i-1].label == p.label && map.fields[i-1].fences[0])) {
      sides += 1;
    }
  }
  if(p.fences[1]) {
    if(!(p.x > 0 && map.fields[i-1].label == p.label && map.fields[i-1].fences[1])) {
      sides += 1;
    }
  }
  if(p.fences[2]) {
    if(!(p.y > 0 && map.fields[i-map.width].label == p.label && map.fields[i-map.width].fences[2])) {
      sides += 1;
    }
  }
  if(p.fences[3]) {
    if(!(p.y > 0 && map.fields[i-map.width].label == p.label && map.fields[i-map.width].fences[3])) {
      sides += 1;
    }
  }
  p.sides = sides;
}

void mergePlots(Map &map)
{
  for(int i=0; i<map.fields.size(); i++)
  {
    Plot f = map.fields[i];
    plotPerimeter(i, map);
    plotSides(i, map);
    if(f.x > 0 && map.fields[i - 1].label == f.label) {
      uni(i, i-1, map.fields);
    }
    if(f.y > 0 && map.fields[i - map.width].label == f.label) {
      uni(i, i - map.width, map.fields);
    }
  }
}


int solve(std::ifstream &file)
{
  std::string line;
  Map map;
  map.height = 0;
  map.width = 0;
  while (std::getline(file, line)) {
    std::cout << line << std::endl;

    if(line.size() > map.width) {
      map.width = line.size();
    }

    for(int j=0; j<line.size(); j++) {
      Plot f {
        (int)map.fields.size(),
        j,
        map.height,
        line[j],
        0,
        1,
        0,
        {false, false, false, false}
      };
      map.fields.push_back(f);
    }
    map.height++;
  }
  mergePlots(map);
  std::cout << "Computing farm price..." << std::endl;
  int price_sum = 0;
  int price_discount_sum = 0;
  for(int i=0; i<map.fields.size(); i++) {
    Plot f = map.fields[i];
    if(f.parent == i){
      int price = f.area * f.perimeter;
      int price_discount = f.area * f.sides;
      price_sum += price;
      price_discount_sum += price_discount;
      std::cout << f.label << ": " << f.area << " * " << f.perimeter << " = " << price  << " | " << f.area << " * " << f.sides << " = " << price_discount << std::endl;;
    }
  }
  std::cout << "Total price of fencing all regions: " << price_sum << std::endl;
  std::cout << "Total price of fencing all regions, with discount: " << price_discount_sum << std::endl;

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