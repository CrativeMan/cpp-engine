#ifndef GFX_H
#define GFX_H

#include "../header/shader.hpp"

#include "../header/main.hpp"
#include "systemMonitor.hpp"

#include <imgui_impl_glfw.h>
#include <vector>

namespace gfx {
void render(Global *g, Shader *ourShader, unsigned int VAO);
} // namespace gfx

namespace ui {
void init(GLFWwindow *w, bool *show_demo_window);
void shutdown();
void render(bool show_debug_window, bool show_demo_window,
            std::vector<unsigned int> texture, SystemMonitor sysMon);
} // namespace ui

#endif // GFX_H
