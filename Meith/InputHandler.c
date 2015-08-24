#include "InputHandler.h"

#include <stdlib.h>

InputHandler *InputHandler_Init(WindowContext *window_context)
{
	InputHandler *input_handler = (InputHandler *)malloc(sizeof(InputHandler));
	
	int i = 0;
	for(i = 0; i < 3; ++i)
		input_handler->mouse_buttonstate[i] = 0;
	
	input_handler->keystates = SDL_GetKeyboardState(NULL);

	input_handler->window_context = *window_context;

	return input_handler;
}

int InputHandler_Update(InputHandler *input_handler)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return 0;
				break;
			
			case SDL_MOUSEMOTION:
				InputHandler_MouseMove(input_handler, &event);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				InputHandler_MouseButtonDown(input_handler, &event);
				break;
				
			case SDL_MOUSEBUTTONUP:
				InputHandler_MouseButtonUp(input_handler, &event);
				break;
				
			case SDL_KEYDOWN:
				InputHandler_KeyboardKeyDown(input_handler);
				break;
				
			case SDL_KEYUP:
				InputHandler_KeyboardKeyUp(input_handler);
				
			default:
				break;
		}
	}
	
	return 1;
}

void InputHandler_MouseMove(InputHandler *input_handler, SDL_Event *event)
{
	int w, h;
	SDL_GetWindowSize(input_handler->window_context.window, &w, &h);
	input_handler->mouse_pos[0] = (float)event->motion.x / (float)w;
	input_handler->mouse_pos[1] = -(float)event->motion.y / (float)h;
	int i;
	for (i = 0; i < 2; ++i)
		input_handler->mouse_pos[i] = (input_handler->mouse_pos[i] * 2) - 1;
}

void InputHandler_MouseButtonDown(InputHandler *input_handler, SDL_Event *event)
{
	if(event->button.button == SDL_BUTTON_LEFT)
        	input_handler->mouse_buttonstate[LEFT] = 1;
    
    	if(event->button.button == SDL_BUTTON_MIDDLE)
        	input_handler->mouse_buttonstate[MIDDLE] = 1;
    
    	if(event->button.button == SDL_BUTTON_RIGHT)
        	input_handler->mouse_buttonstate[RIGHT] = 1;
}

void InputHandler_MouseButtonUp(InputHandler *input_handler, SDL_Event *event)
{
	if(event->button.button == SDL_BUTTON_LEFT)
        	input_handler->mouse_buttonstate[LEFT] = 0;
    
   	if(event->button.button == SDL_BUTTON_MIDDLE)
        	input_handler->mouse_buttonstate[MIDDLE] = 0;
    
    	if(event->button.button == SDL_BUTTON_RIGHT)
        	input_handler->mouse_buttonstate[RIGHT] = 0;
}

int InputHandler_IsMouseButtonPressed(InputHandler *input_handler, int mouse_button)
{
	return input_handler->mouse_buttonstate[mouse_button];
}

void InputHandler_KeyboardKeyDown(InputHandler *input_handler)
{
	input_handler->keystates = SDL_GetKeyboardState(0);
}

void InputHandler_KeyboardKeyUp(InputHandler *input_handler)
{
	input_handler->keystates = SDL_GetKeyboardState(0);
}

int InputHandler_IsKeyPressed(InputHandler *input_handler, SDL_Scancode key)
{
	if(input_handler->keystates[key] == SDL_PRESSED)
		return  1;	
	else	
		return 0;
}

void InputHandler_Destroy(InputHandler *input_handler)
{
	free(input_handler);
}

