#include <cstdio>
#include <string>

#include "../include/logger.hpp"

LOG_LEVEL Logger::level = L_VERBOSE;

int main() {
  std::string str = "Hello, World!";
  Logger::debug("Test %s", str.c_str());
  Logger::debug("Test length %zu", str.length());
  Logger::debug("Test '%d'", str[99]);
  if (str[99] == '\0') {
    Logger::info("test", "String is null-terminated");
  } else if (str[99] == ' ') {
    Logger::warn("test", "String is not null-terminated");
  } else {
    Logger::warn("test", "String is not null-terminated");
  }
  return 0;
}