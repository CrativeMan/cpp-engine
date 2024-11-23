#include "include/logger.hpp"
#include <GL/glew.h>
#include <cstdlib>

#include "header/main.hpp"

#define ID "ENGINE"

Global g;
Logger LOGGER;

/* ----------------------------------------------------------------------------
 * Callbacks
 * ----------------------------------------------------------------------------
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

/* ----------------------------------------------------------------------------
 * Setup and main
 * ----------------------------------------------------------------------------
 */
void init() {
  LOGGER.level = MEDIUM;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g.window = glfwCreateWindow(800, 600, "engine", NULL, NULL);
  if (g.window == NULL) {
    LOGGER.critical(ID, "Failed to initialize glfwWindow");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(g.window);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(g.window, framebuffer_size_callback);

  LOGGER.info(ID, "Initialized engine");
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  init();

  while (!glfwWindowShouldClose(g.window)) {
    // input
    processInput(g.window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap buffers
    glfwSwapBuffers(g.window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
