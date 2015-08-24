#ifndef __GameObject__
#define __GameObject__

#include "Mesh.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Shader.h"

typedef struct GameObject
{
	Mesh *mesh;
	Transform *transform;
	RigidBody *rigid_body;
} GameObject;

GameObject *GameObject_Init(int object_type, char *text_path, Shader *shader);
void GameObject_FixedUpdate(GameObject *game_object, float t, const float dt);
void GameObject_RenderUpdate(GameObject *game_object);
void GameObject_Render(GameObject *game_object, Shader *shader);
void GameObject_Destroy(GameObject *game_object);

#endif