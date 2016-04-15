#include <fstream>
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <vector>

int main() {
  // Which disk?
  char diskName[] = "/dev/loop0";
  std::string diskError = std::string() + diskName + ": ";

  // Open device file
  std::ifstream disk(diskName, std::ios_base::binary);

  if(!disk)
    throw(std::runtime_error(diskError + std::strerror(errno)));

  // Seek to 54321'th sector
  disk.seekg(512 * 54321);
  if(!disk)
    throw(std::runtime_error(diskError + std::strerror(errno)));

  // Read in one sector
  std::vector<char> buffer(512);
  disk.read(&buffer[0], 512);
  if(!disk)
    throw(std::runtime_error(diskError + std::strerror(errno)));
   std::cout << buffer;
}
