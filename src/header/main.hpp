#ifndef MAIN_HPP
#define MAIN_HPP

#include <GLFW/glfw3.h>
#include <vector>

#include "systemMonitor.hpp"

typedef struct {
  GLFWwindow *id;
  char title[100];
} Window;

typedef struct {
  Window window;
  SystemMonitor sysMon;
  std::vector<unsigned int> texture;
  bool show_demo_window;
  bool renderFrame;
  bool trapMouse = true;
} Global;

#endif // MAIN_HPP
