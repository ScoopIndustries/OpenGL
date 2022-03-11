#ifdef _WIN32
	#include <windows.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include<SDL.h>

#include "../Header/stb_image.h"
#include "../Header/camera.hpp"
#include "../Header/Buffer.hpp"
#include "../Header/shader.hpp"

using namespace GC_3D;

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

        //Initialization GLEW
        glewInit();

		Geometry geo;
        Camera_Data camera;
        
        GLuint programID = LoadShaders("D:/Wklein/OpenGL/Shader/SimpleVertexShader.vertexshader", "D:/Wklein/OpenGL/Shader/SimpleFragmentShader.fragmentshader");

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);

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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(programID);

            //View
            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
            glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

            // Or, for an ortho camera :
            //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

            // Camera matrix
            glm::mat4 View = glm::lookAt(
                glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
                glm::vec3(0, 0, 0), // and looks at the origin
                glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
            );

            

            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::vec3 rotationAxe(0.0f, 1.0f, 0.0f);
            glm::mat4 rotator = glm::rotate(0.01f, rotationAxe);
            Model = rotator *  Model;
            glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

            // Get a handle for our "MVP" uniform
            // Only during the initialisation
            GLuint MatrixID = glGetUniformLocation(programID, "MVP");

            // Send our transformation to the currently bound shader, in the "MVP" uniform
            // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

            // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
            // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
            static const GLfloat gVertexBufferData[] = {
                -1.0f,-1.0f,-1.0f, // triangle 1 : begin
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f, // triangle 1 : end
                1.0f, 1.0f,-1.0f, // triangle 2 : begin
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f, // triangle 2 : end
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f
            };

            // One color for each vertex. They were generated randomly.
            static const GLfloat gColorBufferData[] = {
                0.583f,  0.771f,  0.014f,
                0.609f,  0.115f,  0.436f,
                0.327f,  0.483f,  0.844f,
                0.822f,  0.569f,  0.201f,
                0.435f,  0.602f,  0.223f,
                0.310f,  0.747f,  0.185f,
                0.597f,  0.770f,  0.761f,
                0.559f,  0.436f,  0.730f,
                0.359f,  0.583f,  0.152f,
                0.483f,  0.596f,  0.789f,
                0.559f,  0.861f,  0.639f,
                0.195f,  0.548f,  0.859f,
                0.014f,  0.184f,  0.576f,
                0.771f,  0.328f,  0.970f,
                0.406f,  0.615f,  0.116f,
                0.676f,  0.977f,  0.133f,
                0.971f,  0.572f,  0.833f,
                0.140f,  0.616f,  0.489f,
                0.997f,  0.513f,  0.064f,
                0.945f,  0.719f,  0.592f,
                0.543f,  0.021f,  0.978f,
                0.279f,  0.317f,  0.505f,
                0.167f,  0.620f,  0.077f,
                0.347f,  0.857f,  0.137f,
                0.055f,  0.953f,  0.042f,
                0.714f,  0.505f,  0.345f,
                0.783f,  0.290f,  0.734f,
                0.722f,  0.645f,  0.174f,
                0.302f,  0.455f,  0.848f,
                0.225f,  0.587f,  0.040f,
                0.517f,  0.713f,  0.338f,
                0.053f,  0.959f,  0.120f,
                0.393f,  0.621f,  0.362f,
                0.673f,  0.211f,  0.457f,
                0.820f,  0.883f,  0.371f,
                0.982f,  0.099f,  0.879f
            };

            // Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
            static const GLfloat gUvBufferData[] = {
                0.000059f, 1.0f - 0.000004f,
                0.000103f, 1.0f - 0.336048f,
                0.335973f, 1.0f - 0.335903f,
                1.000023f, 1.0f - 0.000013f,
                0.667979f, 1.0f - 0.335851f,
                0.999958f, 1.0f - 0.336064f,
                0.667979f, 1.0f - 0.335851f,
                0.336024f, 1.0f - 0.671877f,
                0.667969f, 1.0f - 0.671889f,
                1.000023f, 1.0f - 0.000013f,
                0.668104f, 1.0f - 0.000013f,
                0.667979f, 1.0f - 0.335851f,
                0.000059f, 1.0f - 0.000004f,
                0.335973f, 1.0f - 0.335903f,
                0.336098f, 1.0f - 0.000071f,
                0.667979f, 1.0f - 0.335851f,
                0.335973f, 1.0f - 0.335903f,
                0.336024f, 1.0f - 0.671877f,
                1.000004f, 1.0f - 0.671847f,
                0.999958f, 1.0f - 0.336064f,
                0.667979f, 1.0f - 0.335851f,
                0.668104f, 1.0f - 0.000013f,
                0.335973f, 1.0f - 0.335903f,
                0.667979f, 1.0f - 0.335851f,
                0.335973f, 1.0f - 0.335903f,
                0.668104f, 1.0f - 0.000013f,
                0.336098f, 1.0f - 0.000071f,
                0.000103f, 1.0f - 0.336048f,
                0.000004f, 1.0f - 0.671870f,
                0.336024f, 1.0f - 0.671877f,
                0.000103f, 1.0f - 0.336048f,
                0.336024f, 1.0f - 0.671877f,
                0.335973f, 1.0f - 0.335903f,
                0.667969f, 1.0f - 0.671889f,
                1.000004f, 1.0f - 0.671847f,
                0.667979f, 1.0f - 0.335851f
            };

            //VAO
            // This will identify our vertex buffer
            //GLuint vertexBuffer{};
            //GLuint colorbuffer{};
            // Generate 1 buffer, put the resulting identifier in vertexbuffer

           //Load Image
            int width, height, nrChannels;
            unsigned char* data = stbi_load("D:/Wklein/OpenGL/Sprite/uvtemplate.bmp", &width, &height, &nrChannels, 0);
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);

            Buffer *A = new Buffer();
            A->CreateBuffer(gVertexBufferData, sizeof(gVertexBufferData));
            A->BindBuffer(0);

            Buffer* B = new Buffer();
            B->CreateBuffer(gUvBufferData, sizeof(gUvBufferData));
            B->BindBuffer(1);

            
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glDisableVertexAttribArray(2);
            
            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LESS);

/*
			glViewport(0, 0, 1024, 768);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

            */
			SDL_GL_SwapWindow(win);
		}
		return 0;
	}