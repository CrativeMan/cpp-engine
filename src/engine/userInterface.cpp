#include <cstddef>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "../include/logger.hpp"
#include "header/gfx.hpp"
#include "header/model.hpp"
#include "header/systemMonitor.hpp"

#define ID "Ui"

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
  Logger::info(ID, "ImGui initialized");
}
void shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  Logger::info(ID, "ImGui shutdown");
}

/*
 * Windows
 */
void w_debugWindow(std::vector<std::unique_ptr<Model>> &models,
                   SystemMonitor sysMon) {
  ImGui::Begin("Debug Window");
  sysMon.render();
  // all textures
  if (ImGui::CollapsingHeader("Textures")) {
    ImGuiTreeNodeFlags flag =
        ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_SpanFullWidth;
    char buffer[100];
    for (size_t j = 0; j < models.size(); j++) {
      Model *model = models[j].get();
      for (size_t i = 0; i < model->textureIds.size(); i++) {
        sprintf((char *)buffer, "%d", model->textureIds[i]);
        if (ImGui::TreeNodeEx(buffer, flag, "%s", buffer)) {
          ImGui::Image(model->textureIds[i], ImVec2(100, 100));
          ImGui::TreePop();
        }
      }
    }
  }

  // all models
  if (ImGui::CollapsingHeader("Models")) {
    char buffer[256];
    for (size_t i = 0; i < models.size(); i++) {
      Model *model = models[i].get();
      snprintf(buffer, sizeof(buffer), "Model '%s' %zu", model->file.c_str(),
               i);
      if (ImGui::TreeNode(buffer, "%s", buffer)) {
        ImGui::Text("Path: %s", model->path.c_str());
        ImGui::Text("Vertices: %d", model->numVertices);
        ImGui::Text("Indices: %d", model->numIndices);
        if (ImGui::TreeNode("Textures")) {
          for (size_t j = 0; j < model->textureIds.size(); j++) {
            unsigned int textureId = model->textureIds[j];
            snprintf(buffer, sizeof(buffer), "Texture %zu", j);
            if (ImGui::TreeNode(buffer, "%s", buffer)) {
              ImGui::Image(textureId, ImVec2(100, 100));
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }
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
            std::vector<std::unique_ptr<Model>> &models, SystemMonitor sysMon) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  if (show_debug_window)
    ui::w_debugWindow(models, sysMon);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace ui
