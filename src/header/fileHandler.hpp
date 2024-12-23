#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP
#include <string>

namespace file {
void initFileHandler();
std::string loadStringFromFile(const char *path);
unsigned int generateImage(const char *path, const std::string &directory);
} // namespace file
#endif // FILE_HANDLER_HPP
