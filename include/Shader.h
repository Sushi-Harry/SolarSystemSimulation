#pragma once
#include "GL/glew.h"

#include <stdio.h>
#include <stdbool.h>
#include "cglm/cglm.h"
#include <string.h>
#include <stdlib.h>

typedef struct Shader{
    char* NAME_VS;
    char* NAME_FS;
    unsigned int SHADER_ID;

} Shader;

void SHADER(Shader* SHADER, const char* vsName, const char* fsName);
void USE_SHADER(Shader* SHADER);
void FREE_SHADER(Shader* SHADER);

/*------------------Some helper functions---------------*/
char* READ_FILE_TO_STRING(const char* path);
void SET_MAT4(Shader* SHADER, const char* UNIFORM_NAME, mat4* MATRIX_4x4);
void SET_MAT3(Shader* SHADER, const char* UNIFORM_NAME, mat3* MATRIX_3x3);
void SET_MAT2(Shader* SHADER, const char* UNIFORM_NAME, mat2* MATRIX_2x2);
void SET_VEC4(Shader* SHADER, const char* UNIFORM_NAME, vec4* VECTOR_4);
void SET_VEC3(Shader* SHADER, const char* UNIFORM_NAME, vec3* VECTOR_3);
void SET_VEC2(Shader* SHADER, const char* UNIFORM_NAME, vec2* VECTOR_2);
void SET_INT(Shader* SHADER, const char* UNIFORM_NAME, int INTEGER_VALUE);
void SET_FLOAT(Shader* SHADER, const char* UNIFORM_NAME, float FLOAT_VALUE);
void SET_BOOL(Shader* SHADER, const char* UNIFORM_NAME, bool BOOLEAN_VALUE);