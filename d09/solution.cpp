#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void defragment(std::vector<int> &data, bool fullFiles = false) 
{
   for(int i=data.size() - 1; i>= 0; i--) {
    if(data[i] != -1) {
      int block_size = 1;
      int block_start = 0;
      if(fullFiles) {
        while(i - block_size >= 0 && data[i - block_size] == data[i]) {
          block_size ++;
        }
      }
      block_start = i - block_size + 1;
      for(int j=0; j<i; j++) {
        if(data[j] == -1) {
          int empty_size = 1;
          while(j + empty_size < data.size() && data[j + empty_size] == data[j]) {
            empty_size ++;
          }
          if(empty_size >= block_size) {
            for(int k=0; k<block_size; k++) {
              data[j+k] = data[block_start+k];
              data[block_start+k] = -1;
            }
            break;
          }
        }
      }
      i -= block_size - 1;
    }  
  }
}

void printData(std::vector<int> &data)
{
  for(int i=0; i<data.size(); i++) {
    if(data[i] == -1) {
      std::cout << '.';
    } else {
      std::cout << data[i];
    }
  }
  std::cout << std::endl;
}

unsigned long long checksum(std::vector<int> &data)
{
  unsigned long long sum = 0;
  for(int i=0; i<data.size(); i++) {
    if(data[i] != -1) {
      sum += i * data[i];
    }
  }
  return sum;
}

int solve(std::ifstream &file)
{
  std::string line;
  std::getline(file, line);

  std::vector<int> data;
  std::vector<int> data_full_blocks;


  int current_pos = 0;
  int current_file_ID = 0;
  for(int i=0; i<line.length(); i++) {
    int block_size = line[i] - '0';
    if(i%2 == 0) {
      for(int j=0; j<block_size; j++) {
        data.push_back(current_file_ID);
      }

      current_file_ID ++;
    } else {
      for(int j=0; j<block_size; j++) {
        data.push_back(-1);
      }
    }
    current_pos += block_size;
  }

  data_full_blocks = data;

  printData(data);

  defragment(data);
  defragment(data_full_blocks, true);

 
  std::cout << checksum(data) << std::endl;
  std::cout << checksum(data_full_blocks) << std::endl;

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