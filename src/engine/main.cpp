#include "header/camera.hpp"

#include "../include/logger.hpp"
#include "header/gfx.hpp"
#include "header/globals.h"
#include "header/main.hpp"
#include "header/model.hpp"
#include "header/shader.hpp"
#include <cstring>
#include <memory>
#include <vector>

#define ID "Engine"

Global g;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime, lastFrame, lastX, lastY;
bool firstMouse;
LOG_LEVEL Logger::level = L_MEDIUM;

/* ----------------------------------------------------------------------------
 * Callbacks
 * ----------------------------------------------------------------------------
 */
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void keyBoardInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    g.renderFrame = !g.renderFrame;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    g.trapMouse = false;
  camera.ProcessKeyboard(window, deltaTime);
}

void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
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

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  (void)window;
  (void)xoffset;
  if (g.trapMouse)
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  (void)window;
  (void)mods;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    g.trapMouse = true;
}

void events() {
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  g.sysMon.update();

  glfwPollEvents();

  if (g.trapMouse)
    glfwSetInputMode(g.window.id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (!g.trapMouse)
    glfwSetInputMode(g.window.id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (g.renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (!g.renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/* ----------------------------------------------------------------------------
 * Setup and main
 * ----------------------------------------------------------------------------
 */
void parseArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-d" || arg == "-debug") {
      Logger::level = L_VERBOSE;
      Logger::info(ID, "Set log level to verbose");
    }
  }
}

void init(int argc, char *argv[]) {
  if (!glfwInit()) {
    Logger::critical(ID, "Failed to initialize glfw");
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g.window.id = glfwCreateWindow(WIDTH, HEIGHT, g.window.title, NULL, NULL);
  if (g.window.id == NULL) {
    Logger::critical(ID, "Failed to initialize glfwWindow");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  Logger::info(ID, "Initialized window '%d'", g.window);
  glfwMakeContextCurrent(g.window.id);
  glfwSetFramebufferSizeCallback(g.window.id, framebufferSizeCallback);
  glViewport(0, 0, WIDTH, HEIGHT);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    Logger::critical(ID, "Failed to initialize GLEW");
    exit(EXIT_FAILURE);
  }

  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(g.window.id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  g.trapMouse = true;
  glfwSetCursorPosCallback(g.window.id, mouseCallback);
  glfwSetScrollCallback(g.window.id, scrollCallback);
  glfwSetMouseButtonCallback(g.window.id, mouseButtonCallback);

  ui::init(g.window.id, &g.show_demo_window);
  g.sysMon = SystemMonitor();
  parseArgs(argc, argv);
  Logger::info(ID, "Initialized engine");
}

void shutdown(Shader *shader) {
  ui::shutdown();
  glDeleteProgram(shader->id);
  glfwDestroyWindow(g.window.id);
  glfwTerminate();
  Logger::info(ID, "Shutdown engine");
};

int main(int argc, char *argv[]) {
  init(argc, argv);
  Shader shader("src/shader/vertex.glsl", "src/shader/fragment.glsl");
  Shader skyboxShader("src/shader/skyboxVertex.glsl",
                      "src/shader/skyboxFragment.glsl");

  std::vector<std::unique_ptr<Model>> models;

  models.push_back(
      std::make_unique<Model>("resources/model/backpack/backpack.obj"));
  models.push_back(std::make_unique<Model>("resources/model/test/pot.obj"));
  models.push_back(std::make_unique<Model>("resources/model/mipha/Mipha.dae"));
  models.push_back(std::make_unique<Model>("resources/model/dog/Dog.dae"));
  models.push_back(std::make_unique<Model>(
      "resources/model/korok/Korok/000/Npc_Korogu_M_000.dae"));

  Logger::info(ID, "Started rendering loop");
  while (!glfwWindowShouldClose(g.window.id)) {
    // Events
    events();
    if (glfwGetWindowAttrib(g.window.id, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // input
    keyBoardInput(g.window.id);

    // rendering
    gfx::render(&shader, models, &camera, &g.window);
    ui::render(true, g.show_demo_window, models, g.sysMon);

    glfwSwapBuffers(g.window.id);
  }

  // shutdown
  shutdown(&shader);
  return 0;
}
