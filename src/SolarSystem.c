#include "../include/SolarSystem.h"
#include "math.h"
#include "GLFW/glfw3.h"

void STAR_INIT(Star* star, vec3 star_position, float star_radius, float star_mass, const char* diffuse_texture, const char* specular_texture, const char* star_name, const char* sphere_model_path){
    // Copy the position attrib
    glm_vec3_copy(star_position, star->POSITION);
    // Assign other variables
    star->RADIUS = star_radius;
    star->MASS = star_mass;
    star->DIFFUSE_PATH = diffuse_texture;
    star->SPECULAR_PATH = specular_texture;
    star->NAME = star_name;
    glm_vec3_copy((vec3){star_radius, star_radius, star_radius}, star->SCALE);
    glm_vec3_copy((vec3){0.0, 0.0, 0.0}, star->ROTATION);
    star->TYPE = BODY_TYPE_STAR;
    // Initialize the sphere model
    MODEL(&(star->MODEL), sphere_model_path, "sun.vs", "sun.fs", star->DIFFUSE_PATH, star->SPECULAR_PATH);
    //initialize the textures
    TEXTURE_PREDEFINED_PATH(&(star->DIFFUSE), star->DIFFUSE_PATH);
    TEXTURE_PREDEFINED_PATH(&(star->SPECULAR), star->SPECULAR_PATH);
    // Initialize the light source - This is a point light source
    star->POINT_LIGHT.enabled = true;
    glm_vec3_copy(star->POINT_LIGHT.color, (vec3){1.0, 1.0, 1.0});
    glm_vec3_copy(star->POINT_LIGHT.position, star->POSITION);
    star->POINT_LIGHT.ambient = 0.05;
    star->POINT_LIGHT.diffuse = 1.0;
    star->POINT_LIGHT.specular = 1.0;
    star->POINT_LIGHT.constant = 1.0;
    star->POINT_LIGHT.linear = 0.0;
    star->POINT_LIGHT.quadratic = 0.00000003;
}

void PLANET_INIT(Planet* planet, Star* parent_star, float planet_radius, float planet_mass, float orbital_radius, const char* diffuse_texture, const char* specular_texture, const char* planet_name, const char* sphere_model_path){
    // Specify the parent star
    planet->PARENT_STAR = parent_star;
    // initialize the planet's position to the parent star's position
    glm_vec3_copy(planet->PARENT_STAR->POSITION, planet->POSITION);
    // Initialize other stuff
    planet->RADIUS = planet_radius;
    planet->MASS = planet_mass;
    planet->DIFFUSE_PATH = diffuse_texture;
    planet->SPECULAR_PATH = specular_texture;
    planet->NAME = planet_name;
    glm_vec3_copy((vec3){planet_radius, planet_radius, planet_radius}, planet->SCALE);
    glm_vec3_copy((vec3){0.0, 0.0, 0.0}, planet->ROTATION);
    planet->TYPE = BODY_TYPE_PLANET;
    // Initialize the model
    MODEL(&(planet->MODEL), sphere_model_path, "planet.vs", "planet.fs", planet->DIFFUSE_PATH, planet->SPECULAR_PATH);

    // Calculate the orbital data
    planet->ORBITAL_RADIUS = orbital_radius;
    float grav_const = 0.6; // I know this is the wrong value according to real world physics but just let it be
    planet->ORBITAL_SPEED = sqrtf((grav_const * planet->MASS)/planet->ORBITAL_RADIUS);
    //Calculate the rotation data
    /* I'll be using a default rotation axis defined by the vector <-0.3, 1.0, 0.0> for the rotations
        and for simplicity I'll also be using a fixed value of rotation speed. Cause who's gonna notice? :weary_emoji_face:*/
    vec3 axis = {-0.3, 1.0, 0.0};
    glm_normalize(axis);
    glm_vec3_copy(axis, planet->ROTATION_AXIS);
    planet->ROTATION_SPEED = 0.2;
    planet->ROTATION_ANGLE = 0.0;
    //initialize the textures
    TEXTURE_PREDEFINED_PATH(&(planet->DIFFUSE), planet->DIFFUSE_PATH);
    TEXTURE_PREDEFINED_PATH(&(planet->SPECULAR), planet->SPECULAR_PATH);
}

void CALCULATE_PLANET_ORBITAL_DATA(Planet* planet){
    glm_vec3_copy((vec3){planet->RADIUS, planet->RADIUS, planet->RADIUS}, planet->SCALE);
    float grav_const = 0.6; // I know this is the wrong value according to real world physics but just let it be
    planet->ORBITAL_SPEED = sqrtf((grav_const * planet->MASS)/planet->ORBITAL_RADIUS);
}

void MOON_INIT(Moon* moon, Planet* parent_planet, float moon_radius, float moon_mass, float orbital_radius, const char* diffuse_texture, const char* specular_texture, const char* moon_name, const char* sphere_model_path){
     // Specify the parent star
    moon->PARENT_PLANET = parent_planet;
    // initialize the planet's position to the parent star's position
    glm_vec3_copy(moon->PARENT_PLANET->POSITION, moon->POSITION);
    // Initialize other stuff
    moon->RADIUS = moon_radius;
    moon->MASS = moon_mass;
    moon->DIFFUSE_PATH = diffuse_texture;
    moon->SPECULAR_PATH = specular_texture;
    moon->NAME = moon_name;
    glm_vec3_copy((vec3){moon_radius, moon_radius, moon_radius}, moon->SCALE);
    glm_vec3_copy((vec3){0.0, 0.0, 0.0}, moon->ROTATION);
    moon->TYPE = BODY_TYPE_MOON;
    // Initialize the model
    MODEL(&(moon->MODEL), sphere_model_path, "moon.vs", "moon.fs", moon->DIFFUSE_PATH, moon->SPECULAR_PATH);

    // Calculate the orbital data
    moon->ORBITAL_RADIUS = orbital_radius;
    float grav_const = 0.6; // I know this is the wrong value according to real world physics but just let it be
    moon->ORBITAL_SPEED = sqrtf((grav_const * moon->PARENT_PLANET->MASS)/moon->ORBITAL_RADIUS);
    //Calculate the rotation data
    moon->ROTATION_SPEED = 0.05;
    moon->ROTATION_ANGLE = 0.0;
    //initialize the textures
    TEXTURE_PREDEFINED_PATH(&(moon->DIFFUSE), moon->DIFFUSE_PATH);
    TEXTURE_PREDEFINED_PATH(&(moon->SPECULAR), moon->SPECULAR_PATH);
}

void CALCULATE_MOON_ORBITAL_DATA(Moon* moon){
    glm_vec3_copy((vec3){moon->RADIUS, moon->RADIUS, moon->RADIUS}, moon->SCALE);
    float grav_const = 0.6; // I know this is the wrong value according to real world physics but just let it be
    moon->ORBITAL_SPEED = sqrtf((grav_const * moon->PARENT_PLANET->MASS)/moon->ORBITAL_RADIUS);
}

void SOLAR_SYSTEM_INIT(SolarSystem* solar_system){
    solar_system->PLANET_COUNT = 0;
    solar_system->MOON_COUNT = 0;

    // Initializing the star of the system
    STAR_INIT(&(solar_system->CENTRAL_STAR), (vec3){0.0, 0.0, 0.0}, 5.0, 1000.0, "textures/models/sun/sun_base_color.png", "textures/models/sun/sun_specular.png", "SUN", "models/sphere/sphere.gltf");
    // INITIALIZING THE PLANETS
    PLANET_INIT(&(solar_system->PLANETS[0]), &(solar_system->CENTRAL_STAR), 1.0, 100.0, 50.0, "textures/models/earth/earth_base_color.png", "textures/models/earth/earth_specular.png", "earth", "models/sphere/sphere.gltf");
    solar_system->PLANET_COUNT++;
    PLANET_INIT(&(solar_system->PLANETS[1]), &(solar_system->CENTRAL_STAR), 0.7, 70.0, 80.0, "textures/models/mars/mars_base_color.png", "textures/models/mars/mars_specular.png", "mars", "models/sphere/sphere.gltf");
    solar_system->PLANET_COUNT++;
    MOON_INIT(&(solar_system->MOONS[0]), &(solar_system->PLANETS[0]), 0.3, 2.0, 10.0, "textures/models/moon/moon_base_color.png", "textures/models/moon/moon_specular.png", "moon1_earth", "models/sphere/sphere.gltf");
    solar_system->MOON_COUNT++;
}

void UPDATE_STAR(Star* star){
    /* Nothing yet */
}

void UPDATE_PLANET(Planet* planet, float delta_time){
    // Update the position data
    planet->POSITION[0] = planet->PARENT_STAR->POSITION[0] + (planet->PARENT_STAR->RADIUS + planet->ORBITAL_RADIUS) * cosf(planet->ORBITAL_SPEED * glfwGetTime());
    planet->POSITION[1] = planet->PARENT_STAR->POSITION[1];
    planet->POSITION[2] = planet->PARENT_STAR->POSITION[2] + (planet->PARENT_STAR->RADIUS + planet->ORBITAL_RADIUS) * sinf(planet->ORBITAL_SPEED * glfwGetTime());
    //update the rotation angle
    planet->ROTATION_ANGLE += 1.0 * delta_time;
}

void UPDATE_MOON(Moon* moon, float deltaTime){

    /*---------------------------POSITION UPDATION----------------------------*/
    if(moon->PARENT_PLANET != NULL){
        float current_time = glfwGetTime();
        float angle_radians = moon->ORBITAL_SPEED * current_time;

        float total_distance = moon->PARENT_PLANET->RADIUS + moon->ORBITAL_RADIUS;
        moon->POSITION[0] = moon->PARENT_PLANET->POSITION[0] + total_distance * cosf(angle_radians);
        moon->POSITION[1] = moon->PARENT_PLANET->POSITION[1];
        moon->POSITION[2] = moon->PARENT_PLANET->POSITION[2] + total_distance * sinf(angle_radians);
    }else{
        glm_vec3_zero(moon->POSITION);
    }
    /*---------------------------ROTATION UPDATION-----------------------------*/
    moon->ROTATION_ANGLE += moon->ROTATION_SPEED * deltaTime;
}

void UPDATE_SOLAR_SYSTEM(SolarSystem* solar_system, float delta_time){
    for(int i = 0; i < solar_system->PLANET_COUNT; i++){
        UPDATE_PLANET(&(solar_system->PLANETS[i]), delta_time);
        if(i < solar_system->MOON_COUNT){
            UPDATE_MOON(&(solar_system->MOONS[i]), delta_time);
        }   
    }
}

void RENDER_SOLAR_SYSTEM(SolarSystem* solar_system, Camera* camera, mat4* projection_matrix){
    // Render the central star first
    RENDER_STAR(&(solar_system->CENTRAL_STAR), camera, projection_matrix);
    // Render the planets
    for(int i = 0; i < solar_system->PLANET_COUNT; i++){
        RENDER_PLANET(&(solar_system->PLANETS[i]), camera, projection_matrix);
    }
    for(int i = 0; i < solar_system->MOON_COUNT; i++){
        RENDER_MOON(&(solar_system->MOONS[i]), camera, projection_matrix);
    }
}

void RENDER_MOON(Moon* moon, Camera* camera, mat4* projection_matrix){
        Model* MODEL = &moon->MODEL;
    
    USE_SHADER(&(MODEL->SHADER));
    // Set the textures and thens start setting the uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moon->DIFFUSE.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_diffuse1", 0);
    //Bind specular to texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, moon->SPECULAR.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_specular1", 1);
    
    // USING A 4x4 IDENTITY MATRIX FOR TESTING PURPOSES. WILL ADD DIFFERENT LOGIC FOR MODEL MATRIX WHEN WORKING ON PLANETARY MOTION
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, moon->POSITION);
    glm_scale(model, moon->SCALE);
    SET_MAT4(&(MODEL->SHADER), "model", &model);

    mat4 view;
    VIEW_MATRIX(camera, &view);
    SET_MAT4(&(moon->MODEL.SHADER), "view", &view);
    
    //Projection Matrix
    SET_MAT4(&(MODEL->SHADER), "projection", projection_matrix);

    vec3* viewPos = VIEW_POS(camera);
    SET_VEC3(&(MODEL->SHADER), "viewPos", viewPos);

    // Setting the light source uniforms
    vec3 lightColor = {1.0, 1.0, 1.0};
    Planet* planet = moon->PARENT_PLANET;
    SET_BOOL(&(MODEL->SHADER), "pntLight.enabled", planet->PARENT_STAR->POINT_LIGHT.enabled);
    SET_VEC3(&(MODEL->SHADER), "pntLight.position", &(planet->PARENT_STAR->POINT_LIGHT.position));
    SET_VEC3(&(MODEL->SHADER), "pntLight.color", &lightColor);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.specular", planet->PARENT_STAR->POINT_LIGHT.specular);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.diffuse", planet->PARENT_STAR->POINT_LIGHT.diffuse);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.ambient", planet->PARENT_STAR->POINT_LIGHT.ambient);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.constant", planet->PARENT_STAR->POINT_LIGHT.constant);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.linear", planet->PARENT_STAR->POINT_LIGHT.linear);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.quadratic", planet->PARENT_STAR->POINT_LIGHT.quadratic);
    SET_FLOAT(&(MODEL->SHADER), "shininess", 2.0);

    glBindVertexArray(moon->MODEL.VAO);
    GLenum gl_index_type = (moon->MODEL.INDEX_TYPE == cgltf_component_type_r_16u) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    glDrawElements(GL_TRIANGLES, moon->MODEL.INDEX_COUNT, gl_index_type, 0);
    glBindVertexArray(0);
}

void RENDER_PLANET(Planet* planet, Camera* camera, mat4* projection_matrix){    
    Model* MODEL = &planet->MODEL;
    
    USE_SHADER(&(MODEL->SHADER));
    // Set the textures and thens start setting the uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planet->DIFFUSE.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_diffuse1", 0);
    //Bind specular to texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, planet->SPECULAR.TEXTURE_OBJECT);
    SET_INT(&(MODEL->SHADER), "texture_specular1", 1);
    
    // USING A 4x4 IDENTITY MATRIX FOR TESTING PURPOSES. WILL ADD DIFFERENT LOGIC FOR MODEL MATRIX WHEN WORKING ON PLANETARY MOTION
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, planet->POSITION);
    glm_scale(model, planet->SCALE);
    glm_rotate(model, planet->ROTATION_ANGLE, planet->ROTATION_AXIS);
    SET_MAT4(&(MODEL->SHADER), "model", &model);

    mat4 view;
    VIEW_MATRIX(camera, &view);
    SET_MAT4(&(planet->MODEL.SHADER), "view", &view);
    
    //Projection Matrix
    SET_MAT4(&(MODEL->SHADER), "projection", projection_matrix);

    vec3* viewPos = VIEW_POS(camera);
    SET_VEC3(&(MODEL->SHADER), "viewPos", viewPos);

    // Setting the light source uniforms
    vec3 lightColor = {1.0, 1.0, 1.0};
    SET_BOOL(&(MODEL->SHADER), "pntLight.enabled", planet->PARENT_STAR->POINT_LIGHT.enabled);
    SET_VEC3(&(MODEL->SHADER), "pntLight.position", &(planet->PARENT_STAR->POINT_LIGHT.position));
    SET_VEC3(&(MODEL->SHADER), "pntLight.color", &lightColor);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.specular", planet->PARENT_STAR->POINT_LIGHT.specular);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.diffuse", planet->PARENT_STAR->POINT_LIGHT.diffuse);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.ambient", planet->PARENT_STAR->POINT_LIGHT.ambient);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.constant", planet->PARENT_STAR->POINT_LIGHT.constant);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.linear", planet->PARENT_STAR->POINT_LIGHT.linear);
    SET_FLOAT(&(MODEL->SHADER), "pntLight.quadratic", planet->PARENT_STAR->POINT_LIGHT.quadratic);
    SET_FLOAT(&(MODEL->SHADER), "shininess", 2.0);

    glBindVertexArray(planet->MODEL.VAO);
    GLenum gl_index_type = (planet->MODEL.INDEX_TYPE == cgltf_component_type_r_16u) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    glDrawElements(GL_TRIANGLES, planet->MODEL.INDEX_COUNT, gl_index_type, 0);
    glBindVertexArray(0);
}

void RENDER_STAR(Star* star, Camera* camera, mat4* projection_matrix){    
    USE_SHADER(&(star->MODEL.SHADER));
    // Set the textures and thens start setting the uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, star->DIFFUSE.TEXTURE_OBJECT);
    SET_INT(&(star->MODEL.SHADER), "texture_diffuse1", 0);
    //Bind specular to texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, star->SPECULAR.TEXTURE_OBJECT);
    SET_INT(&(star->MODEL.SHADER), "texture_specular1", 1);
    
    // USING A 4x4 IDENTITY MATRIX FOR TESTING PURPOSES. WILL ADD DIFFERENT LOGIC FOR MODEL MATRIX WHEN WORKING ON PLANETARY MOTION
    mat4 model;
    glm_mat4_identity(model);
    glm_scale(model, star->SCALE);
    glm_translate(model, star->POSITION);
    SET_MAT4(&(star->MODEL.SHADER), "model", &model);

    mat4 view;
    VIEW_MATRIX(camera, &view);
    SET_MAT4(&(star->MODEL.SHADER), "view", &view);
    
    //Projection Matrix
    SET_MAT4(&(star->MODEL.SHADER), "projection", projection_matrix);

    vec3* viewPos = VIEW_POS(camera);
    SET_VEC3(&(star->MODEL.SHADER), "viewPos", viewPos);

    // Setting the light source uniforms
    // No need to set the light uniforms for the sun since I'm taking it as the source of the light
    glBindVertexArray(star->MODEL.VAO);
    GLenum gl_index_type = (star->MODEL.INDEX_TYPE == cgltf_component_type_r_16u) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    glDrawElements(GL_TRIANGLES, star->MODEL.INDEX_COUNT, gl_index_type, 0);
    glBindVertexArray(0);
}