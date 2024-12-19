#include <fstream>
#include <iostream>

#include "include/logger.hpp"

#define ID "File Handler"

const char *readFile(const std::string &filePath, size_t &fileSize) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);

  if (!file.is_open()) {
    LOGGER.critical(ID, "Failed to open file: " + filePath);
    return nullptr;
  }

  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  char *buffer = new char[fileSize + 4]; // Allocate memory dynamically

  if (!file.read(buffer, fileSize)) {
    LOGGER.error(ID, "Failed to read file" + filePath);
    delete[] buffer; // Clean up allocated memory
    return nullptr;
  }

  return buffer; // Return the file content as const char*
}
