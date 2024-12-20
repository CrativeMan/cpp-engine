#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <vector>

typedef struct {
  GLFWwindow *window;
  std::vector<unsigned int> texture;
  bool show_demo_window;
  bool renderFrame;
} Global;

#endif // MAIN_H
