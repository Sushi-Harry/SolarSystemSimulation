#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include "SolarSystem.h"

typedef enum {
    _ADD_MOON,
    _ADD_PLANET,
    EDIT_PLANET_DATA,
    EDIT_MOON_DATA
} GUI_STATE;

/*
This file also includes functions to get and set values for different member variables of the structs defined
in SolarSystem.h

Editable Variables (Via GUI):
Planet: -> Radius, mass, orbital radius
Moon: -> Radius, Mass, Orbital Radius

and some object addition functions
*/

void GUI_INIT(GLFWwindow* WINDOW);
void ADD_PLANET(SolarSystem *SOLAR_SYSTEM);
void ADD_MOON(SolarSystem* SOLAR_SYSTEM);
void EDIT_PLANET(SolarSystem *SOLAR_SYSTEM);
void EDIT_MOON(SolarSystem *SOLAR_SYSTEM);
void GUI_RENDER(SolarSystem* SOLAR_SYSTEM);