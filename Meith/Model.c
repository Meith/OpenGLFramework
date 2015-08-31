#include "Model.h"

#include <stdlib.h>
#include <string.h>

#include <SOIL/SOIL.h>

int Model_GetTotalMeshes(struct aiNode *node)
{
	int num_meshes = 0;
	unsigned int i;
	for (i = 0; i < node->mNumChildren; ++i)
	{
		num_meshes += Model_GetTotalMeshes(node->mChildren[i]);
	}

	return num_meshes + node->mNumMeshes;
}

Model *Model_Init(const char *path)
{
	Model *model = (Model *)malloc(sizeof(Model));

	const struct aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs);
	
	model->num_meshes = Model_GetTotalMeshes(scene->mRootNode);
	model->meshes = (Mesh *)malloc(sizeof(Mesh) * model->num_meshes);

	model->num_textures_loaded = 0;
	
	Model_ProcessNode(model, scene->mRootNode, scene, 0);

	return model;
}

void Model_ProcessNode(Model *model, struct aiNode *node, const struct aiScene *scene, int num_node)
{
	unsigned int i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes[num_node++] = Model_ProcessMesh(model, mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
		Model_ProcessNode(model, node->mChildren[i], scene, num_node);
}

int Model_GetTotalIndices(struct aiMesh *mesh)
{
	int num_indices = 0;
	unsigned int i;
	for (i = 0; i < mesh->mNumFaces; ++i)
	{
		num_indices += mesh->mFaces[i].mNumIndices;
	}

	return num_indices;
}

int Model_GetTotalTextures(struct aiMaterial *material)
{
	int num_textures = 0;
	
	num_textures += aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
	num_textures += aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);

	return num_textures;
}

Mesh Model_ProcessMesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene)
{
	int num_indices = Model_GetTotalIndices(mesh);
	int num_textures = Model_GetTotalTextures(scene->mMaterials[mesh->mMaterialIndex]);
	Mesh *temp_mesh = Mesh_Init(mesh->mNumVertices, num_indices, num_textures);

	unsigned int i;
	for (i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vec3 vector;

		// Vertices
		vector[0] = mesh->mVertices[i].x;
		vector[1] = mesh->mVertices[i].y;
		vector[2] = mesh->mVertices[i].z;
		vec3_Copy(vertex.v_position, vector);

		// Normals
		vector[0] = mesh->mNormals[i].x;
		vector[1] = mesh->mNormals[i].y;
		vector[2] = mesh->mNormals[i].z;
		vec3_Copy(vertex.v_normal, vector);

		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec[0] = mesh->mTextureCoords[0][i].x;
			vec[1] = mesh->mTextureCoords[0][i].y;
			vec2_Copy(vertex.v_texcoords, vec);
		}
		else
		{
			vec2_Zero(vertex.v_texcoords);
		}

		temp_mesh->vertices[i] = vertex;
	}

	unsigned int j;
	unsigned int k;
	for (i = 0, k = 0; i < mesh->mNumFaces; ++i)
	{
		struct aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (j = 0; j < face.mNumIndices; ++j)
		{
			temp_mesh->indices[k++] = face.mIndices[j];
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		int current_texture_count = 0;
		Model_LoadMaterialTextures(model, temp_mesh, current_texture_count, material, aiTextureType_DIFFUSE, "texture_diffuse");
		// 2. Specular maps
		Model_LoadMaterialTextures(model, temp_mesh, current_texture_count, material, aiTextureType_SPECULAR, "texture_specular");
	}

	return *temp_mesh;
}

void Model_LoadMaterialTextures(Model *model, Mesh *mesh, int current_texture_count, struct aiMaterial *material, enum aiTextureType type, char *type_name)
{
	unsigned int i;
	int j;
	for (i = 0; i < aiGetMaterialTextureCount(material, type); ++i)
	{
		struct aiString string;
		aiGetMaterialTexture(material, type, i, &string, NULL, NULL, NULL, NULL, NULL, 0);

		int skip = 0;
		for (j = 0; j < model->num_textures_loaded; ++j)
		{
			if (strcmp(model->textures_loaded[j].path.data, string.data))
			{
				mesh->textures[current_texture_count++] = model->textures_loaded[j];
				skip = 1;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = Model_TextureFromFile(string.data);
			texture.type = type_name;
			texture.path = string;
			mesh->textures[current_texture_count++] = texture;
			model->textures_loaded = realloc(model->textures_loaded, sizeof(Texture) * (model->num_textures_loaded + 1));
			model->textures_loaded[model->num_textures_loaded++] = texture;
		}
	}
}

GLint Model_TextureFromFile(const char *path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

void Model_Render(Model *model, Shader *shader)
{
	int i;
	for (i = 0; i < model->num_meshes; ++i)
		Mesh_Render(&model->meshes[i], shader);
}