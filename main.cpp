#include <cstdio>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <gl/GL.h>
#include <gl/GLU.h>
#include <SDL.h>

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_VIDEO);

	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_Window* window = SDL_CreateWindow("Motor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, windowFlags);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	bool appRunning = true;

	while (appRunning)
	{
		SDL_Event curEvent;
		while (SDL_PollEvent(&curEvent))
		{
			glViewport(0, 0, 1024, 768);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			

			glColor4f(1.0, 1.0, 0.0, 1.0);
			glBegin(GL_TRIANGLES);
			glVertex3f(-1.0, 0.0, 0.0);
			glVertex3f(0.0, 1.0, 0.0);
			glVertex3f(1.0, 0.0, 0.0);

			glEnd();
			SDL_GL_SwapWindow(window);
		}
	}
	return 0;
}