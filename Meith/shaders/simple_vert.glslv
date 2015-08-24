#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texcoords;

out vec3 temp_normal;
out vec2 temp_texcoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(i_position, 1.0f);
	temp_normal = vec3(u_model * vec4(i_normal, 1.0f));
	temp_texcoords = i_texcoords;
};
