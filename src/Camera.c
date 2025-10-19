#include "Camera.h"

// Default camera values
const float DEFAULT_YAW         = -90.0;
const float DEFAULT_PITCH       =  0.0;
const float DEFAULT_SPEED       =  2.5;
const float DEFAULT_SENSITIVITY =  0.1;


void UPDATE_VECTORS(Camera* cam){
    //------------------Updating the camera vectors---------------------//
    vec3 front;
    front[0] = cos(glm_rad(cam->YAW)) * cos(glm_rad(cam->PITCH));
    front[1] = sin(glm_rad(cam->PITCH));
    front[2] = sin(glm_rad(cam->YAW)) * cos(glm_rad(cam->PITCH));
    glm_normalize(front);
    //Assigning FRONT
    glm_vec3_copy(front, cam->FRONT);
    
    // Calculating and assigning the right vector in the camera object
    vec3 right;
    glm_cross(cam->FRONT, cam->WORLD_UP, right);
    glm_normalize(right);
    glm_vec3_copy(right, cam->RIGHT);

    //calculating and assigning up vector
    vec3 up;
    glm_cross(cam->RIGHT, cam->FRONT, up);
    glm_normalize(up);
    glm_vec3_copy(up, cam->UP);
}
void CAMERA_INIT(Camera* cam, vec3 position, vec3 up, float move_speed, float sensitivity){
    vec3 DEFAULT_UP_VECTOR = {0.0, 1.0, 0.0};
    
    glm_vec3_copy(position, cam->POSITION);
    glm_vec3_copy(DEFAULT_UP_VECTOR, cam->WORLD_UP);
    
    cam->YAW = DEFAULT_YAW;
    cam->PITCH = DEFAULT_PITCH;
    cam->MOVE_SPEED = move_speed;
    cam->SENSITIVITY = sensitivity;

    UPDATE_VECTORS(cam);
}
void VIEW_MATRIX(Camera* cam, mat4* destViewMat){
    vec3 center;
    glm_vec3_add(cam->POSITION, cam->FRONT, center);
    glm_lookat(cam->POSITION, center, cam->UP, *destViewMat);
}
vec3* VIEW_POS(Camera* cam){
    return &(cam->POSITION);
}
void KEYBOARD_PROCESSING(Camera* cam, CAMERA_MOVEMENT direction, float deltaTime){
    float velocity = cam->MOVE_SPEED * deltaTime;
    printf("LOG::CAMERA.H::KEYBOARD_PROCESSING()::VELOCITY_%f::DELTA_TIME_%f\n", velocity, deltaTime);
    if(direction == CAMERA_MOVEMENT_FORWARD){
        glm_vec3_muladds(cam->FRONT, velocity, cam->POSITION);
    }
    if(direction == CAMERA_MOVEMENT_BACKWARD){
        glm_vec3_muladds(cam->FRONT, -velocity, cam->POSITION);
    }
    if(direction == CAMERA_MOVEMENT_LEFT){
        glm_vec3_muladds(cam->RIGHT, -velocity, cam->POSITION);
    }
    if(direction == CAMERA_MOVEMENT_RIGHT){
        glm_vec3_muladds(cam->RIGHT, velocity, cam->POSITION);
    }
}
// The corrected function signature
void PROCESS_MOUSE_MOVEMENT(Camera* cam, float xoffset, float yoffset, bool constrainPitch){
    xoffset *= cam->SENSITIVITY;
    yoffset *= cam->SENSITIVITY;

    cam->YAW += xoffset;
    cam->PITCH += yoffset;

    // Use the constrainPitch parameter
    if(constrainPitch)
    {
        if(cam->PITCH > 89.0f)
            cam->PITCH = 89.0f;
        if(cam->PITCH < -89.0f)
            cam->PITCH = -89.0f;
    }
    
    // now update the camera vectors
    UPDATE_VECTORS(cam);
}