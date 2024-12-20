#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class FileHandler {
public:
  static const char *readFile(std::string filepath);
};

#endif // !FILE_HANDLER_H
