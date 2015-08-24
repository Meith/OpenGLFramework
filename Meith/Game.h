#ifndef __Game__
#define __Game__

#include <SDL/SDL.h>

#include "WindowContext.h"
#include "InputHandler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "RigidBody.h"
#include "Light.h"
#include "GameObject.h"

typedef enum OBJECT_TYPE
{
	CUBE = 0,
	CONE = 1,
	CYLINDER = 2,
	SPHERE = 3,
	TORUS = 4,
	HELIX = 5
} OBJECT_TYPE;

typedef struct Game
{
	int quit_flag;
	
	WindowContext *window_context;
	InputHandler *input_handler;
	Shader *shader;
	Mesh *mesh;
	Transform *transform;
	Camera *camera;
	RigidBody *rigid_body;
	DirectionalLight *directional_light;
	GameObject *moving_object[5];
	GameObject *still_object;
	// To add later: List of gameobjects, cameras, shaders and lights
} Game;

Game *Game_Init(const char *title, const int width, const int height, int flags);
int Game_HandleInput(Game *game);
void Game_FixedUpdate(Game *game, float t, const float dt);
void Game_RenderUpdate(Game *game, const float dt);
void Game_Render(Game *game);
void Game_Destroy(Game *game); 

#endif
