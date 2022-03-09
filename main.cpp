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
#include <SDL.h>
#include <gl/GLU.h>

#include "gc_3d_defs.hpp"
using namespace std;

/// <summary>
/// Camera Free / Rotation around Object
/// </summary>


	class APP {
	public:
		const float cameraSpeed = .5f;
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

		float camX;
		float camZ;

		APP()
		{
			cameraPosition = glm::vec3(0.0f, 0.0f, 0.3f);
			cameraTarget = glm::vec3(0.0f, 0.0f, -2.0f);
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
				case SDLK_a:
					camX = glm::sin(resultCloak) * cameraSpeed;
					camZ = glm::cos(resultCloak) * cameraSpeed;
					cameraPosition = glm::vec3(camX, -1.0, camZ);
					break;
				case SDLK_q:
					cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;
				case SDLK_e:
					camX = glm::sin(resultCloak) * cameraSpeed;
					camZ = glm::cos(resultCloak) * cameraSpeed;
					cameraPosition = glm::vec3(camX, 1.0, camZ);
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

	void DrawCube(float size) {
		glBegin(GL_QUADS);
		//Front
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(size / 2, size / 2, size / 2);
		glVertex3f(-size / 2, size / 2, size / 2);
		glVertex3f(-size / 2, -size / 2, size / 2);
		glVertex3f(size / 2, -size / 2, size / 2);
		//LEFT
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-size / 2, size / 2, size / 2);
		glVertex3f(-size / 2, size / 2, -size / 2);
		glVertex3f(-size / 2, -size / 2, -size / 2);
		glVertex3f(-size / 2, -size / 2, size / 2);
		//Back
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(size / 2, size / 2, -size / 2);
		glVertex3f(-size / 2, size / 2, -size / 2);
		glVertex3f(-size / 2, -size / 2, -size / 2);
		glVertex3f(size / 2, -size / 2, -size / 2);
		//Right
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(size / 2, size / 2, -size / 2);
		glVertex3f(-size / 2, size / 2, -size / 2);
		glVertex3f(-size / 2, -size / 2, -size / 2);
		glVertex3f(size / 2, -size / 2, -size / 2);
		//Top
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(size / 2, size / 2, size / 2);
		glVertex3f(-size / 2, size / 2, size / 2);
		glVertex3f(-size / 2, size / 2, -size / 2);
		glVertex3f(size / 2, size / 2, -size / 2);
		//Bottom
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(size / 2, -size / 2, size / 2);
		glVertex3f(-size / 2, -size / 2, size / 2);
		glVertex3f(-size / 2, -size / 2, -size / 2);
		glVertex3f(size / 2, -size / 2, -size / 2);
		glEnd();
	}
	void Display() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glLoadIdentity();
		glTranslatef(0.0, 0.0, -5.0);
		DrawCube(1.0);

	}

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
			view = glm::lookAt(app.cameraPosition, app.cameraTarget, glm::vec3(0.0, 1.0, 0.0));
			glm::mat4 projMatrix = glm::perspective(50.0f, 1024.f / 768.f, 1.0f, 1000.f);
			

			glClearColor(0.0, 0.0, 0.0, 1.0);
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(projMatrix));
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(view));
			glEnable(GL_DEPTH_TEST);
			Display();

			glDepthMask(GL_TRUE);
			
			glViewport(0, 0, 1024, 768);
			SDL_GL_SwapWindow(window);

			
		}
		return 0;
	}
