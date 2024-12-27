#include "../include/logger.hpp"

LOG_LEVEL Logger::level = L_VERBOSE;

int main() {
  Logger::testLogger();
  return 0;
}