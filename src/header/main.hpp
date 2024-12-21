#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <vector>

#include "systemMonitor.hpp"

typedef struct {
  GLFWwindow *window;
  SystemMonitor sysMon;
  std::vector<unsigned int> texture;
  bool show_demo_window;
  bool renderFrame;
} Global;

#endif // MAIN_H
