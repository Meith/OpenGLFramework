#include "Mesh.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "SOIL/SOIL.h"

Mesh *Mesh_Init(int num_vertices, int num_indices, int num_textures)
{
	Mesh *mesh = (Mesh *)malloc(sizeof(Mesh));
	
	mesh->num_vertices = num_vertices;
	mesh->vertices = (Vertex *)malloc(sizeof(Vertex) * mesh->num_vertices);

	mesh->num_indices = num_indices;
	mesh->indices = (GLuint *)malloc(sizeof(GLuint) * mesh->num_indices);

	mesh->num_textures = num_textures;
	mesh->textures = (Texture *)malloc(sizeof(Texture) * mesh->num_textures);
	
	return mesh;
}

void Mesh_LoadTexture(Mesh *mesh, const char *text_path, const char *text_type)
{	
	glGenTextures(1, &(mesh->textures[mesh->num_textures].id));
	int w, h;
	unsigned char *image = SOIL_load_image(text_path, &w, &h, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, mesh->textures[mesh->num_textures].id);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    	glGenerateMipmap(GL_TEXTURE_2D);	

    	// Parameters
    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glBindTexture(GL_TEXTURE_2D, 0);
    	SOIL_free_image_data(image);
	    	
    	mesh->textures[mesh->num_textures].type = (char*)malloc(sizeof(text_type));
    	strcpy(mesh->textures[mesh->num_textures].type, text_type);
    	
    	++mesh->num_textures;
}

void Mesh_Setup(Mesh *mesh, Shader *shader)
{	
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);
	
	glBindVertexArray(mesh->vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

		GLuint pos_attrib = glGetAttribLocation(shader->program, "i_position");
		glEnableVertexAttribArray(pos_attrib);
		glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, v_position));

		GLuint norm_attrib = glGetAttribLocation(shader->program, "i_normal");
		glEnableVertexAttribArray(norm_attrib);
		glVertexAttribPointer(norm_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, v_normal));

		GLuint tex_attrib = glGetAttribLocation(shader->program, "i_texcoords");
		glEnableVertexAttribArray(tex_attrib);
		glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, v_texcoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
}

void Mesh_Render(Mesh *mesh, Shader *shader)
{	
	GLuint diffuse_no = 1;
	GLuint specular_no = 1;
	
	int i = 0;
	for(i = 0; i < mesh->num_textures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		char number[3] = "";
		char name[25] = "";
		strcpy(name, mesh->textures[i].type);
		if(strcmp(name, "texture_diffuse") == 0)
			sprintf(number, "%d", diffuse_no++);
		else if(strcmp(name, "texture_specular") == 0)
			sprintf(number, "%d", specular_no++);

		char material[30] = "my_material.";
		strcat(material, strcat(name, number));
		
		glUniform1f(glGetUniformLocation(shader->program, material), (GLfloat)i);
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}
	
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	for(i = 0; i < mesh->num_textures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
        	glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh_Destroy(Mesh *mesh)
{
	free(mesh->vertices);
	free(mesh->indices);
	free(mesh->textures);
	free(mesh);
}
