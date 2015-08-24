#include "Mesh.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "SOIL/SOIL.h"

Mesh *Mesh_Init(const char *object_file, const int obj_data[6])
{
	Mesh *mesh = (Mesh *)malloc(sizeof(Mesh));
	assert(mesh != NULL);

	// mesh data
	vec3 *positions = (vec3 *)malloc(obj_data[0] * sizeof(vec3));
	vec3 *normals = (vec3 *)malloc(obj_data[1] * sizeof(vec3));
	vec2 *uvs = (vec2 *)malloc(obj_data[2] * sizeof(vec2));
	
	assert(positions != NULL);
	assert(normals != NULL);
	assert(uvs != NULL); 
	
	int num_pos = 0;
	int num_norm = 0;
	int num_uvs = 0;
	
	mesh->vertices = (Vertex *)malloc(obj_data[3] * sizeof(Vertex));
	mesh->num_vertices = 0;
	assert(mesh->vertices != NULL);
	
	mesh->indices = (GLuint *)malloc(obj_data[4] * sizeof(GLuint));
	mesh->num_indices = 0;  
	assert(mesh->indices != NULL);
	
	mesh->textures = (Texture *)malloc(obj_data[5] * sizeof(Texture));
	mesh->num_textures = 0;
	assert(mesh->textures != NULL);
	
	FILE *fptr;
	fptr = fopen(object_file, "r");
	assert(fptr != NULL);
	
	char buffer[101];
	
	int ctr = 0;
	
	while(fgets(buffer,sizeof(buffer),fptr)!= NULL)
	{			
		// check type of line
		if(buffer[0] == 'v' && buffer[1] == 't')
		{
			vec2 uv;
			ctr = sscanf(buffer, "vt %f %f", &uv[0], &uv[1]);
			assert(ctr == 2);
			memcpy(uvs[num_uvs], uv, sizeof(uv));
			++num_uvs;
		}
		
		else if(buffer[0] == 'v' && buffer[1] == 'n')
		{
			vec3 norm;
			ctr = sscanf(buffer, "vn %f %f %f", &norm[0], &norm[1], &norm[2]);
			assert(ctr == 3);
			memcpy(normals[num_norm], norm, sizeof(norm));
			++num_norm;
		}
		
		else if(buffer[0] == 'v')
		{
			vec3 pos;
			ctr = sscanf(buffer, "v %f %f %f", &pos[0], &pos[1], &pos[2]);
			assert(ctr == 3);
			memcpy(positions[num_pos], pos, sizeof(pos));
			++num_pos;	
		}
	
		else if(buffer[0] == 'f')
		{
			GLuint ind[9];
			ctr = sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &ind[0], &ind[1], &ind[2], &ind[3], &ind[4], &ind[5], &ind[6], &ind[7], &ind[8]);
			assert(ctr == 9);
			
			// Create vertices by looking up corresponding data from vectors
			// OBJ file indices are 1-based, sp they need to be adjusted
			Vertex v1;
			memcpy(&v1.v_position, positions[ind[0] - 1], sizeof(v1.v_position));
			memcpy(&v1.v_texcoords, uvs[ind[1] - 1], sizeof(v1.v_texcoords));
			memcpy(&v1.v_normal, normals[ind[2] - 1], sizeof(v1.v_normal)); 
			
			Vertex v2;
			memcpy(&v2.v_position, positions[ind[3] - 1], sizeof(v2.v_position));
			memcpy(&v2.v_texcoords, uvs[ind[4] - 1], sizeof(v2.v_texcoords));
			memcpy(&v2.v_normal, normals[ind[5] - 1], sizeof(v2.v_normal)); 
			
			Vertex v3;
			memcpy(&v3.v_position, positions[ind[6] - 1], sizeof(v3.v_position));
			memcpy(&v3.v_texcoords, uvs[ind[7] - 1], sizeof(v3.v_texcoords));
			memcpy(&v3.v_normal, normals[ind[8] - 1], sizeof(v3.v_normal));
			
			// Add the vertices to current mesh->vertices
			mesh->vertices[mesh->num_vertices++] = v1;
			mesh->vertices[mesh->num_vertices++] = v2;
			mesh->vertices[mesh->num_vertices++] = v3;
			
			// Add indices to current mesh->indices
			// Indices are just 0, 1, 2...num_indices :|
			int i = 0;
			for(i = 0; i < 3; ++i, ++mesh->num_indices) 
				mesh->indices[mesh->num_indices] = mesh->num_indices;	
		}
	}
	
	fclose(fptr);
	
	assert(mesh->num_vertices == obj_data[3]);
	assert(mesh->num_indices == obj_data[4]);
	
	// free temp arrays
	free(positions);
	free(normals);
	free(uvs);
	
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
