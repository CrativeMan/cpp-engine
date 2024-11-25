#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <sstream>
#include <string>
inline std::string file_to_string(std::string filepath) {
  std::ifstream file(filepath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

#endif // !FILE_HANDLER_H
