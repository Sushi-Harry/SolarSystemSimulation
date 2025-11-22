#pragma once

#include "GL/glew.h"
#include "Camera.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "cgltf/cgltf.h"

typedef struct Material{
    Texture diffuse;
    Texture specular;
} Material;

typedef struct Model{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    cgltf_component_type INDEX_TYPE;
    unsigned int INDEX_COUNT;
    Shader SHADER;
    Material MATERIAL;
} Model;

void MODEL(Model* MODEL, const char* MODEL_PATH, const char* VS_NAME, const char* FS_NAME, const char* DIFFUSE_PATH, const char* SPECULAR_PATH);
void RENDER_MODEL(Model* MODEL, Camera* CAMERA, mat4* PROJECTION_MATRIX);
void FREE_MODEL(Model* MODEL);