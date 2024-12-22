#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "header/camera.hpp"
#include "header/fileHandler.hpp"
#include "header/gfx.hpp"
#include "header/globals.h"
#include "header/main.hpp"
#include "header/model.hpp"
#include "header/shader.hpp"
#include "include/logger.hpp"

#define ID "ENGINE"

Global g;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime, lastFrame, lastX, lastY;
bool firstMouse;
LOG_LEVEL Logger::level = L_MEDIUM;

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
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    g.renderFrame = !g.renderFrame;
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    g.trapMouse = !g.trapMouse;
  camera.ProcessKeyboard(window, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  (void)window;
  if (g.trapMouse) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  (void)window;
  (void)xoffset;
  if (g.trapMouse)
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void events() {
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  g.sysMon.update();

  glfwPollEvents();

  if (g.trapMouse)
    glfwSetInputMode(g.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (!g.trapMouse)
    glfwSetInputMode(g.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (g.renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (!g.renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

  glfwSetInputMode(g.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  g.trapMouse = true;
  glfwSetCursorPosCallback(g.window, mouse_callback);
  glfwSetScrollCallback(g.window, scroll_callback);

  file::initFileHandler();
  ui::init(g.window, &g.show_demo_window);
  g.sysMon = SystemMonitor();
  Logger::info("SystemMonitor", "SystemMonitor initialized");
  Logger::info(ID, "Initialized engine");
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  init();
  Shader shader("src/shader/vertex.glsl", "src/shader/fragment.glsl");
  Model model("resources/model/backpack/backpack.obj");

  Logger::info(ID, "Started rendering loop");
  while (!glfwWindowShouldClose(g.window)) {
    // Events
    events();
    if (glfwGetWindowAttrib(g.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // input
    processInput(g.window);

    // rendering
    gfx::render(&shader, &model, &camera);
    ui::render(true, g.show_demo_window, model.textureIds, g.sysMon);

    glfwSwapBuffers(g.window);
  }

  // shutdown
  ui::shutdown();
  glDeleteProgram(shader.id);
  glfwDestroyWindow(g.window);
  glfwTerminate();
  return 0;
}
