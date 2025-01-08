#include <GL/glew.h>

#include "../include/logger.hpp"
#include "header/levelBuilderUi.hpp"

#include <GLFW/glfw3.h>
#include <string>

#define ID "Level"

struct LevelEditor {
  GLFWwindow *wid;
  char title[1024];
};

static struct LevelEditor g;

LOG_LEVEL Logger::level = L_VERBOSE;
levelBuilder_Ui levelUi();

void init(int argc, char *argv[]) {
  if (!glfwInit()) {
    Logger::critical(ID, "Failed to initialize glfw");
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g.wid = glfwCreateWindow(1200, 700, g.title, NULL, NULL);
  if (g.wid == NULL) {
    Logger::critical(ID, "Failed to initialize glfwWindow");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(g.wid);
  glViewport(0, 0, 1200, 700);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    Logger::critical(ID, "Failed to initialize GLEW");
    exit(EXIT_FAILURE);
  }

  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(g.wid, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Logger::info(ID, "Initialized engine");
}

int main(int argc, char *argv[]) {
  init(argc, argv);
  levelUi().init(g.wid);

  glfwPollEvents();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.1, 0.1, 0.1, 1.0);

  levelUi().shutdown();
  return 0;
}
