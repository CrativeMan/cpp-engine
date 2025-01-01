#ifndef GFX_HPP
#define GFX_HPP

#include "camera.hpp"
#include "main.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "systemMonitor.hpp"

#include <imgui_impl_glfw.h>
#include <memory>
#include <vector>

namespace gfx {
typedef struct {
  Shader *shader;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture;
} Skybox;

void render(Shader *shader, std::vector<std::unique_ptr<Model>> &models,
            Camera *camera,
            Window *window); // namespace gfx
} // namespace gfx
namespace ui {
void init(GLFWwindow *w, bool *show_demo_window);
void shutdown();
void render(bool show_debug_window, bool show_demo_window,
            std::vector<std::unique_ptr<Model>> &models, SystemMonitor sysMon);
} // namespace ui

#endif // GFX_HPP
