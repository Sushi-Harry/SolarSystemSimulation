#include "ModelLoader.h"
#include "Camera.h"
#include "Engine.h"
#include <cglm/cglm.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

typedef struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} Vertex;

void MODEL(Model* MODEL, const char* MODEL_PATH, const char* VS_NAME, const char* FS_NAME, const char* DIFFUSE_PATH, const char* SPECULAR_PATH){
    
    // Creating the shader for the model
    SHADER(&(MODEL->SHADER), VS_NAME, FS_NAME);
    
    cgltf_options options = {0};
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, MODEL_PATH, &data);
    if(result != cgltf_result_success){
        printf("ERROR::MODELLOADER.H::COULD_NOT_PARSE_GLTF\n");
        cgltf_free(data);
        return;
    }
    result = cgltf_load_buffers(&options, data, MODEL_PATH);
    if(result != cgltf_result_success){
        printf("ERROR::MODELLOADER::COULD_NOT_LOAD_BINARY_BUFFERS\n");
        cgltf_free(data);
        return;
    }

    if (data->meshes_count == 0) {
        printf("ERROR::MODELLOADER::File contains no meshes.\n");
        cgltf_free(data);
        return;
    }
    cgltf_mesh* mesh = &data->meshes[0];
    cgltf_primitive* primitive = &mesh->primitives[0];

    /*--------------------------------Vertex Data---------------------------*/
    float* positions = NULL;
    cgltf_size vertexCount = 0;
    for(int i = 0; i < primitive->attributes_count; i++){
        if(primitive->attributes[i].type == cgltf_attribute_type_position){
            cgltf_accessor* accessor = primitive->attributes[i].data;
            positions = (float*)((char*)accessor->buffer_view->buffer->data + accessor->buffer_view->offset + accessor->offset);
            vertexCount = accessor->count;
            break;
        }
    }

    /*-------------------INDEX DATA----------------------*/
    cgltf_accessor* indices_accessor = primitive->indices;
    void* indices = (char*)indices_accessor->buffer_view->buffer->data + indices_accessor->buffer_view->offset + indices_accessor->offset;
    MODEL->INDEX_COUNT = indices_accessor->count;
    cgltf_component_type index_type = indices_accessor->component_type;
    MODEL->INDEX_TYPE = index_type;
    /*-------------------------TEXTURE COORDS--------------------*/
    float* texture_coords = NULL;
    cgltf_size texture_coord_count = 0;
    for(int i = 0; i < primitive->attributes_count; i++){
        if(primitive->attributes[i].type == cgltf_attribute_type_texcoord){
            cgltf_accessor* accessor = primitive->attributes[i].data;
            texture_coords = (float*)((char*)accessor->buffer_view->buffer->data + accessor->buffer_view->offset + accessor->offset);
            texture_coord_count = accessor->count;
            break;
        }
    }
    /*---------------------------NORMALS----------------------------*/
    float* normals = NULL;
    cgltf_size normal_count = 0;
    for(int i = 0; i < primitive->attributes_count; i++){
        if(primitive->attributes[i].type == cgltf_attribute_type_normal){
            cgltf_accessor* accessor = primitive->attributes[i].data;
            normals = (float*)((char*)accessor->buffer_view->buffer->data + accessor->buffer_view->offset + accessor->offset);
            normal_count = accessor->count;
            break;
        }
    }
    /*--------------------------------------------------UPLOAD DATA TO THE GPU---------------------------------------*/
    // This makes it easier to pass the data to the gpu. I could've done it without the vertices struct but it's just a better way of doing writing this part
    Vertex* vertices = malloc(vertexCount * sizeof(Vertex));
    if(vertices == NULL){
        printf("ERROR::MODELLOADER.H::COULD_NOT_ALLOCATE_MEMORY_TO_VERTICES\n");
        cgltf_free(data);
        return;
    }
    for(cgltf_size i = 0; i < vertexCount; i++){
        float *pos_ptr = positions + (i*3);
        float *norm_ptr = normals + (i*3);
        float *tex_ptr = texture_coords + (i*2);

        glm_vec3_copy(pos_ptr, vertices[i].position);
        glm_vec3_copy(norm_ptr, vertices[i].normal);
        glm_vec2_copy(tex_ptr, vertices[i].texCoord);
    }

    //generating buffers
    glGenVertexArrays(1, &MODEL->VAO);
    glGenBuffers(1, &MODEL->VBO);
    glGenBuffers(1, &MODEL->EBO);
    
    glBindVertexArray(MODEL->VAO);

        // ARRAY BUFFER OBJECT
        glBindBuffer(GL_ARRAY_BUFFER, MODEL->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(Vertex), positions, GL_STATIC_DRAW);

        // ELEMENT BUFFER OBJECT
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MODEL->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MODEL->INDEX_COUNT * cgltf_component_size(index_type), indices, GL_STATIC_DRAW);

        // POSITION ATTRIBUTE
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // NORMAL ATTRIBUTE
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture Coordinates Attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);

    // Loading the textures for the model
    TEXTURE(&(MODEL->MATERIAL.diffuse), "textures/", DIFFUSE_PATH);
    TEXTURE(&(MODEL->MATERIAL.specular), "textures/", SPECULAR_PATH);
    // Free the data at last
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
    GLenum gl_index_type = (MODEL->INDEX_TYPE == cgltf_component_type_r_16u) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    glDrawElements(GL_TRIANGLES, MODEL->INDEX_COUNT, gl_index_type, 0);
    glBindVertexArray(0);
}

void FREE_MODEL(Model* MODEL){
    if(MODEL){
        FREE_SHADER(&(MODEL->SHADER));
        glDeleteVertexArrays(1, &MODEL->VAO);
        glDeleteBuffers(1, &MODEL->VBO);
        glDeleteBuffers(1, &MODEL->EBO);
    }
}