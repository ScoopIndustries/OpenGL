#ifdef _WIN32
	#include <windows.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include<SDL.h>
#include "imgui.h"
#include "../Header/stb_image.h"
#include "../Header/camera.hpp"
#include "../Header/Buffer.hpp"
#include "../Header/shader.hpp"
#include "../Header/Shape.hpp"

using namespace GC_3D;

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

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
	auto startTime = Clock::now();

    //Controls

    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 Vdirection = vec3(0.0f, 0.0f, 0.0f);
    vec3 Vright = vec3(0.0f, 0.0f, 0.0f);
    vec3 Vup = vec3(0.0f, 0.0f, 0.0f);
    // horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // vertical angle : 0, look at the horizon
    float verticalAngle = 0.0f;
    // Initial Field of View
    float initialFoV = 45.0f;
    float speed = 20.0f; // 3 units / second
    float mouseSpeed = 0.5f;


	//Initialization GLEW
	glewInit();
    Shape shape;
    Buffer m_Buffer;
    Geometry MakeSphere;
    MakeSphere = MakeSphere.MakeSphere(1.0f);
    for (int i = 0; i < MakeSphere.m_Pos.size(); i++)
    {
        std::cout << MakeSphere.m_Pos[i].x << ' ' << MakeSphere.m_Pos[i].y << ' ' << MakeSphere.m_Pos[i].z << std::endl;   // problem line
    }
	Shader ourShader = Shader("D:/WKLEIN/OpenGL/Shader/SimpleVertexShader.vertexshader", "D:/WKLEIN/OpenGL/Shader/SimpleFragmentShader.fragmentshader");
	//Shader ourShader = Shader("D:/ProjetOPENGL/OpenGL/Shader/SimpleVertexShader.vertexshader", "D:/ProjetOPENGL/OpenGL/Shader/SimpleFragmentShader.fragmentshader");

    glEnable(GL_DEPTH_TEST);

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Buffer
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    m_Buffer.CreateBuffer(CubeVertices, sizeof(CubeVertices));
    m_Buffer.BindBufferToAttrib(0, 3, 5 * sizeof(float), 0);
    m_Buffer.BindBufferToAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float)));

    //Buffer posBuff, normBuff, uvBuff;
    //
    //posBuff.CreateBuffer((float const*)MakeSphere.m_Pos.data(), MakeSphere.m_Pos.size()* sizeof(vec3));
    //normBuff.CreateBuffer((float const*)MakeSphere.m_Normals.data(), MakeSphere.m_Normals.size() * sizeof(vec3));
    //uvBuff.CreateBuffer((float const*)MakeSphere.m_TexCoord.data(), MakeSphere.m_TexCoord.size() * sizeof(vec3));
    //
    //posBuff.BindBufferToAttrib(0, 3, 3 * sizeof(float), 0);
    //uvBuff.BindBufferToAttrib(1, 2, 2 * sizeof(float), 0);
    //
    //uint32_t IBO;
    //glGenBuffers(1, &IBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, MakeSphere.m_Indices.size(), MakeSphere.m_Indices.data(), GL_STATIC_DRAW);

    //MakeSphere.Bind();
    //MakeSphere.Draw();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // load and create a texture -- Texture
        // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* data = stbi_load("D:/ProjetOPENGL/OpenGL/Sprite/container.jpg", &width, &height, &nrChannels, 0);
    unsigned char* data = stbi_load("D:/WKLEIN/OpenGL/Sprite/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("D:/WKLEIN/OpenGL/Sprite/awesomeface.png", &width, &height, &nrChannels, 0);
    //data = stbi_load("D:/ProjetOPENGL/OpenGL/Sprite/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto prevTime = startTime;
	bool appRunning = true;
    //

	while (appRunning)
	{

        auto curTime = Clock::now();
        Duration ftime = prevTime - curTime;
        prevTime = curTime;

		SDL_Event curEvent;
		while (SDL_PollEvent(&curEvent))
		{
            int x, y;
            Uint32 mouseState;
            mouseState = SDL_GetMouseState(&x, &y);
            SDL_WarpMouseInWindow(win, 1024 / 2, 768 / 2);

            Vdirection = vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
            );
            Vright = vec3(
                sin(horizontalAngle - 3.14f / 2.0f),
                0,
                cos(horizontalAngle - 3.14f / 2.0f)
            );
            Vup = cross(Vright, Vdirection);
            // Compute new orientation
            horizontalAngle += mouseSpeed * Seconds(ftime) * float(1024 / 2 - x);
            verticalAngle += mouseSpeed * Seconds(ftime) * float(768 / 2 - y);

			if (curEvent.type == SDL_QUIT)
			{
				SDL_Quit();
			}
			else if (curEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_Quit();
			}else if (curEvent.key.keysym.sym == SDLK_DOWN)
			{
                position += Vdirection * Seconds(ftime) * speed;
			}else if (curEvent.key.keysym.sym == SDLK_UP)
			{
                position -= Vdirection * Seconds(ftime) * speed;
            }else if (curEvent.key.keysym.sym == SDLK_LEFT)
			{
                position += Vright * Seconds(ftime) * speed;
			}else if (curEvent.key.keysym.sym == SDLK_RIGHT)
			{
                position -= Vright * Seconds(ftime) * speed;
			}
		}

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
        // create transformations
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = lookAt(
            position,
            position + Vdirection,
            Vup
        );
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        m_Buffer.BindBufferToAttrib(0, 3, 5 * sizeof(float), 0);
        m_Buffer.BindBufferToAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float)));

        // render boxes
        for (unsigned int i = 0; i < 10; i++)
        {
            shape.DrawCube(cubePositions[i], true, 50.0f, i, ourShader);
        }

        //posBuff.BindBufferToAttrib(0, 3, 3 * sizeof(float), 0);
        //uvBuff.BindBufferToAttrib(1, 2, 2 * sizeof(float), 0);
        //
        //
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        //
        ////MakeSphere.Bind();
        //MakeSphere.Draw();
        //
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(win);
	}
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

	return 0;
}