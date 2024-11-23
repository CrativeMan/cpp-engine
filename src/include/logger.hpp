#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"

typedef enum { NOTHING = 0, NORMAL, MEDIUM, VERBOSE } LOG_LEVEL;

class Logger {
public:
  LOG_LEVEL level;

  void debug(std::string id, std::string text) {
    if (level >= VERBOSE)
      std::cout << print_id(id) << "[DEBUG]\t" << text << "\n";
  }
  void info(std::string id, std::string text) {
    if (level >= MEDIUM)
      std::cout << print_id(id) << "[INFO]\t" << text << "\n";
  }
  void warn(std::string id, std::string text) {
    if (level >= NORMAL)
      std::cout << YELLOW << print_id(id) << "[WARN]\t" << text << RESET
                << "\n";
  }
  void error(std::string id, std::string text) {
    if (level >= NORMAL)
      std::cerr << RED << print_id(id) << "[ERROR]\t" << text << RESET << "\n";
  }
  void critical(std::string id, std::string text) {
    if (level >= NORMAL)
      std::cerr << RED << print_id(id) << "[CRITICAL]\t" << text << RESET
                << "\n";
  }

private:
  std::string print_id(std::string id) {
    std::string rtrn;
    rtrn.append(time_as_string());
    rtrn.append("[" + id + "]");
    rtrn.append(" ");
    return rtrn;
  }

  std::string time_as_string() {
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "[%H:%M:%S-%d/%m/%Y]", now);
    return std::string(buffer);
  }
};

#endif // LOGGER_H
