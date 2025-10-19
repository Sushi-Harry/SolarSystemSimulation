#include "Engine.h"

// --- PRIVATE STATE VARIABLES ---
// These are only visible inside this file.
static float lastX;
static float lastY;
static bool firstMouse = true;

// --- PRIVATE HELPER FUNCTION PROTOTYPES ---
// Declare the static functions here, at the top of the .c file.
static void processInput(Engine* ENGINE);
static void framebuffer_size_callback(GLFWwindow* WINDOW, int WIDTH, int HEIGHT);
static void mouse_callback(GLFWwindow* WINDOW, double xPos, double yPos);

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
    lastX = (float)WIDTH / 2.0;
    lastY = (float)HEIGHT / 2.0;

    // Creating the window object
    ENGINE->WINDOW = glfwCreateWindow(ENGINE->WINDOW_WIDTH, ENGINE->WINDOW_HEIGHT, "SOLAR_SYS_SIM", NULL, NULL);

    // Ensuring that the window was created successfully
    if(ENGINE->WINDOW == NULL){
        printf("ERROR::WINDOW_INITIALIZATION_FAILED\n");
        return false;
    }
    glfwMakeContextCurrent(ENGINE->WINDOW);

    /*----------------GLEW INITIALIZATION---------------------*/
    printf("--- ABOUT TO INIT GLEW ---\n");

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        printf("ERROR::GLEW_INITIALIZATION_FAILED\n");
        glfwTerminate();
        return false;
    }
    printf("--- GLEW INIT SUCCEEDED ---\n");
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
    
    vec3 cam_pos = {0.0, 0.0, 3.0};
    vec3 cam_up = {0.0, 1.0, 0.0};
    CAMERA_INIT(&(ENGINE->CAMERA), cam_pos, cam_up, 10.0, 0.1);
    glfwSetInputMode(ENGINE->WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    SKYBOX(&(ENGINE->spaceSkybox));
    /*--------------------SETTING THE PROJECTION MATRIX---------------------------*/
    glm_perspective(glm_rad(45.0f), (float)ENGINE->WINDOW_WIDTH / (float)ENGINE->WINDOW_HEIGHT, 0.1f, 1000.0f, ENGINE->PROJECTION_MATRIX);
    return true;
}

void ENGINE_RUN(Engine* ENGINE){
    while(!glfwWindowShouldClose(ENGINE->WINDOW)){
        /*-----------------DELTA TIME CALCULATIONS---------------------*/
        double currentFrame = glfwGetTime();
        ENGINE->deltaTime = currentFrame - ENGINE->lastFrameTime;
        ENGINE->lastFrameTime = currentFrame;

        /*----------INPUT------------*/
        processInput(ENGINE);
        
        /*-------------------Clearing the buffer-------------------------*/
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /*----------UPDATE------------*/
        // Draw skybox first
        RENDER_SKYBOX(&(ENGINE->spaceSkybox), &(ENGINE->CAMERA), GET_PROJECTION_MATRIX(ENGINE));
        // update(ENGINE);
        RENDER_MODEL(&(ENGINE->Sphere), &(ENGINE->CAMERA), GET_PROJECTION_MATRIX(ENGINE));
        /*---------This is the end of the current frame--------------*/
        glfwSwapBuffers(ENGINE->WINDOW);
        glfwPollEvents();
    }
}

void ENGINE_CLEANUP(Engine* ENGINE){
    glfwTerminate();
    if(&(ENGINE->spaceSkybox)){
        printf("LOG::ENGINE.H::CLEANUP::SKYBOX_FREED\n");
    }
    if(&(ENGINE->Sphere))
        printf("LOG::ENGINE.H::CLEANUP::MODEL_FREED\n");
        FREE_MODEL(&(ENGINE->Sphere));
    glfwDestroyWindow(ENGINE->WINDOW);
}

/*--------------HELPER FUNCTION DEFINITIONS------------------*/
mat4* GET_PROJECTION_MATRIX(Engine* ENGINE){
    return &(ENGINE->PROJECTION_MATRIX);
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
    PROCESS_MOUSE_MOVEMENT(&(ENGINE->CAMERA), xoffset, yoffset, true);
}

static void framebuffer_size_callback(GLFWwindow* WINDOW, int WIDTH, int HEIGHT) {
    Engine* engine = (Engine*)glfwGetWindowUserPointer(WINDOW);
    engine->WINDOW_WIDTH = WIDTH;
    engine->WINDOW_HEIGHT = HEIGHT;
    glViewport(0, 0, WIDTH, HEIGHT);
    glm_perspective(glm_rad(90.0), (float)WIDTH / (float)HEIGHT, 0.1, 1000.0, engine->PROJECTION_MATRIX);
}

// In processInput() in engine.c
static void processInput(Engine* ENGINE){
    if(glfwGetKey(ENGINE->WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ENGINE->WINDOW, true);

    if (glfwGetKey(ENGINE->WINDOW, GLFW_KEY_W) == GLFW_PRESS){
        KEYBOARD_PROCESSING(&(ENGINE->CAMERA), CAMERA_MOVEMENT_FORWARD, ENGINE->deltaTime);
    }
    if (glfwGetKey(ENGINE->WINDOW, GLFW_KEY_S) == GLFW_PRESS)
        KEYBOARD_PROCESSING(&(ENGINE->CAMERA), CAMERA_MOVEMENT_BACKWARD, ENGINE->deltaTime);
    if (glfwGetKey(ENGINE->WINDOW, GLFW_KEY_A) == GLFW_PRESS)
        KEYBOARD_PROCESSING(&(ENGINE->CAMERA), CAMERA_MOVEMENT_LEFT, ENGINE->deltaTime);
    if (glfwGetKey(ENGINE->WINDOW, GLFW_KEY_D) == GLFW_PRESS)
        KEYBOARD_PROCESSING(&(ENGINE->CAMERA), CAMERA_MOVEMENT_RIGHT, ENGINE->deltaTime);
}