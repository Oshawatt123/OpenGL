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

#include "Object.h"
#include "Camera.h"
#include "Vertex.h"
#include "Shader.h"
#include "LightBase.h"

#include "Time.h"

/*void DrawThing(Mesh& thingToDraw, Shader& program, LightBase& light, GLuint texID, GLuint normalID)//, vec2 screenPos, vec2 dimensions, vec4 colour)
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
}*/

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

	// make a camera

	Camera cam = Camera(100, 1, 0.1, 100);
	vec3 camPos = { 0, 0, -1 };
	cam.m_transform.setPos(camPos);
	//cam.m_transform.setRotation(vec3(0, 0, 0));

	// create shader program

	Shader* phongShader = new Shader("../SDL_GL_Assignment2/simplevertex.vert", "../SDL_GL_Assignment2/texturedFrag.frag", cam);
	Shader* morphgrid = new Shader("../SDL_GL_Assignment2/basicvert.vert", "../SDL_GL_Assignment2/morphgrid.frag", cam);
	Shader* waterShaderMaybe = new Shader("../SDL_GL_Assignment2/watermaybe.vert", "../SDL_GL_Assignment2/limefrag.frag", cam);


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


	std::vector<Object*> sceneObject;

	Object* myObj = new Object("Not Barry");

	bool playing = true;

	LightBase* light = new LightBase();

	bool lightMovingLeft = false;
	
	bool paused = false;

	while (playing)
	{

		//std::cout << (float)Time::Instance()->GetTimeSinceStart() << std::endl;

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
					//sceneObject.push_back(new Mesh(&squareVertices[0], squareVertices.size(), &SquareIndices[0], 6, cam.m_transform.getPos() + cam.m_target));
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

			cam.Translate(cam.getRightVector() * 0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_D])
		{
			cam.Translate(cam.getRightVector() * -0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_W])
		{
			cam.Translate(cam.getForwardVector() * 0.1f);
		}
		if (keyboardStates[SDL_SCANCODE_S])
		{
			cam.Translate(cam.getForwardVector() * -0.1f);
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
			/*for (int i = 0; i < sceneObject.size(); i++)
			{
				DrawThing(*sceneObject[i], *phongShader, *light, diffuseTex, normalTex);
			}*/

			//std::cout << "X: " << light->getTransform()->getPos().x << " Y: " << light->getTransform()->getPos().y << " Z: " << light->getTransform()->getPos().z << std::endl;

			myObj->Draw(*morphgrid, *light);

			// swap buffers
			SDL_GL_SwapWindow(window);
			 
			if (light->getTransform()->getPos().x >= 1)
			{
				lightMovingLeft = false;
			}
			if (light->getTransform()->getPos().x <= -5)
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