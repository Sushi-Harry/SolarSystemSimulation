#include "ModelLoader.h"
#include "Camera.h"
#include <cglm/cglm.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

typedef struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} Vertex;

void MODEL(Model* MODEL, const char* MODEL_PATH, const char* VS_NAME, const char* FS_NAME, const char* DIFFUSE_PATH, const char* SPECULAR_PATH) {
    //Load Shaders and Textures
    SHADER(&(MODEL->SHADER), VS_NAME, FS_NAME);
    TEXTURE(&(MODEL->MATERIAL.diffuse), "textures/", DIFFUSE_PATH);
    TEXTURE(&(MODEL->MATERIAL.specular), "textures/", SPECULAR_PATH);

    //Parse the glTF file
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, MODEL_PATH, &data);
    if (result != cgltf_result_success) {
        printf("ERROR::MODELLOADER::COULD_NOT_PARSE_FILE: %s\n", MODEL_PATH);
        return;
    }

    //Load the binary data buffers
    result = cgltf_load_buffers(&options, data, MODEL_PATH);
    if (result != cgltf_result_success) {
        printf("ERROR::MODELLOADER::COULD_NOT_LOAD_BUFFERS_FOR_:%s\n", MODEL_PATH);
        cgltf_free(data);
        return;
    }

    // ensuring that the file contains at least one mesh.
    if (data->meshes_count == 0) {
        printf("ERROR::MODELLOADER::NO_MESHES_FOUND_IN_FILE\n");
        cgltf_free(data);
        return;
    }

    //load the first primitive of the first mesh.
    cgltf_mesh* mesh = &data->meshes[0];
    cgltf_primitive* primitive = &mesh->primitives[0];

    //  pointers to the attribute data from the glTF buffers.
    float* positions = NULL;
    float* normals = NULL;
    float* tex_coords = NULL;
    cgltf_size vertex_count = 0;

    for (int i = 0; i < primitive->attributes_count; ++i) {
        cgltf_attribute* attr = &primitive->attributes[i];
        cgltf_accessor* acc = attr->data;

        // Get a pointer to the start of the data for this attribute.
        void* data_ptr = (char*)acc->buffer_view->buffer->data + acc->buffer_view->offset + acc->offset;

        if (attr->type == cgltf_attribute_type_position) {
            positions = data_ptr;
            vertex_count = acc->count;
        } else if (attr->type == cgltf_attribute_type_normal) {
            normals = data_ptr;
        } else if (attr->type == cgltf_attribute_type_texcoord) {
            tex_coords = data_ptr;
        }
    }

    // Validate that we found the essential position data.
    if (positions == NULL) {
        printf("ERROR::MODELLOADER::POSITION_DATA_NOT_FOUND_IN_PRIMITIVI\n");
        cgltf_free(data);
        return;
    }

    // 5. Extract Index Data
    cgltf_accessor* indices_accessor = primitive->indices;
    void* indices = (char*)indices_accessor->buffer_view->buffer->data + indices_accessor->buffer_view->offset + indices_accessor->offset;
    MODEL->INDEX_COUNT = indices_accessor->count;
    MODEL->INDEX_TYPE = indices_accessor->component_type;

    // 6. Interleave the vertex data
    // This is the most crucial step. We create one single buffer that mixes
    // all the vertex attributes together (e.g., [pos1, norm1, uv1, pos2, norm2, uv2, ...]).
    Vertex* vertices = malloc(vertex_count * sizeof(Vertex));
    if (vertices == NULL) {
        printf("ERROR::MODELLOADER::COULD_NOT_ALLOCATE_MEMORY_FOR_VERTICES\n");
        cgltf_free(data);
        return;
    }

    for (cgltf_size i = 0; i < vertex_count; i++) {
        // Copy position data
        memcpy(vertices[i].position, positions + (i * 3), sizeof(vec3));

        // Copy normal data (if it exists)
        if (normals) {
            memcpy(vertices[i].normal, normals + (i * 3), sizeof(vec3));
        }

        // Copy texture coordinate data (if it exists)
        if (tex_coords) {
            memcpy(vertices[i].texCoord, tex_coords + (i * 2), sizeof(vec2));
        }
    }

    // 7. Upload the interleaved data to the GPU
    glGenVertexArrays(1, &MODEL->VAO);
    glGenBuffers(1, &MODEL->VBO);
    glGenBuffers(1, &MODEL->EBO);

    glBindVertexArray(MODEL->VAO);

        // Upload the single, interleaved VBO
        glBindBuffer(GL_ARRAY_BUFFER, MODEL->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        // Upload the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MODEL->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MODEL->INDEX_COUNT * cgltf_component_size(MODEL->INDEX_TYPE), indices, GL_STATIC_DRAW);

        //Set the Vertex Attribute Pointers
        // This tells OpenGL how to read the interleaved data in the VBO.
        // Position Attribute (location = 0) (These locations correspond to the layout (location = ) thing in the start of all the vertex shaders)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal Attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture Coordinate Attribute (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //Final Cleanup
    // The data is now on the GPU, so we can free the temporary CPU-side buffers.
    free(vertices);
    cgltf_free(data);
}

void RENDER_MODEL(Model* MODEL, Camera* CAMERA, mat4* PROJECTION_MATRIX){
    USE_SHADER(&(MODEL->SHADER));
    // Setting up the texture units
    //Bind diffuse to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, MODEL->MATERIAL.diffuse.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_diffuse1", 0);
    //Bind specular to texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, MODEL->MATERIAL.specular.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_specular1", 1);

    // USING A 4x4 IDENTITY MATRIX FOR TESTING PURPOSES. WILL ADD DIFFERENT LOGIC FOR MODEL MATRIX WHEN WORKING ON PLANETARY MOTION
    mat4 model;
    glm_mat4_identity(model);
    glm_scale(model, (vec3){0.1f, 0.1f, 0.1f});
    SET_MAT4(&(MODEL->SHADER), "model", &model);

    mat4 view;
    VIEW_MATRIX(CAMERA, &view);
    SET_MAT4(&(MODEL->SHADER), "view", &view);
    
    //Projection Matrix
    SET_MAT4(&(MODEL->SHADER), "projection", PROJECTION_MATRIX);

    vec3* viewPos = VIEW_POS(CAMERA);
    SET_VEC3(&(MODEL->SHADER), "viewPos", viewPos);

    // Draw Calls After thsi comment
    glBindVertexArray(MODEL->VAO);
    GLenum gl_index_type = (MODEL->INDEX_TYPE == cgltf_component_type_r_16u) 
                           ? GL_UNSIGNED_SHORT 
                           : GL_UNSIGNED_INT;
    /*---------------------DEBUG DEBUG DEBUG DEBUG DEBUG....--------------------------*/
    // printf("DEBUG: cgltf index type: %d, GL index type: %s\n", 
    //        MODEL->INDEX_TYPE, 
    //        (gl_index_type == GL_UNSIGNED_SHORT) ? "GL_UNSIGNED_SHORT" : "GL_UNSIGNED_INT");
    glDrawElements(GL_TRIANGLES, MODEL->INDEX_COUNT, gl_index_type, 0);
    glBindVertexArray(0);
    // printf("LOG::MODELLOADER.H::MODEL_DRAWN_SUCCESSFULLY\n");
}

void FREE_MODEL(Model* MODEL){
    if(MODEL){
        FREE_SHADER(&(MODEL->SHADER));
        glDeleteVertexArrays(1, &MODEL->VAO);
        glDeleteBuffers(1, &MODEL->VBO);
        glDeleteBuffers(1, &MODEL->EBO);
    }
}