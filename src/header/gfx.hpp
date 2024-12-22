#ifndef GFX_HPP
#define GFX_HPP

#include "camera.hpp"
#include "main.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "systemMonitor.hpp"

#include <imgui_impl_glfw.h>
#include <vector>

namespace gfx {
void render(Shader *shader, Model *model, Camera *camera,
            Window *window); // namespace gfx
}
namespace ui {
void init(GLFWwindow *w, bool *show_demo_window);
void shutdown();
void render(bool show_debug_window, bool show_demo_window,
            std::vector<unsigned int> textures, SystemMonitor sysMon);
} // namespace ui

#endif // GFX_HPP
