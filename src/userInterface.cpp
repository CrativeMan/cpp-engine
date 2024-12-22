#include <cstddef>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "header/gfx.hpp"
#include "header/systemMonitor.hpp"
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
  *show_demo_window = false;
  Logger::info("UI", "ImGui initialized");
}
void shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  Logger::info("UI", "ImGui shutdown");
}

/*
 * Windows
 */
void w_debugWindow(std::vector<unsigned int> textures, SystemMonitor sysMon) {
  ImGui::Begin("Debug Window");
  sysMon.render();
  if (ImGui::CollapsingHeader("Textures")) {
    ImGuiTreeNodeFlags flag =
        ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_SpanFullWidth;
    char buffer[100];
    for (size_t i = 0; i < textures.size(); i++) {
      sprintf((char *)buffer, "%d", textures[i]);
      if (ImGui::TreeNodeEx(buffer, flag, "%s", buffer)) {
        ImGui::Image(textures[i], ImVec2(100, 100));
        ImGui::TreePop();
      }
    }
  }
  ImGui::End();
}

/*
 * Rendering
 */
void render(bool show_debug_window, bool show_demo_window,
            std::vector<unsigned int> textures, SystemMonitor sysMon) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  if (show_debug_window)
    ui::w_debugWindow(textures, sysMon);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace ui
