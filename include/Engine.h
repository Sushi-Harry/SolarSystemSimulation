#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

#include "ModelLoader.h"

#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>

// These static variables store the mouse state between calls to the callback
static float lastX = 400.0f; // Initialize to center of a hypothetical 800x600 window
static float lastY = 300.0f;
static bool firstMouse = true;

// A struct to define the basic Engine object type
typedef struct Engine
{
    GLFWwindow* WINDOW;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    // For delta time calculations
    double deltaTime;
    double lastFrameTime;

    // Test Model
    Model Sphere;

    // Globally required Data
    mat4* PROJECTION_MATRIX;
    Camera* CAMERA;

} Engine;

// ------------------------------------------------------------Functions for the Engine object--------------------------------------------------------------//

// Static Helper functions
static void processInput(Engine* ENGINE);
static void update(Engine* ENGINE);
mat4* PROJECTION_MATRIX(Engine* ENGINE);
static void framebuffer_size_callback(GLFWwindow* WINDOW, int WIDTH, int HEIGHT);
static void mouse_callback(GLFWwindow* WINDOW, double xPos, double yPos);

// Functions for the Engine Object
bool ENGINE_INIT(Engine* ENGINE, int WIDTH, int HEIGHT){
    if(!glfwInit()){
        printf("ERROR::GLFW_INITIALIZATION_FAILED\n");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ENGINE->WINDOW_WIDTH = WIDTH;
    ENGINE->WINDOW_HEIGHT = HEIGHT;

    // Creating the window object
    ENGINE->WINDOW = glfwCreateWindow(ENGINE->WINDOW_WIDTH, ENGINE->WINDOW_HEIGHT, "SOLAR_SYS_SIM", NULL, NULL);

    // Ensuring that the window was created successfully
    if(ENGINE->WINDOW == NULL){
        printf("ERROR::WINDOW_INITIALIZATION_FAILED\n");
        return false;
    }
    glfwMakeContextCurrent(ENGINE->WINDOW);

    /*----------------GLEW INITIALIZATION---------------------*/
    if(glewInit() != GLEW_OK){
        printf("ERROR::GLEW_INITIALIZATION_FAILED\n");
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, ENGINE->WINDOW_WIDTH, ENGINE->WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    
    // Setting window user pointer
    glfwSetWindowUserPointer(ENGINE->WINDOW, ENGINE);

    glfwSetFramebufferSizeCallback(ENGINE->WINDOW, framebuffer_size_callback);
    glfwSetCursorPosCallback(ENGINE->WINDOW, mouse_callback);

    /* ---- Delta time calculations: Setup------*/
    ENGINE->deltaTime = 0.0;
    ENGINE->lastFrameTime = 0.0;
    /*--------------------------------------------------TESTING---------------------------------------------*/
    MODEL(&(ENGINE->Sphere), "models/Sphere/MetalRoughSpheres.gltf", "celestialBody.vs", "celestialBody.fs", "models/Sphere/Spheres_BaseColor.png", "models/Sphere/Spheres_MetalRough.png");

    /*--------------------SETTING THE PROJECTION MATRIX---------------------------*/
    glm_perspective(glm_rad(90.0), (float)ENGINE->WINDOW_WIDTH / (float)ENGINE->WINDOW_HEIGHT, 0.1, 1000.0, *(ENGINE->PROJECTION_MATRIX));
    return true;
}

void ENGINE_RUN(Engine* ENGINE){
    while(!glfwWindowShouldClose(ENGINE->WINDOW)){
        
        /*-------------------Clearing the buffer-------------------------*/
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /*-----------------DELTA TIME CALCULATIONS---------------------*/
        double currentFrame = glfwGetTime();
        ENGINE->deltaTime = currentFrame - ENGINE->lastFrameTime;
        ENGINE->lastFrameTime = currentFrame;

        /*----------INPUT------------*/
        // processInput(ENGINE);

        /*----------UPDATE------------*/
        // update(ENGINE);

        /*---------This is the end of the current frame--------------*/
        glfwSwapBuffers(ENGINE->WINDOW);
        glfwPollEvents();
    }
}

void ENGINE_CLEANUP(Engine* ENGINE){
    glfwTerminate();
}

/*--------------HELPER FUNCTION DEFINITIONS------------------*/
mat4* PROJECTION_MATRIX(Engine* ENGINE){
    return (ENGINE->PROJECTION_MATRIX);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Retrieve the pointer to your Engine struct (which contains the camera)
    // You must set this with glfwSetWindowUserPointer(window, &myEngine); in main()
    Engine* ENGINE = (Engine*)glfwGetWindowUserPointer(window);
    if (ENGINE == NULL) {
        return;
    }

    // This check prevents a large camera jump on the first mouse input
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate the mouse's movement since the last frame
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    // Update the last positions for the next frame
    lastX = xpos;
    lastY = ypos;

    // Use your existing function to process the movement
    PROCESS_MOUSE_MOVEMENT(ENGINE->CAMERA, xoffset, yoffset, true);
}

static void framebuffer_size_callback(GLFWwindow* WINDOW, int WIDTH, int HEIGHT) {
    Engine* engine = (Engine*)glfwGetWindowUserPointer(WINDOW);
    
    engine->WINDOW_WIDTH = WIDTH;
    engine->WINDOW_HEIGHT = HEIGHT;
    glViewport(0, 0, WIDTH, HEIGHT);
}
