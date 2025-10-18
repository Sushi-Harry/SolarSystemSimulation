#pragma once

#include <GL/glew.h>

typedef struct Texture{
    
    unsigned int TEXTURE_OBJECT;
    char *PATH;
    int WIDTH, HEIGHT, NR_CHANNELS;

} Texture;

void TEXTURE(Texture* TEXTURE, char *directory, char* name);
void TEXTURE_FREE(Texture* TEXTURE);