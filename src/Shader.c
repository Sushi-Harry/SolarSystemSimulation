#include "GL/glew.h"
#include "../include/Shader.h"

/*------------------FUNCTION DEFINITIONS------------------*/
void SHADER(Shader* SHADER, const char* vsName, const char* fsName){
    printf("DEBUG (Shader.c):      sizeof(Shader) = %zu\n", sizeof(Shader));
    //These varibles will hold the path to the VS and FS
    const char* DEFAULT_DIR = "shaders/";
    char PATH_VS[512];
    char PATH_FS[512];
    snprintf(PATH_VS, sizeof(PATH_VS), "%s%s", DEFAULT_DIR, vsName);
    snprintf(PATH_FS, sizeof(PATH_FS), "%s%s", DEFAULT_DIR, fsName);
    // Reading the files
    char *CODE_VS = READ_FILE_TO_STRING(PATH_VS);
    char *CODE_FS = READ_FILE_TO_STRING(PATH_FS);
    if(CODE_FS == NULL || CODE_VS == NULL){
        free(CODE_FS);
        free(CODE_VS);
        return;
    }
    // Assigning the names to the member variables
    SHADER->NAME_VS = (char*)malloc(strlen(vsName) + 1);
    strcpy(SHADER->NAME_VS, vsName);
    SHADER->NAME_FS = (char*)malloc(strlen(fsName) + 1);
    strcpy(SHADER->NAME_FS, fsName);

    //==========================================Compiling the shaders===========================================//
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    const char* vertexSource = CODE_VS;
    const char* fragmentSource = CODE_FS;

    //Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER.H::VERTEX::COMPILATION_FAILED::%s::%s\n", vsName, infoLog);
        glDeleteShader(vertex);
        free(CODE_FS);
        free(CODE_VS);
    }

    //Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER.H::FRAGMENT::COMPILATION_FAILED::%s::%s\n", fsName, infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        free(CODE_FS);
        free(CODE_VS);
    }
    SHADER->SHADER_ID = glCreateProgram();
    glAttachShader(SHADER->SHADER_ID, vertex);
    glAttachShader(SHADER->SHADER_ID, fragment);
    glLinkProgram(SHADER->SHADER_ID);
    // Printing the linking errors
    glGetProgramiv(SHADER->SHADER_ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(SHADER->SHADER_ID, 512, NULL, infoLog);
        printf("ERROR::SHADER.H::PROGRAM::LINKING_FAILED::%s", infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        free(CODE_FS);
        free(CODE_VS);
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(CODE_FS);
    free(CODE_VS);
}
void USE_SHADER(Shader* SHADER){
    glUseProgram(SHADER->SHADER_ID);
    
}
void FREE_SHADER(Shader* shader) {
    if (shader) {
        glDeleteProgram(shader->SHADER_ID);
        free(shader->NAME_VS);
        free(shader->NAME_FS);
        shader->NAME_VS = NULL;
        shader->NAME_FS = NULL;
    }
}

/*--------------------Helper Function Definitions----------------------------------*/ //(MOSTLY JUST SETTING UNIFORMS)
char* READ_FILE_TO_STRING(const char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("ERROR::SHADER::COULD_NOT_READ_FILE: %s\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long lsize = ftell(file);
    rewind(file);

    char* buffer = (char*)calloc(1, lsize + 1);
    if (!buffer) {
        printf("ERROR::SHADER::BUFFER_ALLOCATION_FAILED\n");
        fclose(file);
        return NULL;
    }

    if (fread(buffer, lsize, 1, file) != 1) {
        printf("ERROR::SHADER::READ_FAILED\n");
        fclose(file);
        free(buffer);
        return NULL;
    }

    fclose(file);
    return buffer;
}
void SET_MAT4(Shader* SHADER, const char* UNIFORM_NAME, mat4 *MATRIX_4x4){
    glUniformMatrix4fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, GL_FALSE, (float*)MATRIX_4x4);
}
void SET_MAT3(Shader* SHADER, const char* UNIFORM_NAME, mat3* MATRIX_3x3){
    glUniformMatrix3fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, GL_FALSE, (float*)MATRIX_3x3);
}
void SET_MAT2(Shader* SHADER, const char* UNIFORM_NAME, mat2* MATRIX_2x2){
    glUniformMatrix2fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, GL_FALSE, (float*)MATRIX_2x2);
}
void SET_VEC4(Shader* SHADER, const char* UNIFORM_NAME, vec4* VECTOR_4){
    glUniform4fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, (float*)VECTOR_4);
}
void SET_VEC3(Shader* SHADER, const char* UNIFORM_NAME, vec3* VECTOR_3){
    glUniform3fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, (float*)VECTOR_3);
}
void SET_VEC2(Shader* SHADER, const char* UNIFORM_NAME, vec2* VECTOR_2){
    glUniform2fv(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), 1, (float*)VECTOR_2);
}
void SET_INT(Shader* SHADER, const char* UNIFORM_NAME, int INTEGER_VALUE){
    glUniform1i(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), INTEGER_VALUE);
}
void SET_FLOAT(Shader* SHADER, const char* UNIFORM_NAME, float FLOAT_VALUE){
    glUniform1f(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), FLOAT_VALUE);
}
void SET_BOOL(Shader* SHADER, const char* UNIFORM_NAME, bool BOOLEAN_VALUE){
    glUniform1i(glGetUniformLocation(SHADER->SHADER_ID, UNIFORM_NAME), (int)BOOLEAN_VALUE);
}