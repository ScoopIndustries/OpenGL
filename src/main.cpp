#include<cstdio>
#ifdef _WIN32
	#include <windows.h>
#endif

#include<SDL.h>
#include<SDL_opengl.h>
#include<gl/GL.h>
#include"../Header/gc_3d_defs.hpp"

using namespace GC_3D;

class Controls
{
    Controls() {};
    

};
	int main(int argc, char* argv[])
	{
		SDL_Init(SDL_INIT_VIDEO);

		uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		

		SDL_Window* win = SDL_CreateWindow("Moteur",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1024,
			768,
			windowFlags);

		SDL_GLContext context = SDL_GL_CreateContext(win);
		SDL_GL_MakeCurrent(win, context);

		auto prevTime = Clock::now();

		Geometry geo;

		bool appRunning = true;
		while (appRunning)
		{
			SDL_Event curEvent;
			while (SDL_PollEvent(&curEvent))
			{
                if (curEvent.type == SDL_QUIT)
                {
                    SDL_Quit();
                }
                else if (curEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    SDL_Quit();
                }
			}

			glViewport(0, 0, 1024, 768);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);



			const float radius = 0.5;

			auto curTime = Clock::now();
			std::chrono::duration<float> fTime = curTime - prevTime;
			float camX = sin(fTime.count()) * radius;
			float camZ = cos(fTime.count()) * radius;

			vec3 cameraTarget = vec3(0.0, 0.0, -2.0);

			vec3 cameraPos = cameraTarget - 6.0f * vec3(camX, 0.2, camZ);

			// Creation de la camera
			mat4 view;
			view = lookAt(cameraPos, //Position de la camera
				cameraTarget, //Cible à regarder
				vec3(0.0, 1.0, 0.0)); //position vertical
			mat4 camFrustum = frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&camFrustum[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&view[0][0]);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glTranslatef(0.0, 0.0, -2.0);

            //GLfloat ambientLightFull[] = { 0.0f, 1.0f, 1.0f, 0.0f };
            //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightFull);
            //float red[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
            
            geo.m_Pos = {
                //Front
                vec3(-1.0, 1.0, 1.0),
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, -1.0, 1.0),
                vec3(-1.0, -1.0, 1.0),
                //Back
                vec3(-1.0, 1.0, -1.0),
                vec3(1.0, 1.0, -1.0),
                vec3(1.0, -1.0, -1.0),
                vec3(-1.0, -1.0, -1.0),
                //Left
                vec3(-1.0, 1.0, 1.0),
                vec3(-1.0, 1.0, -1.0),
                vec3(-1.0, -1.0, -1.0),
                vec3(-1.0, -1.0, 1.0),
                //Right
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, 1.0, -1.0),
                vec3(1.0, -1.0, -1.0),
                vec3(1.0, -1.0, 1.0),
                //Up
                vec3(-1.0, 1.0, 1.0),
                vec3(-1.0, 1.0, -1.0),
                vec3(1.0, 1.0, -1.0),
                vec3(1.0, 1.0, 1.0),
                //Back
                vec3(-1.0, -1.0, 1.0),
                vec3(-1.0, -1.0, -1.0),
                vec3(1.0, -1.0, -1.0),
                vec3(1.0, -1.0, 1.0),
            };

            geo.m_Normals = {
                //Front
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 0.0, 1.0),
                //Back
                vec3(0.0, 0.0, -1.0),
                vec3(0.0, 0.0, -1.0),
                vec3(0.0, 0.0, -1.0),
                vec3(0.0, 0.0, -1.0),
                //Left
                vec3(-1.0, 0.0, 0.0),
                vec3(-1.0, 0.0, 0.0),
                vec3(-1.0, 0.0, 0.0),
                vec3(-1.0, 0.0, 0.0),
                //Right
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 0.0, 0.0),
                //Up
                vec3(0.0, 1.0, 0.0),
                vec3(0.0, 1.0, 0.0),
                vec3(0.0, 1.0, 0.0),
                vec3(0.0, 1.0, 0.0),
                //Back
                vec3(0.0, -1.0, 0.0),
                vec3(0.0, -1.0, 0.0),
                vec3(0.0, -1.0, 0.0),
                vec3(0.0, -1.0, 0.0),
            };

            geo.m_Indices = {
            0, 1, 2, 0, 2, 3, //Front
            4, 5, 6, 4, 6, 7, //Back
            8, 9, 10, 8, 10, 11, //Left
            12, 13, 14, 12, 14, 15, //Right
            16, 17, 18, 16, 18, 19, //Up
            20, 21, 22, 20, 22, 23 //Down
            };

            
			geo.Bind();
			geo.Draw();


			SDL_GL_SwapWindow(win);
		}

		printf("\nHello World !\n");
		return 0;
	}