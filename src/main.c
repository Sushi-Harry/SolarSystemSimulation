#include "../include/Engine.h"

int main(){
    Engine newEngine;
    if(!ENGINE_INIT(&newEngine, 1600, 900)){
        return -1;
    }

    ENGINE_RUN(&newEngine);

    printf("\n-----------------------CLEANUP-------------------------\n");
    ENGINE_CLEANUP(&newEngine);

    return 0;
}