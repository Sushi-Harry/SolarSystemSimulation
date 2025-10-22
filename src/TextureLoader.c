#include "../include/TextureLoader.h"
#include <stdlib.h>
#include <string.h>

// Had to split TextureLoader into a header and a source file cause stb_image.h wouldn't work otherwise
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void TEXTURE(Texture* TEXTURE, char *directory, char* name){

    /*-----------BUILDING THE FULL PATH------------------------------*/
    
    //Calculate the length needed
    size_t dir_len = strlen(directory);
    size_t name_len = strlen(name);
    // Adding +1 for the null terminator '\0'
    size_t total_len = dir_len + name_len + 1; 

    // Allocate the exact memory needed
    TEXTURE->PATH = (char*)malloc(total_len);
    
    //check if malloc succeeded
    if (TEXTURE->PATH == NULL) {
        printf("ERROR::TEXTURE_LOADER::FAILED_TO_ALLOCATE_MEMORY_FOR_PATH\n");
        return; // Exit the function; we can't continue
    }
    strcpy(TEXTURE->PATH, directory);
    strcat(TEXTURE->PATH, name);

    /*----------------------SETTING UP THE TEXTURE-----------------------------*/
    glGenTextures(1, &(TEXTURE->TEXTURE_OBJECT));
    glBindTexture(GL_TEXTURE_2D, TEXTURE->TEXTURE_OBJECT);

    // Texture wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading and generating the texture data
    // Use the TEXTURE->PATH we just built
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(TEXTURE->PATH, &(TEXTURE->WIDTH), &(TEXTURE->HEIGHT), &(TEXTURE->NR_CHANNELS), 0);
    
    if(data){
        GLenum format = GL_RGB;
        if (TEXTURE->NR_CHANNELS == 1)
            format = GL_RED;
        else if (TEXTURE->NR_CHANNELS == 3)
            format = GL_RGB;
        else if (TEXTURE->NR_CHANNELS == 4)
            format = GL_RGBA;
            
        glTexImage2D(GL_TEXTURE_2D, 0, format, TEXTURE->WIDTH, TEXTURE->HEIGHT, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        printf("ERROR::TEXTURE_LOADER::FAILED_TO_LOAD_TEXTURE: %s\n", TEXTURE->PATH);
    }
    
    stbi_image_free(data);
}

void TEXTURE_PREDEFINED_PATH(Texture* TEXTURE, const char* TEXTURE_PATH){
    TEXTURE->PATH = TEXTURE_PATH;
    /*----------------------SETTING UP THE TEXTURE-----------------------------*/
    glGenTextures(1, &(TEXTURE->TEXTURE_OBJECT));
    glBindTexture(GL_TEXTURE_2D, TEXTURE->TEXTURE_OBJECT);

    // Texture wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading and generating the texture data
    // Use the TEXTURE->PATH we just built
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(TEXTURE->PATH, &(TEXTURE->WIDTH), &(TEXTURE->HEIGHT), &(TEXTURE->NR_CHANNELS), 0);
    
    if(data){
        GLenum format = GL_RGB;
        if (TEXTURE->NR_CHANNELS == 1)
            format = GL_RED;
        else if (TEXTURE->NR_CHANNELS == 3)
            format = GL_RGB;
        else if (TEXTURE->NR_CHANNELS == 4)
            format = GL_RGBA;
            
        glTexImage2D(GL_TEXTURE_2D, 0, format, TEXTURE->WIDTH, TEXTURE->HEIGHT, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        printf("ERROR::TEXTURE_LOADER::FAILED_TO_LOAD_TEXTURE: %s\n", TEXTURE->PATH);
    }
    
    stbi_image_free(data);
}


void TEXTURE_FREE(Texture* TEXTURE) {
    if (TEXTURE) {
        glDeleteTextures(1, &(TEXTURE->TEXTURE_OBJECT));
        free(TEXTURE->PATH); // Free the path string
        TEXTURE->PATH = NULL;
    }
}

void CUBEMAP_TEXTURE(CubemapTexture* texture){
    glGenTextures(1, &(texture->CUBEMAP_TEXTURE_ID));
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->CUBEMAP_TEXTURE_ID);

    char PATHS[6][50] = {
        "skybox/skybox_2/right.png",  
        "skybox/skybox_2/left.png",   
        "skybox/skybox_2/top.png",    
        "skybox/skybox_2/bottom.png", 
        "skybox/skybox_2/back.png",   
        "skybox/skybox_2/front.png"
    };
    
    for(int i = 0; i < 6; i++){
        int WIDTH, HEIGHT, NR_CHANNELS;
        unsigned char* data = stbi_load(PATHS[i], &WIDTH, &HEIGHT, &NR_CHANNELS, 0 );
        if(data){

            // THe following code was suggested by Gemini. This is a rather simple fix that I actually missed out on while writing this function for the very first time
            // Very amateur mistake but oh well..
            // This part of the code actually handles different image formats
            // 1 channel means that the image probably only has the red channel.
            // jpg uses 3 channels: r,g,b
            // But the skyboxes I'm using are made of 6 png files and png has 4 channels. It has an extra alpha channel that jpg lacks
            // Initially I was using a hardcoded GL_RGB format in the glTexImage2D format which led to a rather nice looking visual glitch. Very similar to an interference pattern formed using white light
            GLenum format;
            if (NR_CHANNELS == 1)
                format = GL_RED;
            else if (NR_CHANNELS == 3)
                format = GL_RGB;
            else if (NR_CHANNELS == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, WIDTH, HEIGHT, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }else{
            printf("ERROR::CUBEMAP_TEXTURE::FAILED_TO_LOAD_CUBE_MAP_TEX_AT_PATH\n");
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}