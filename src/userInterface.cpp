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
void w_debugWindow(unsigned int texture) {
  ImGui::Begin("Debug Window");
  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Text("Application average %.3f ms/f",
              1000.0f / ImGui::GetIO().Framerate);
  if (ImGui::CollapsingHeader("Images")) {
    ImGui::Image(texture, ImVec2(100, 100));
  }
  ImGui::End();
}

/*
 * Rendering
 */
void render(bool show_debug_window, bool show_demo_window,
            unsigned int texture) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  if (show_debug_window)
    ui::w_debugWindow(texture);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace ui