#define GLEW_STATIC

const float SCREEN_WIDTH = 1080;
const float SCREEN_HEIGHT = 720;

#include <glew.h>
#include <SDL_opengl.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include <iostream>
#include <vector>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Mesh.h"
#include "Camera.h"
#include "Vertex.h"
#include "Shader.h"
#include "LightBase.h"

void DrawThing(Mesh& thingToDraw, Shader& program, LightBase& light, GLuint texID, GLuint normalID)//, vec2 screenPos, vec2 dimensions, vec4 colour)
{
	program.Bind();

	GLuint textureLoc;

	glActiveTexture(GL_TEXTURE0);
	textureLoc = glGetUniformLocation(program.getProgram(), "texture_diffuse");
	glUniform1i(textureLoc, 0); // 0 for location 0
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE1);
	textureLoc = glGetUniformLocation(program.getProgram(), "texture_normal");
	glUniform1i(textureLoc, 1); // 0 for location 0
	glBindTexture(GL_TEXTURE_2D, normalID);

	program.Update(&thingToDraw.m_transform, light);
	thingToDraw.Draw();
}

GLuint LoadTexture(std::string texLocation)
{
	GLuint textureID;

	// yummy hardcoding

	int width, height, numComponents;

	unsigned char* ImageData = stbi_load(texLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	if (ImageData == NULL)
	{
		std::cerr << "Houston, the servers are streaming some unsettling images from " << texLocation << "\n";
	}

	GLenum format;
	if (numComponents == 1)
		format = GL_RED;
	else if (numComponents == 3)
		format = GL_RGB;
	else if (numComponents == 4)
		format = GL_RGBA;

	glGenTextures(1, &textureID); // generate our buffer
	glBindTexture(GL_TEXTURE_2D, textureID); // bind our texture ID to the buffer for use

	// tell openGL how it should handle texture overlap and underlap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // S and T are U and V
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // openGL is just special
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);

	if (glTexImage2D == NULL)
	{
		std::cout << "Texture not loaded by openGL!" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(ImageData);

	return textureID;
}

int main(int argc, char* agrv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	// set the size of the RGBA pixel value buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 32 bit RGBA
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // enables double buffering
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// ignore this jazz for now
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	// create the window
	SDL_Window* window = SDL_CreateWindow("Filthy Engine Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1920, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// create an OpenGL context so we can draw
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, GLContext);

	// enable glew experimental bc we're edgey and init glew
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	// more edgey stuff
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// check that lil glew is alright
	if (status != GLEW_OK)
	{
		std::cout << "Lil GLEW is not alright" << std::endl;
	}

	GLuint diffuseTex = LoadTexture("../SDL_GL_Assignment2/brickwall.jpg");
	GLuint normalTex = LoadTexture("../SDL_GL_Assignment2/brickwall_normal.jpg");

	// make a camera

	Camera cam = Camera(100, 1, 0.1, 100);
	vec3 camPos = { 0, 0, -10 };
	cam.m_transform.setPos(camPos);

	// make a square

	std::vector<vertex> squareVertices;
	squareVertices.push_back(vertex(vec3(-0.5f, 0.5f, 1.0f),	vec2(0, 0))); // top left
	squareVertices.push_back(vertex(vec3(0.5f, 0.5f, 1.0f),	vec2(1, 0))); // top right
	squareVertices.push_back(vertex(vec3(0.5f, -0.5f, 1.0f),	vec2(1, 1))); // bottom right
	squareVertices.push_back(vertex(vec3(-0.5f, -0.5f, 1.0f),	vec2(0, 1))); // bottom left

	unsigned int SquareIndices[]
	{
		0,1,2,0,2,3
	};

	std::vector<vertex> cubeVertices;
	cubeVertices.push_back(vertex(vec3(-0.5f, 0.5f, 1.0f), vec2(0, 0))); // top left
	cubeVertices.push_back(vertex(vec3(0.5f, 0.5f, 1.0f), vec2(1, 0))); // top right
	cubeVertices.push_back(vertex(vec3(0.5f, -0.5f, 1.0f), vec2(1, 1))); // bottom right
	cubeVertices.push_back(vertex(vec3(-0.5f, -0.5f, 1.0f), vec2(0, 1))); // bottom left
	cubeVertices.push_back(vertex(vec3(-0.5f, 0.5f, 2.0f), vec2(0, 0))); // top left
	cubeVertices.push_back(vertex(vec3(0.5f, 0.5f, 2.0f), vec2(1, 0))); // top right
	cubeVertices.push_back(vertex(vec3(0.5f, -0.5f, 2.0f), vec2(1, 1))); // bottom right
	cubeVertices.push_back(vertex(vec3(-0.5f, -0.5f, 2.0f), vec2(0, 1))); // bottom left

	unsigned int CubeIndices[]
	{
		0,1,2,0,2,3, // front
		4,0,3,4,3,7, // left
		5,7,4,5,6,7, // back
		2,1,5,2,1,6, // right
		0,5,4,0,7,5, // top
		3,6,7,3,2,6  // bottom
	};

	// create shader program

	Shader* phongShader = new Shader("../SDL_GL_Assignment2/simplevertex.vert", "../SDL_GL_Assignment2/texturedFrag.frag", cam);


	float R;
	float G;
	float B;
	float A;

	float zRot = 0.0f;

	float xPos = 0.0f;
	bool movingRight = true;

	const Uint8* keyboardStates;

	bool RMB = false;

	float mouseDeltaX = 0;
	float mouseDeltaY = 0;

	vec3 worldForward = { 0, 0, 1.0f };
	vec3 worldRight = { 1.0f, 0, 0 };
	vec3 worldUp = { 0, 1.0f, 0 };


	std::vector<Mesh*> sceneObject;

	Mesh square1(&squareVertices[0], squareVertices.size(), &SquareIndices[0], 6);
	sceneObject.push_back(&square1);

	Mesh Cube1(&cubeVertices[0], cubeVertices.size(), &CubeIndices[0], 36);
	//sceneObject.push_back(&Cube1);
	//Cube1.m_transform.Translate(-3, 0, 0);

	bool playing = true;

	LightBase* light = new LightBase();

	bool lightMovingLeft = false;
	
	bool paused = false;

	while (playing)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				playing = false;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					RMB = true;
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					RMB = false;
				}
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				if (RMB)
				{
					mouseDeltaX = e.motion.xrel;
					mouseDeltaY = e.motion.yrel;

					cam.targetDeltaX = -mouseDeltaX;
					cam.targetDeltaY = -mouseDeltaY;
				}
			}

			if (e.type == SDL_KEYUP)
			{
				if (e.key.keysym.scancode == SDL_SCANCODE_O)
				{
					sceneObject.push_back(new Mesh(&squareVertices[0], squareVertices.size(), &SquareIndices[0], 6, cam.m_transform.getPos() + cam.m_target));
					std::cout << "fsbnjfbnjif" << std::endl;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_R)
				{
					cam.m_target = { 1,1,1 };
				}
			}
		}

		keyboardStates = SDL_GetKeyboardState(NULL);

		if (keyboardStates[SDL_SCANCODE_A])
		{

			cam.m_transform.Translate(cam.getRightVector() * 0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_D])
		{
			cam.m_transform.Translate(cam.getRightVector() * -0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_W])
		{
			cam.m_transform.Translate(cam.getForwardVector() * 0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_S])
		{
			cam.m_transform.Translate(cam.getForwardVector() * -0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_P])
		{
			paused = true;
		}
		else
			paused = false;
		if (!paused)
		{
			glViewport(0, 0, 1920, 1080);
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




			// move the light
			if (lightMovingLeft)
				light->getTransform()->Translate(0.01, 0, 0);
			else
				light->getTransform()->Translate(-0.01, 0, 0);

			// draw the light gizmo
			light->Draw(&cam);

			// draw my scene objects
			for (int i = 0; i < sceneObject.size(); i++)
			{
				DrawThing(*sceneObject[i], *phongShader, *light, diffuseTex, normalTex);
			}

			std::cout << "X: " << light->getTransform()->getPos().x << " Y: " << light->getTransform()->getPos().y << " Z: " << light->getTransform()->getPos().z << std::endl;


			// swap buffers
			SDL_GL_SwapWindow(window);

			//system("CLS");

			if (light->getTransform()->getPos().x >= 1)
			{
				lightMovingLeft = false;
			}
			if (light->getTransform()->getPos().x <= -1)
			{
				lightMovingLeft = true;
			}
		}
		
	}

	for (auto object : sceneObject)
	{
		object = nullptr;
		delete(object);
	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return 0;
}