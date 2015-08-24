#include "Model.h"

#include <stdlib.h>

int Model_GetTotalMeshes(struct aiNode *node)
{
	int num_meshes = 0;
	int i;
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
	
	Model_ProcessNode(model, scene->mRootNode, scene, 0);

	return model;
}

void Model_ProcessNode(Model *model, struct aiNode *node, struct aiScene *scene, int num_node)
{
	int i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes[num_node++] = Model_ProcessMesh(mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
		Model_ProcessNode(model, node->mChildren[i], scene, num_node);
}

Mesh Model_ProcessMesh(struct aiMesh *mesh, struct aiScene *scene)
{
	Mesh *temp_mesh = Mesh_Init(mesh->mNumVertices, )
}

Texture *Model_LoadMaterialTextures(struct aiMaterial *material, enum aiTextureType type, char *type_name);
GLint Model_TextureFromFile(const char *path);