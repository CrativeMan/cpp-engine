#include "header/fileHandler.hpp"
#include "include/logger.hpp"

#define ID "File Handler"

const char *FileHandler::readFile(std::string filepath) {
  FILE *file = fopen(filepath.c_str(), "rb");
  if (file == NULL) {
    Logger::critical(ID, "Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long unsigned int fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL) {
    Logger::critical(ID, "Failed to alloc buffers");
    fclose(file);
    return NULL;
  }

  size_t readSize = fread(buffer, 1, fileSize, file);
  if (readSize != fileSize) {
    Logger::critical(ID, "Failed to read file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[fileSize] = '\0';
  fclose(file);
  const char *fileContents = (const char *)buffer;

  Logger::info(ID, "Generated char* from file");
  Logger::info(ID, filepath);
  return fileContents;
}
