#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP
#include <string>
#include <vector>

namespace file {
std::string loadStringFromFile(const char *path);
unsigned int generateCubeMap(std::vector<std::string> faces);
unsigned int generateImage(const char *path, const std::string &directory);
} // namespace file
#endif // FILE_HANDLER_HPP
