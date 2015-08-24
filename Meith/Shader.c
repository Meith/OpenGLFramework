#include "Shader.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// Function to read shader contents from file into char buffer
char *Shader_ReadFile(const GLchar *shaderpath)
{
	FILE *fptr;
	long length;
	char *buffer;	// char buffer for shader contents
	
	// Open file for reading and check if successful
	fptr = fopen(shaderpath, "rb");
	if(!fptr)
	{
		return NULL;
	}
	
	// Seek till the end to find length of file
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	
	// Create char buffer the size of length to store shader contents
	buffer = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);	// Go back to start of file
	fread(buffer, length, sizeof(char), fptr);
	buffer[length] = 0;	// Null terminator
	
	// Close file
	fclose(fptr);
	
	// Return character buffer with contents of shader
	return buffer;
}

// Function to compile, create and return a shader program
Shader *Shader_CreateProg(const GLchar *vert_path, const GLchar *frag_path)
{
	// Read vertex shader into char buffer
	const GLchar *vert_source = Shader_ReadFile(vert_path);
	assert(vert_source != NULL);
	
	// Read fragment shader into char buffer
	const GLchar *frag_source = Shader_ReadFile(frag_path);
	assert(frag_source != NULL);
	
	// Compiling vertex shader
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);
	
	// Check compilation for errors in vertex shader
	GLint status;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
	printf("Vertex Shader compile status %d\n", status);
	
	char error_log[512];
	glGetShaderInfoLog(vert_shader, 512, NULL, error_log);
	printf("Vertex Shader error log: %s\n", error_log);
	
	// Compiling fragment shader
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_source, NULL);
	glCompileShader(frag_shader);
	
	// Check for compilation errors in fragment shader
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
	printf("Fragment Shader compile status %d\n", status);
	
	glGetShaderInfoLog(frag_shader, 512, NULL, error_log);
	printf("Fragment Shader error log: %s\n", error_log);
	
	// Convert shader compiled objects into executable program
	Shader *shader = (Shader *)malloc(sizeof(Shader));
	shader->program = glCreateProgram();
	glAttachShader(shader->program, vert_shader);
	glAttachShader(shader->program, frag_shader);
	glLinkProgram(shader->program);
	
	// Delete shaders as they are linked
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
	
	return shader;
}

// Function to use shader program
void Shader_UseProg(Shader *shader)
{
	glUseProgram(shader->program);
}

void Shader_UnUseProg()
{
	glUseProgram(0);
}

// Funtion to delete shader program
void Shader_DestroyProg(Shader *shader)
{
	glDeleteProgram(shader->program);
	free(shader);
}
