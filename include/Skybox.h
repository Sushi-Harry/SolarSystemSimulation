#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include "Camera.h"
#include "TextureLoader.h"

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