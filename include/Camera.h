#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include <cglm/cglm.h>

enum CAMERA_MOVEMENT {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

typedef struct Camera{
    /* Camera Attributes */
    vec3 POSITION;
    vec3 FRONT;
    vec3 UP;
    vec3 RIGHT;
    vec3 WORLD_UP;
    
    /* Euler Angles */
    float YAW;
    float PITCH;
    
    /* Other Basic Attributes / options */
    float MOVE_SPEED;
    float SENSITIVITY;
    
} Camera;

void KEYBOARD_PROCESSING(Camera* cam, CAMERA_MOVEMENT direction, float deltaTime);
void UPDATE_VECTORS(Camera* cam);
void CAMERA_INIT(Camera* cam, vec3 position, vec3 up, float move_speed, float sensitivity);
void VIEW_MATRIX(Camera* cam, mat4* destViewMat);
vec3* VIEW_POS(Camera* cam);
void PROCESS_MOUSE_MOVEMENT(Camera* cam, float xoffset, float yoffset, bool constrainPitch);
