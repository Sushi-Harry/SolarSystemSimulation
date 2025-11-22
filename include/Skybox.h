#pragma once

#include "GL/glew.h"
#include "Shader.h"
#include "Camera.h"
#include "TextureLoader.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Skybox
{
    unsigned int VAO;
    unsigned int VBO;
    CubemapTexture CUBEMAP;
    Shader SHADER;
} Skybox;

void SKYBOX(Skybox* SKYBOX);
void RENDER_SKYBOX(Skybox* SKYBOX, Camera* CAMERA, mat4* PROJECTION_MATRIX);
void FREE_SKYBOX(Skybox* SKYBOX);

#ifdef __cplusplus
}
#endif