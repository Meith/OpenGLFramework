#ifndef __WindowContext__
#define __WindowContext__

#include <SDL/SDL.h>

typedef struct WindowContext
{
	SDL_Window *window;
	SDL_GLContext gl_context;

} WindowContext;

WindowContext *WindowContext_Init(const char *title, int width, int height, int flags);
void WindowContext_SwapBuffers(WindowContext *window_context);
void WindowContext_Destroy(WindowContext *window_context);

#endif