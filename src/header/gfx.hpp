#ifndef GFX_H
#define GFX_H

#include "../header/shader.hpp"

#include "camera.hpp"
#include "model.hpp"
#include "systemMonitor.hpp"

#include <imgui_impl_glfw.h>
#include <vector>

namespace gfx {
void render(Shader *shader, Model *model, Camera *camera); // namespace gfx
}
namespace ui {
void init(GLFWwindow *w, bool *show_demo_window);
void shutdown();
void render(bool show_debug_window, bool show_demo_window,
            std::vector<unsigned int> texture, SystemMonitor sysMon);
} // namespace ui

#endif // GFX_H
