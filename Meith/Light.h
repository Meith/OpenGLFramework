#ifndef __Light__
#define __Light__

#include "Vector.h"
#include "Shader.h"

typedef struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 direction;
} DirectionalLight;

DirectionalLight *DirectionalLight_Init();
void DirectionalLight_Render(DirectionalLight *directional_light, Shader *shader);
void DirectionalLight_Destroy(DirectionalLight *directional_light);

#endif