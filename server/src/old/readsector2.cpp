#include <fstream>
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdint.h>


int readSector(char *diskName, uint32_t sector) {
	// Which disk?
	std::string diskError = std::string() + diskName + ": ";

	// Open device file
	std::ifstream disk(diskName, std::ios_base::binary);

	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));

	// Seek to 54321'th sector
	disk.seekg(512 * sector);
	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));

	// Read in one sector
	std::vector<char> buffer(512);
	disk.read(&buffer[0], 512);
	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));
}

/*
int writeSector(std::string diskName, uint32_t sector, std::string toWrite) {
	// Which disk?
	std::string diskError = std::string() + diskName + ": ";

	// Open device file
	std::ofstream disk(diskName);

	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));

	// Seek to 54321'th sector
	//disk.seekg(512 * sector);
	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));

	// Read in one sector
	std::vector<char> buffer(512);
	disk.write(&buffer[0], 512);
	if(!disk)
		throw(std::runtime_error(diskError + std::strerror(errno)));
}
*/
int main() {
	//writeSector("/dev/sdb", 0, "try to write on disk");
	readSector("/dev/testlo", 1);
	return 0;
}
