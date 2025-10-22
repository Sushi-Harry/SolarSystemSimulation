#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ModelLoader.h"
#include "Skybox.h"

#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>
#include "Camera.h"
#include "SolarSystem.h"

// A struct to define the basic Engine object type
typedef struct Engine
{
    GLFWwindow* WINDOW;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    // For delta time calculations
    double deltaTime;
    double lastFrameTime;

    // SOlar system
    SolarSystem solar_system;

    // TEsting
    Model sphere_model;

    // Skybox
    Skybox spaceSkybox;

    // Globally required Data
    mat4 PROJECTION_MATRIX;
    Camera CAMERA;

} Engine;

// ------------------------------------------------------------Functions for the Engine object--------------------------------------------------------------//

// Static Helper functions
mat4* GET_PROJECTION_MATRIX(Engine* ENGINE);

// Functions for the Engine Object
bool ENGINE_INIT(Engine* ENGINE, int WIDTH, int HEIGHT);
void ENGINE_RUN(Engine* ENGINE);
void ENGINE_CLEANUP(Engine* ENGINE);