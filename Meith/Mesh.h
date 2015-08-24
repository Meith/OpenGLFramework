#ifndef __Mesh__
#define __Mesh__

#include "GL/glew.h"

#include "Vector.h"
#include "Shader.h"

typedef struct Vertex
{
	vec3 v_position;
	vec3 v_normal;
	vec2 v_texcoords;
} Vertex;

typedef struct Texture
{
	GLuint id;
	char *type;
} Texture;

typedef struct Mesh
{
	// Mesh data
	Vertex *vertices;
	int num_vertices;
	
	GLuint *indices;
	int num_indices;

	Texture *textures;
	int num_textures;

	// Render data
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
} Mesh;

Mesh *Mesh_Init(const char *object_file, const int obj_data[6]);
void Mesh_LoadTexture(Mesh *mesh, const char *text_path, const char *text_type);
void Mesh_Setup(Mesh *mesh, Shader *shader);
void Mesh_Render(Mesh *mesh, Shader *shader);
void Mesh_Destroy(Mesh *mesh);

#endif
