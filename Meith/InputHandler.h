#ifndef __InputHandler__
#define __InputHandler__

#include <SDL/SDL.h>

#include "Vector.h"
#include "WindowContext.h"

typedef enum MouseButtons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
} MouseButtons;

typedef struct InputHandler
{
	int mouse_buttonstate[3];
	vec2 mouse_pos;
	const Uint8 *keystates;

	WindowContext window_context;
} InputHandler;

// handle mouse events
void InputHandler_MouseMove(InputHandler *input_handler, SDL_Event *event);
void InputHandler_MouseButtonDown(InputHandler *input_handler, SDL_Event *event);
void InputHandler_MouseButtonUp(InputHandler *input_handler, SDL_Event *event);

// handle keyboard events
void InputHandler_KeyboardKeyDown(InputHandler *input_handler);
void InputHandler_KeyboardKeyUp(InputHandler *input_handler);

InputHandler *InputHandler_Init(WindowContext *window_context);
int InputHandler_Update(InputHandler *input_handler);
void InputHandler_Destroy(InputHandler *input_handler);

#endif
