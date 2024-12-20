#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "include/logger.hpp"

namespace ui {
/*
 * Initialization
 */
void init(GLFWwindow *w, bool *show_demo_window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(w, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  *show_demo_window = true;
  Logger::info("UI", "ImGui initialized");
}
void shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

/*
 * Windows
 */
void mainWindow() {
  ImGui::Begin("Main Window");
  ImGui::TextColored(ImVec4(0.0f, 0.62f, 0.66f, 1.0f), "C++ Engine");
  ImGui::Text("Application average %.3f ms/f\n(%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

/*
 * Rendering
 */
void render(bool show_window, bool show_demo_window) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  if (show_window) {
    ui::mainWindow();
  }
  ImGui::Render();
}
void renderEnd() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }
} // namespace ui
