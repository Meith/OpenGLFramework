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
	int num_textures;
} Model;

int Model_GetTotalMeshes(struct aiNode *node);
Model *Model_Init(const char *path);
void Model_ProcessNode(Model *model, struct aiNode *node, struct aiScene *scene, int num_node);
Mesh Model_ProcessMesh(struct aiMesh *mesh, struct aiScene *scene);
Texture *Model_LoadMaterialTextures(struct aiMaterial *material, enum aiTextureType type, char *type_name);
GLint Model_TextureFromFile(const char *path);

#endif