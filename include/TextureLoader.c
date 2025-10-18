#include "TextureLoader.h"
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

    // Build the path string safely
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

void TEXTURE_FREE(Texture* TEXTURE) {
    if (TEXTURE) {
        glDeleteTextures(1, &(TEXTURE->TEXTURE_OBJECT));
        free(TEXTURE->PATH); // Free the path string
        TEXTURE->PATH = NULL;
    }
}