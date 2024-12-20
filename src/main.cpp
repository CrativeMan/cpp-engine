#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "header/main.hpp"
#include "header/shader.hpp"
#include "header/userInterface.hpp"
#include "include/logger.hpp"

#define ID "ENGINE"

Global g;
float vertices[] = {
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // top right
    -0.5f, 0.5f,  0.0f, // top right
};
unsigned int indices[]{
    0, 1, 3, // first tirangle
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

  g.window = glfwCreateWindow(800, 600, "engine", NULL, NULL);
  if (g.window == NULL) {
    Logger::critical(ID, "Failed to initialize glfwWindow");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(g.window);
  glfwSetFramebufferSizeCallback(g.window, framebuffer_size_callback);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, 800, 600);

  ui::init(g.window, &g.show_demo_window);
  Logger::info(ID, "Initialized engine");
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  init();
  Shader shader("src/shader/vertex.glsl", "src/shader/fragment.glsl");

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(g.window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(g.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // input
    processInput(g.window);

    // rendering
    if (g.renderFrame)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    ui::render(true, g.show_demo_window);

    // check and call events and swap buffers
    glfwSwapBuffers(g.window);
    glfwPollEvents();
  }
  ui::shutdown();
  glDeleteProgram(shader.id);
  glfwDestroyWindow(g.window);
  glfwTerminate();
  return 0;
}
