#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdarg>
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

  static void debug(const std::string &id, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log<L_VERBOSE>(id, "[DEBUG]\t", format, args);
    va_end(args);
  }
  static void info(const std::string &id, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log<L_MEDIUM>(id, "[INFO]\t", format, args);
    va_end(args);
  }
  static void warn(const std::string &id, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log<L_NORMAL>(id, YELLOW + std::string("[WARN]\t") + RESET, format, args);
    va_end(args);
  }
  static void error(const std::string &id, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log<L_NORMAL>(id, RED + std::string("[ERROR]\t") + RESET, format, args,
                  std::cerr);
    va_end(args);
  }
  static void critical(const std::string &id, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log<L_NORMAL>(id, RED + std::string("[CRITICAL]\t") + RESET, format, args,
                  std::cerr);
    va_end(args);
  }

private:
  template <LOG_LEVEL L, typename Stream = std::ostream>
  static void log(const std::string &id, const std::string &prefix,
                  const char *format, va_list args,
                  Stream &stream = std::cout) {
    if (level >= L) {
      stream << print_id(id) << prefix << format_string(format, args) << "\n";
    }
  }

  static std::string format_string(const char *format, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    return std::string(buffer);
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

#endif // LOGGER_HPP