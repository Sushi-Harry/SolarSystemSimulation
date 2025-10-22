#include "../include/Engine.h"
#include <stdlib.h> // Required for malloc and free

int main() {
    // Allocate the Engine on the heap to prevent a stack overflow.
    Engine* newEngine = (Engine*)malloc(sizeof(Engine));
    if (newEngine == NULL) {
        printf("CRITICAL ERROR: Failed to allocate memory for the engine.\n");
        return -1;
    }

    // Initialize the engine. We pass the pointer directly.
    if (!ENGINE_INIT(newEngine, 1600, 900)) {
        free(newEngine); // Clean up memory on failure
        return -1;
    }

    // Run the main loop.
    ENGINE_RUN(newEngine);

    // Clean up all engine resources.
    ENGINE_CLEANUP(newEngine);

    // Free the engine struct itself before the program exits.
    free(newEngine);

    return 0;
}
