#include "Skybox.h"
#include "stb_image.h"

void SKYBOX(Skybox* SKYBOX){
    // Initialize the shader
    printf("-----------------Entering SKYBOX INIT-------------\n");
    SHADER(&(SKYBOX->SHADER), "skybox.vs", "skybox.fs");
    CUBEMAP_TEXTURE(&(SKYBOX->CUBEMAP));

    float vertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    // Generating data for the gpu and uploading it
    glGenVertexArrays(1, &(SKYBOX->VAO));
    glGenBuffers(1, &(SKYBOX->VBO));

    glBindVertexArray(SKYBOX->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, SKYBOX->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void RENDER_SKYBOX(Skybox* SKYBOX, Camera* CAMERA, mat4* PROJECTION_MATRIX){
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    USE_SHADER(&(SKYBOX->SHADER));
    
    mat4 view;
    VIEW_MATRIX(CAMERA, &view);
    // REMOVING THE TRANSLATION PART OF THE VIEW MATRIX FOR RENDERING SKYBOX
    glm_vec4_copy((vec4){0.0, 0.0, 0.0, 1.0}, view[3]);
    
    SET_MAT4(&(SKYBOX->SHADER), "view", &view);
    SET_MAT4(&(SKYBOX->SHADER), "projection", PROJECTION_MATRIX);
    glBindVertexArray(SKYBOX->VAO);
    //Setting the sampler2d uniform
    glActiveTexture(GL_TEXTURE0);
    SET_INT(&(SKYBOX->SHADER), "skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SKYBOX->CUBEMAP.CUBEMAP_TEXTURE_ID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, SKYBOX->CUBEMAP.CUBEMAP_TEXTURE_ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void FREE_SKYBOX(Skybox* SKYBOX){
    if(SKYBOX){
        FREE_SHADER(&(SKYBOX->SHADER));
        glDeleteVertexArrays(1, &(SKYBOX->VAO));
        glDeleteBuffers(1, &(SKYBOX->VBO));
        glDeleteTextures(1, &(SKYBOX->CUBEMAP.CUBEMAP_TEXTURE_ID));
    }
}