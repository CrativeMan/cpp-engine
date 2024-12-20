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

typedef enum { L_NOTHING = 0, L_NORMAL, L_MEDIUM, L_VERBOSE } LOG_LEVEL;

class Logger {
public:
  static LOG_LEVEL level;

  static void debug(const std::string &id, const std::string &text) {
    log<L_VERBOSE>(id, "[DEBUG]\t", text);
  }
  static void info(const std::string &id, const std::string &text) {
    log<L_MEDIUM>(id, "[INFO]\t", text);
  }
  static void warn(const std::string &id, const std::string &text) {
    log<L_NORMAL>(id, YELLOW + std::string("[WARN]\t") + RESET, text);
  }
  static void error(const std::string &id, const std::string &text) {
    log<L_NORMAL>(id, RED + std::string("[ERROR]\t") + RESET, text, std::cerr);
  }
  static void critical(const std::string &id, const std::string &text) {
    log<L_NORMAL>(id, RED + std::string("[CRITICAL]\t") + RESET, text,
                  std::cerr);
  }

private:
  template <LOG_LEVEL L, typename Stream = std::ostream>
  static void log(const std::string &id, const std::string &prefix,
                  const std::string &text, Stream &stream = std::cout) {
    if (level >= L) {
      stream << print_id(id) << prefix << text << "\n";
    }
  }

  static std::string print_id(const std::string &id) {
    std::string rtrn;
    rtrn.append(time_as_string());
    rtrn.append("[" + id + "]");
    rtrn.append(" ");
    return rtrn;
  }

  static std::string time_as_string() {
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "[%H:%M:%S-%d/%m/%Y]", now);
    return std::string(buffer);
  }
};

#endif // LOGGER_H
