#include "Light.h"

#include <stdlib.h>

DirectionalLight *DirectionalLight_Init()
{
	DirectionalLight *directional_light = (DirectionalLight *)malloc(sizeof(DirectionalLight));

	vec3 ambient= { 0.3f, 0.3f, 0.3f };
	vec3 diffuse = { 1.0f, 1.0f, 1.0f };
	vec3 direction = { 1.0f, 0.0f, 0.0f };

	vec3_Copy(directional_light->ambient, ambient);
	vec3_Copy(directional_light->diffuse, diffuse);
	vec3_Copy(directional_light->direction, direction);

	return directional_light;
}

void DirectionalLight_Render(DirectionalLight *directional_light, Shader *shader)
{
	glUniform3fv(glGetUniformLocation(shader->program, "directional_light.ambient"), 1, directional_light->ambient);
	glUniform3fv(glGetUniformLocation(shader->program, "directional_light.diffuse"), 1, directional_light->diffuse);
	glUniform3fv(glGetUniformLocation(shader->program, "directional_light.direction"), 1, directional_light->direction);
}

void DirectionalLight_Destroy(DirectionalLight *directional_light)
{
	free(directional_light);
}