#include "GUI.h"

char name[1024] = "";
char specularPath[1024] = "";
char diffusePath[1024] = "";
float radius = 0.0f;
float mass = 0.0f;
float orbitalRadius = 0.0f;
int parent_index = 0;
int moon_edit_index = 0, planet_edit_index = 0;
GUI_STATE gui_state = DISPLAY_DATA_MODE;

void GUI_INIT(GLFWwindow* WINDOW){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WINDOW, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ADD_PLANET(SolarSystem* SOLAR_SYSTEM){
    ImGui::InputText("Planet Name", name, IM_ARRAYSIZE(name));
    ImGui::InputText("Path to Planet's Specular Texture", specularPath, IM_ARRAYSIZE(specularPath));
    ImGui::InputText("Path to Planet's Diffuse Texture", diffusePath, IM_ARRAYSIZE(diffusePath));
    ImGui::InputFloat("RADIUS of Planet: ", &radius, 0.01f, 0.05f, "%.3f");
    ImGui::InputFloat("MASS of Planet: ", &mass, 0.01f, 0.05f, "%.3f");
    ImGui::InputFloat("ORBITAL RADIUS of Planet: ", &orbitalRadius, 0.01f, 0.05f, "%.3f");
    if(ImGui::Button("ADD PLANET")){
        if(name != "" && specularPath != "" && diffusePath != "" && radius != 0.0f && mass != 0.0f && orbitalRadius != 0.0f){
            PLANET_INIT(&SOLAR_SYSTEM->PLANETS[SOLAR_SYSTEM->PLANET_COUNT], &SOLAR_SYSTEM->CENTRAL_STAR, radius, mass, orbitalRadius, diffusePath, specularPath, name, "models/sphere/sphere.gltf");
            SOLAR_SYSTEM->PLANET_COUNT++;
            gui_state = DISPLAY_DATA_MODE;
        }
    }
}

void ADD_MOON(SolarSystem* SOLAR_SYSTEM){
    ImGui::InputText("Moon Name", name, IM_ARRAYSIZE(name));
    ImGui::InputText("Path to Moon's Specular Texture", specularPath, IM_ARRAYSIZE(specularPath));
    ImGui::InputText("Path to Moon's Diffuse Texture", diffusePath, IM_ARRAYSIZE(diffusePath));
    ImGui::InputFloat("RADIUS of Moon: ", &radius, 0.01f, 0.05f, "%.3f");
    ImGui::InputFloat("MASS of Moon: ", &mass, 0.01f, 0.05f, "%.3f");
    ImGui::InputFloat("ORBITAL RADIUS of Moon: ", &orbitalRadius, 0.01f, 0.05f, "%.3f");
    ImGui::SliderInt("Parent Planet Index", &parent_index, 0, SOLAR_SYSTEM->PLANET_COUNT - 1, "%d");
    if(ImGui::Button("ADD MOON")){
        if(name != "" && specularPath != "" && diffusePath != "" && radius != 0.0f && mass != 0.0f && orbitalRadius != 0.0f){
            MOON_INIT(&SOLAR_SYSTEM->MOONS[SOLAR_SYSTEM->MOON_COUNT], &SOLAR_SYSTEM->PLANETS[parent_index], radius, mass, orbitalRadius, diffusePath, specularPath, name, "models/sphere/sphere.gltf");
            SOLAR_SYSTEM->MOON_COUNT++;
            gui_state = DISPLAY_DATA_MODE;
        }
    }
}

void EDIT_PLANET(SolarSystem *SOLAR_SYSTEM){
    if(SOLAR_SYSTEM->PLANET_COUNT > 0){
        ImGui::SliderInt("Planet Index To Edit", &planet_edit_index, 0, SOLAR_SYSTEM->PLANET_COUNT - 1);
        ImGui::SliderFloat("Radius of The Planet", &SOLAR_SYSTEM->PLANETS[planet_edit_index].RADIUS, 0.1f, 5.0f);
        ImGui::SliderFloat("Mass of The Planet", &SOLAR_SYSTEM->PLANETS[planet_edit_index].MASS, 0.1f, 800.0f);
        ImGui::SliderFloat("Orbital Radius of The Planet", &SOLAR_SYSTEM->PLANETS[planet_edit_index].ORBITAL_RADIUS, 0.0f, 500.0f);
    
        if(ImGui::Button("Update Data")){
            CALCULATE_PLANET_ORBITAL_DATA(&SOLAR_SYSTEM->PLANETS[planet_edit_index]);
            gui_state = DISPLAY_DATA_MODE;
        }
    }else{
        ImGui::Text("There Are No Planets to Edit.");
    }
}

void EDIT_MOON(SolarSystem *SOLAR_SYSTEM){

    if(SOLAR_SYSTEM->MOON_COUNT > 0){
        ImGui::SliderInt("Moon Index To Edit", &moon_edit_index, 0, SOLAR_SYSTEM->MOON_COUNT - 1);
        ImGui::SliderFloat("Radius of The Moon", &SOLAR_SYSTEM->MOONS[moon_edit_index].RADIUS, 0.1f, 1.0f);
        ImGui::SliderFloat("Mass of The Moon", &SOLAR_SYSTEM->MOONS[moon_edit_index].MASS, 0.1f, 200.0f);
        ImGui::SliderFloat("Orbital Radius of The Moon", &SOLAR_SYSTEM->MOONS[moon_edit_index].ORBITAL_RADIUS, 0.0f, 20.0f);
    
        if(ImGui::Button("Update Data")){
            CALCULATE_MOON_ORBITAL_DATA(&SOLAR_SYSTEM->MOONS[moon_edit_index]);
            gui_state = DISPLAY_DATA_MODE;
        }
    }else{
        ImGui::Text("There Are No Moons to Edit.");
    }
}

void DISPLAY_DATA(SolarSystem *SOLAR_SYSTEM){
    /*--------------PLANET DATA DISPLAY---------------------*/
    ImGui::TextColored((ImVec4){0.0, 1.0, 1.0, 1.0}, "\nPLANETS:\n");
    for(int i = 0; i < SOLAR_SYSTEM->PLANET_COUNT; i++){
        ImGui::Text("Name of Planet %d = %s", i+1, SOLAR_SYSTEM->PLANETS[i].NAME);
        ImGui::Text("Radius of %s = %.3f", SOLAR_SYSTEM->PLANETS[i].NAME, SOLAR_SYSTEM->PLANETS[i].RADIUS);
        ImGui::Text("Rotation Speed (Angular) of %s = %.3f", SOLAR_SYSTEM->PLANETS[i].NAME, SOLAR_SYSTEM->PLANETS[i].ROTATION_SPEED);
        ImGui::Text("Orbital Speed (Angular) of %s = %.3f", SOLAR_SYSTEM->PLANETS[i].NAME, SOLAR_SYSTEM->PLANETS[i].ORBITAL_SPEED);
        ImGui::Text("\n");
    }
    ImGui::TextColored((ImVec4){0.0, 1.0, 1.0, 1.0}, "\n\nMOONS:\n");
    for(int i = 0; i < SOLAR_SYSTEM->MOON_COUNT; i++){
        ImGui::Text("Name of Parent Planet = %s", SOLAR_SYSTEM->MOONS[i].PARENT_PLANET->NAME);
        ImGui::Text("Name of Moon %d = %s", i+1, SOLAR_SYSTEM->MOONS[i].NAME);
        ImGui::Text("Radius of %s = %.3f", SOLAR_SYSTEM->MOONS[i].NAME, SOLAR_SYSTEM->MOONS[i].RADIUS);
        ImGui::Text("Rotation Speed (Angular) of %s = %.3f", SOLAR_SYSTEM->MOONS[i].NAME, SOLAR_SYSTEM->MOONS[i].ROTATION_SPEED);
        ImGui::Text("Orbital Speed (Angular) of %s = %.3f", SOLAR_SYSTEM->MOONS[i].NAME, SOLAR_SYSTEM->MOONS[i].ORBITAL_SPEED);
        ImGui::Text("\n");
    }
}

void GUI_RENDER(SolarSystem* SOLAR_SYSTEM){

    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("MENU");

    const char *enumNames[] = {
        "_ADD_MOON",
        "_ADD_PLANET",
        "EDIT_PLANET_DATA",
        "EDIT_MOON_DATA",
        "DISPLAY_DATA_MODE"
    };
    static GUI_STATE currentState = DISPLAY_DATA_MODE;
    if(ImGui::Combo("MODE", (int*)&currentState, enumNames, IM_ARRAYSIZE(enumNames))){
        gui_state = currentState;
    }
    switch(currentState){
        case _ADD_PLANET:
            ADD_PLANET(SOLAR_SYSTEM);
            break;

        case _ADD_MOON:
            ADD_MOON(SOLAR_SYSTEM);
            break;
        case EDIT_PLANET_DATA:
            EDIT_PLANET(SOLAR_SYSTEM);
            break;
        case EDIT_MOON_DATA:
            EDIT_MOON(SOLAR_SYSTEM);
            break;
        default:
            DISPLAY_DATA(SOLAR_SYSTEM);
        break;
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}