#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

void GUI_INIT(GLFWwindow* WINDOW);
void GUI_RENDER();