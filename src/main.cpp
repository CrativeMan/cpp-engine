#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "header/fileHandler.hpp"
#include "header/gfx.hpp"
#include "header/globals.h"
#include "header/main.hpp"
#include "header/shader.hpp"
#include "include/logger.hpp"

#define ID "ENGINE"

Global g;
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
LOG_LEVEL Logger::level = L_MEDIUM;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    g.renderFrame = !g.renderFrame;
}

/* ----------------------------------------------------------------------------
 * Setup and main
 * ----------------------------------------------------------------------------
 */
void init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g.window = glfwCreateWindow(WIDTH, HEIGHT, "engine", NULL, NULL);
  if (g.window == NULL) {
    Logger::critical(ID, "Failed to initialize glfwWindow");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  Logger::info(ID, "Initialized window '%d'", g.window);
  glfwMakeContextCurrent(g.window);
  glfwSetFramebufferSizeCallback(g.window, framebuffer_size_callback);
  glViewport(0, 0, WIDTH, HEIGHT);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  ui::init(g.window, &g.show_demo_window);
  g.sysMon = SystemMonitor();
  Logger::info("SystemMonitor", "SystemMonitor initialized");
  Logger::info(ID, "Initialized engine");
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  init();
  Shader ourShader("src/shader/vertex.glsl", "src/shader/fragment.glsl");

  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  g.texture.push_back(file::generateImage("src/imgs/container.jpg"));

  Logger::info(ID, "Started rendering loop");
  while (!glfwWindowShouldClose(g.window)) {
    // Events
    g.sysMon.update();
    glfwPollEvents();
    if (glfwGetWindowAttrib(g.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // input
    processInput(g.window);

    // rendering
    gfx::render(&g, &ourShader, VAO);

    // check and call events and swap buffers
    glfwSwapBuffers(g.window);
  }

  // shutdown
  ui::shutdown();
  glDeleteProgram(ourShader.id);
  glfwDestroyWindow(g.window);
  glfwTerminate();
  return 0;
}
