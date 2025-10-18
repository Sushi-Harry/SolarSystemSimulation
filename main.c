#include "include/Engine.h"

int main(){
    Engine newEngine;
    if(!ENGINE_INIT(&newEngine, 800, 600)){
        return -1;
    }

    ENGINE_RUN(&newEngine);
    return 0;
}