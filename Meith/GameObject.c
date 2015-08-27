#include "GameObject.h"
#include "ObjectData.h"

#include <stdlib.h>

GameObject *GameObject_Init(int object_type, char *text_path, Shader *shader)
{
	GameObject *game_object = (GameObject *)malloc(sizeof(GameObject));

	//game_object->mesh = Mesh_Init(object_name[object_type], object_data[object_type]);
	//Mesh_LoadTexture(game_object->mesh, text_path, "texture_diffuse");
	Mesh_Setup(game_object->mesh, shader);

	game_object->transform = Transform_Init();
	game_object->rigid_body = RigidBody_Init(*game_object->transform);

	return game_object;
}

void GameObject_FixedUpdate(GameObject *game_object, float t, const float dt)
{
	RigidBody_Update(game_object->rigid_body, game_object->transform, t, dt);
	RigidBody_UpdateTransform(game_object->rigid_body, game_object->transform);
}

void GameObject_RenderUpdate(GameObject *game_object)
{
	Transform_GetWorldMat(game_object->transform);
}

void GameObject_Render(GameObject *game_object, Shader *shader)
{
	//Mesh_Resend(game_object->mesh, shader);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "u_model"), 1, GL_TRUE, &game_object->transform->world_matrix[0][0]);
	Mesh_Render(game_object->mesh, shader);
}

void GameObject_Destroy(GameObject *game_object)
{
	free(game_object->mesh);
	free(game_object->rigid_body);
	free(game_object->transform);
	free(game_object);
}
