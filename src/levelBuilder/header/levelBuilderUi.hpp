#ifndef USERINTERFACE_LEVEL_HPP
#define USERINTERFACE_LEVEL_HPP
#include "../../include/userInterface.hpp"
#include <imgui.h>

class levelBuilder_Ui : public I_userInterface {
  void renderUI() { ImGui::Text("Test"); }
};

#endif // USERINTERFACE_LEVEL_HPP
