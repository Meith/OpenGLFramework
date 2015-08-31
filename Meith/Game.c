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

	game->model = Model_Init("objects/nanosuit/nanosuit.obj");

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
		
	}

	if (game->input_handler->keystates[SDL_SCANCODE_DOWN])
	{
		
	}

	if (game->input_handler->keystates[SDL_SCANCODE_LEFT])
	{
		
	}

	if (game->input_handler->keystates[SDL_SCANCODE_RIGHT])
	{
		
	}
}

void Game_RenderUpdate(Game *game, const float dt)
{
	
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

		Model_Render(game->model, game->shader);
	}
	Shader_UnUseProg();

	WindowContext_SwapBuffers(game->window_context);
}

void Game_Destroy(Game *game)
{
	DirectionalLight_Destroy(game->directional_light);
	Camera_Destroy(game->camera);
	Shader_DestroyProg(game->shader);
	InputHandler_Destroy(game->input_handler);
	WindowContext_Destroy(game->window_context);
	free(game);
	
	SDL_Quit();
} 
