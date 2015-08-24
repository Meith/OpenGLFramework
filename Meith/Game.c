#include "Game.h"
#include "Vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Game *Game_Init(const char *title, const int width, const int height, int flags)
{
	Game *game = (Game *)malloc(sizeof(Game));
	assert(game != NULL);
	game->quit_flag = 1;

	game->window_context = WindowContext_Init(title, width, height, flags);
	game->input_handler = InputHandler_Init(game->window_context);
	game->shader = Shader_CreateProg("shaders/simple_vert.glslv", "shaders/simple_frag.glslf");
	game->camera = Camera_Init();
	game->directional_light = DirectionalLight_Init();

	game->moving_object[0] = GameObject_Init(SPHERE, "textures/blue.png", game->shader);
	vec3 up = { 0.0f, 1.0f, 0.0f };
	Transform_MoveDirection(game->moving_object[0]->transform, up, 0.5f);

	int i;
	for (i = 1; i < 5; ++i)
		game->moving_object[i] = GameObject_Init(CUBE, "textures/yellow.png", game->shader);
	vec3 forward = { 0.0f, 0.0f, -1.0f };
	vec3 back = { 0.0f, 0.0f, 1.0f };
	vec3 left = { -1.0f, 0.0f, 0.0f };
	vec3 right = { 1.0f, 0.0f, 0.0f };
	float disp = 5.0f;
	Transform_MoveDirection(game->moving_object[1]->transform, forward, disp);
	Transform_MoveDirection(game->moving_object[2]->transform, back, disp);
	Transform_MoveDirection(game->moving_object[3]->transform, left, disp);
	Transform_MoveDirection(game->moving_object[4]->transform, right, disp);

	Transform_MoveDirection(game->moving_object[0]->transform, left, disp);

	game->still_object = GameObject_Init(CUBE, "textures/grey.png", game->shader);
	vec3 scale = { 20.0f, 0.01f, 20.0f };
	Transform_Scale(game->still_object->transform, scale);
	GameObject_RenderUpdate(game->still_object);

	return game;
}

int Game_HandleInput(Game *game)
{
	game->quit_flag = InputHandler_Update(game->input_handler);
	
	return game->quit_flag;		
}

void Game_FixedUpdate(Game *game, float t, const float dt)
{
	if (game->input_handler->keystates[SDL_SCANCODE_UP])
	{
		game->moving_object[0]->rigid_body->current.direction[2] = -1.0f;
		game->moving_object[0]->rigid_body->current.speed = 1.0f;
	}

	if (game->input_handler->keystates[SDL_SCANCODE_DOWN])
	{
		game->moving_object[0]->rigid_body->current.direction[2] = 1.0f;
		game->moving_object[0]->rigid_body->current.speed = 1.0f;
	}

	if (game->input_handler->keystates[SDL_SCANCODE_LEFT])
	{
		game->moving_object[0]->rigid_body->current.direction[0] = -1.0f;
		game->moving_object[0]->rigid_body->current.speed = 1.0f;
	}

	if (game->input_handler->keystates[SDL_SCANCODE_RIGHT])
	{
		game->moving_object[0]->rigid_body->current.direction[0] = 1.0f;
		game->moving_object[0]->rigid_body->current.speed = 1.0f;
	}

	GameObject_FixedUpdate(game->moving_object[0], t, dt);
	vec3_Zero(game->moving_object[0]->rigid_body->current.direction);
}

void Game_RenderUpdate(Game *game, const float dt)
{
	int i;
	vec3 axis = { 1.0f, 1.0f, -1.0f };
	float angle = 20.0f;
	for (i = 1; i < 5; ++i)
		Transform_RotateAxis(game->moving_object[i]->transform, axis, angle * dt);

	for (i = 0; i < 5; ++i)
		GameObject_RenderUpdate(game->moving_object[i]);
}

void Game_Render(Game *game)
{
	// Clear screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader_UseProg(game->shader);
	{
		Camera_Render(game->camera, game->shader);
		DirectionalLight_Render(game->directional_light, game->shader);

		// render: List of gameobjects, cameras, lights
		GameObject_Render(game->still_object, game->shader);
		int i;
		for (i = 0; i < 5; ++i)
			GameObject_Render(game->moving_object[i], game->shader);
	}
	Shader_UnUseProg();

	WindowContext_SwapBuffers(game->window_context);
}

void Game_Destroy(Game *game)
{
	int i;
	for (i = 0; i < 5; ++i)
		GameObject_Destroy(game->moving_object[i]);
	GameObject_Destroy(game->still_object);
	DirectionalLight_Destroy(game->directional_light);
	Camera_Destroy(game->camera);
	Shader_DestroyProg(game->shader);
	InputHandler_Destroy(game->input_handler);
	WindowContext_Destroy(game->window_context);
	free(game);
	
	SDL_Quit();
} 
