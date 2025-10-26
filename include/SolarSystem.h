#pragma once
#include "ModelLoader.h"

// ADD THIS WRAPPER AROUND YOUR C FUNCTION DECLARATIONS
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PLANETS 8
#define MAX_MOONS 16

typedef enum{
    BODY_TYPE_STAR,
    BODY_TYPE_PLANET,
    BODY_TYPE_MOON
} BODY_TYPE;


typedef struct PointLight{
    bool enabled;
    vec3 position;
    vec3 color;
    
    // Attribute Strength
    float specular;
    float diffuse;
    float ambient;

    // Attenuation
    float constant, linear, quadratic;
} PointLight;

typedef struct Star{
    const char* NAME;
    Model MODEL;
    // Texture data
    Texture DIFFUSE;
    Texture SPECULAR;
    const char* DIFFUSE_PATH;
    const char* SPECULAR_PATH;
    // Type data
    BODY_TYPE TYPE;
    // Light source
    PointLight POINT_LIGHT;    
    // Transformation data
    vec3 POSITION;
    float RADIUS;
    float MASS;
    vec3 ROTATION;
    vec3 SCALE;

} Star;

typedef struct Planet{
    const char* NAME;
    Model MODEL;
    // Texture data
    Texture DIFFUSE;
    Texture SPECULAR;
    const char* DIFFUSE_PATH;
    const char* SPECULAR_PATH;
    // Type data
    BODY_TYPE TYPE;
    // Transformation data
    vec3 POSITION;
    float RADIUS;
    float MASS;
    vec3 ROTATION;
    vec3 SCALE;

    // Rotation data
    float ROTATION_SPEED;
    float ROTATION_ANGLE;
    vec3 ROTATION_AXIS;
    // Orbital Path Data
    Star* PARENT_STAR; // Can be derived from the parent star's data)
    float ORBITAL_RADIUS;
    float ORBITAL_SPEED;
    
} Planet;

typedef struct Moon{
    const char* NAME;
    Model MODEL;
    // Texture data
    Texture DIFFUSE;
    Texture SPECULAR;
    const char* DIFFUSE_PATH;
    const char* SPECULAR_PATH;
    // Type data
    BODY_TYPE TYPE;
    // Transformation data
    vec3 POSITION;
    float RADIUS;
    float MASS;
    vec3 ROTATION;
    vec3 SCALE;

    // Rotation data
    float ROTATION_SPEED;
    float ROTATION_ANGLE;
    vec3 ROTATION_AXIS;
    // Orbital Path Data
    Planet* PARENT_PLANET;
    float ORBITAL_RADIUS;
    float ORBITAL_SPEED;
    
}Moon;

typedef struct SolarSystem{
    Star CENTRAL_STAR;
    Planet PLANETS[MAX_PLANETS];
    Moon MOONS[MAX_MOONS];

    int MOON_COUNT;
    int PLANET_COUNT;
} SolarSystem;


/*-------------------------FUNCTIONS FOR THE STARS AND PLANETS----------------------------------------*/
void STAR_INIT(Star* star, vec3 star_position,  float star_radius, float star_mass, const char* diffuse_texture, const char* specular_texture, const char* star_name, const char* sphere_model_path);
void PLANET_INIT(Planet* planet, Star* parent_star, float planet_radius, float planet_mass, float orbital_radius, const char* diffuse_texture, const char* specular_texture, const char* planet_name, const char* sphere_model_path);
void MOON_INIT(Moon* moon, Planet* parent_planet, float moon_radius, float moon_mass, float orbital_radius, const char* diffuse_texture, const char* specular_texture, const char* moon_name, const char* sphere_model_path);
void SOLAR_SYSTEM_INIT(SolarSystem* solar_system);

void UPDATE_STAR(Star* star);
void UPDATE_PLANET(Planet* planet, float delta_time);
void UPDATE_MOON(Moon* moon, float deltaTime);
void UPDATE_SOLAR_SYSTEM(SolarSystem* solar_system, float delta_time);

void RENDER_MOON(Moon* moon, Camera* camera, mat4* projection_matrix);
void RENDER_PLANET(Planet* planet, Camera* camera, mat4* projection_matrix);
void RENDER_STAR(Star* star, Camera* camera, mat4* projection_matrix);
void RENDER_SOLAR_SYSTEM(SolarSystem* solar_system, Camera* camera, mat4* projection_matrix);

// ADD THIS CLOSING BRACE AT THE END
#ifdef __cplusplus
}
#endif