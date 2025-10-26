#include "GUI.h"

void GUI_INIT(GLFWwindow* WINDOW){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WINDOW, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI_RENDER(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("MENUS");
        ImGui::Text("THIS IS A WINDOW");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}