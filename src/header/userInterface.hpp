#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <imgui_impl_glfw.h>

namespace ui {
void init(GLFWwindow *w, bool *show_demo_window);
void shutdown();
void render(bool show_debug_window, bool show_demo_window,
            unsigned int texture);
} // namespace ui

#endif // USER_INTERFACE_