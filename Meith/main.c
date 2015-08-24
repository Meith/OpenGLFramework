#include <SDL/SDL.h>

#include "Game.h"

int main(int argc, char *argv[])
{
	const float dt = 0.01f;
	float t = 0.0f;
	float accumulator = 0.0f;
	float current_time = 0.0f;
	
	Game *game = Game_Init("OpenGL", 800, 600, SDL_WINDOW_OPENGL);
	
	while(game->quit_flag)
	{
		game->quit_flag = Game_HandleInput(game);
	
		float new_time = (float)SDL_GetTicks() / 1000.0f;
		float delta_time = new_time - current_time;
		
		if(delta_time <= 0.0f)
			continue;
			
		current_time = new_time;
		
		accumulator += delta_time;
		while(accumulator >= dt)
		{
			Game_FixedUpdate(game, t, dt);
			accumulator -= dt;
			t += dt;
		}

		Game_RenderUpdate(game, dt);
		Game_Render(game);
	}
	
	Game_Destroy(game);
	
	return 0;
}
