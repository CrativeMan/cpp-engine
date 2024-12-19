/*
   Created by Crative (github.com/CrativeMan)

   MIT License

   Copyright (c) 2024 Sebastian Hannig

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

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

static Logger LOGGER;

#endif // LOGGER_H
