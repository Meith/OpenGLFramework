#ifndef __Model__
#define __Model__

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

typedef struct Model
{
	Mesh *meshes;
	int num_meshes;
	Texture *textures_loaded;
	int num_textures_loaded;
} Model;

int Model_GetTotalMeshes(struct aiNode *node);
Model *Model_Init(const char *path);
void Model_ProcessNode(Model *model, struct aiNode *node, const struct aiScene *scene, int num_node);
int Model_GetTotalIndices(struct aiMesh *mesh);
int Model_GetTotalTextures(struct aiMaterial *material);
Mesh Model_ProcessMesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene);
void Model_LoadMaterialTextures(Model *model, Mesh *mesh, int current_texture_count, struct aiMaterial *material, enum aiTextureType type, char *type_name);
GLint Model_TextureFromFile(const char *path);

#endif