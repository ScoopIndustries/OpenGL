#include <cstdio>
#include <iostream>
#include <ratio>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SDL.h>

/// <summary>
/// Camera Free / Rotation around Object
/// </summary>

class APP {
public:
	const float cameraSpeed = 0.5f;
	//Camera Position
	glm::vec3 cameraPosition;
	//Camera Direction
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	//Right Axis
	glm::vec3 up;
	glm::vec3 cameraRight;
	//UpAxis
	glm::vec3 cameraUp;
	//Camera Front
	glm::vec3 cameraFront;

	APP()
	{
		cameraPosition = glm::vec3(0.0f, 0.0f, 0.3f);
		cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	}

	void InputHandler(bool& appRunning, SDL_Event& event, std::chrono::steady_clock::time_point& prevTime)
	{
		auto resultCloak = Cloak(prevTime);

		switch (event.type) {
		case SDL_QUIT:
			appRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				appRunning = false;
				break;
			case SDLK_z:
				cameraPosition += cameraSpeed * cameraFront;
				break;
			case SDLK_q:
				cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				break;
			case SDLK_s:
				cameraPosition -= cameraSpeed * cameraFront;
				break;
			case SDLK_d:
				cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				break;
			}
		
		}
	}

	float Cloak(std::chrono::steady_clock::time_point& prevTime)
	{
		auto curTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> fTime = curTime - prevTime;
		return fTime.count();
	}

};


int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_VIDEO);

	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_Window* window = SDL_CreateWindow("Motor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, windowFlags);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	bool appRunning = true;
	auto prevTime = std::chrono::steady_clock::now();

	APP app;
	while (appRunning)
	{
		SDL_Event curEvent;
		while (SDL_PollEvent(&curEvent))
		{
			app.InputHandler(appRunning, curEvent, prevTime);
			
		}
		//LookAt
		glm::mat4 view;
		view = glm::lookAt(app.cameraPosition, app.cameraPosition + app.cameraFront, app.up);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&view[0][0]);

		glViewport(0, 0, 1024, 768);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Creation d'un triangle
		glColor4f(0.0, 1.0, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);

		glEnd();

		// Creation d'un autre triangle
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0, 0.0, -1.0);
		glVertex3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);

		glEnd();
		SDL_GL_SwapWindow(window);
	}
	return 0;
}