#pragma once

#include "GL/glew.h"

typedef struct Texture{
    
    unsigned int TEXTURE_OBJECT;
    char *PATH;
    int WIDTH, HEIGHT, NR_CHANNELS;

} Texture;

typedef struct CubemapTexture{
    unsigned int CUBEMAP_TEXTURE_ID;
    char *DIRECTORY;
} CubemapTexture;

/*----------------------------------TEXTURE------------------------------*/
void TEXTURE(Texture* TEXTURE, char *directory, char* name);
void TEXTURE_PREDEFINED_PATH(Texture* TEXTURE, const char* TEXTURE_PATH);
void TEXTURE_FREE(Texture* TEXTURE);

/*------------------------------CUBEMAP TEXTURE--------------------------*/
void CUBEMAP_TEXTURE(CubemapTexture* texture);