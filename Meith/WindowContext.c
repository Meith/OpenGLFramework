#include "WindowContext.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <GL/glew.h>

WindowContext *WindowContext_Init(const char *title, int width, int height, int flags)
{
	WindowContext *window_context = (WindowContext *)malloc(sizeof(WindowContext));

	// Sdl initialization
	SDL_Init(SDL_INIT_EVERYTHING);

	// Setting opengl version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	// Creating Window
	window_context->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	// Setting context of the window
	window_context->gl_context = SDL_GL_CreateContext(window_context->window);

	// Glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	assert(err == GLEW_OK);

	// Print opengl version
	printf("Using opengl version %s.\n", glGetString(GL_VERSION));
	printf("Using glsl version %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return window_context;
}

void WindowContext_SwapBuffers(WindowContext *window_context)
{
	SDL_GL_SwapWindow(window_context->window);
}

void WindowContext_Destroy(WindowContext *window_context)
{
	SDL_GL_DeleteContext(window_context->gl_context);
	SDL_DestroyWindow(window_context->window);
	free(window_context);
}