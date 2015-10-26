#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"


#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define WINDOW_WIDTH = 640;
#define WINDOW_HEIGHT = 480;

class ClassDemoApp {
	public:
		ClassDemoApp(); 
		~ClassDemoApp();
	
		void Setup();
		void ProcessEvents(); 
		bool UpdateAndRender();
		void DrawText(int fontTexture, std::string text, float size, float spacing);
		

		void Render(); 
		void Update(float elapsed);

	private:
		bool done;
		SDL_Event event;
		float lastFrameTicks;
		SDL_Window* displayWindow;

		SDL_Joystick* joystick;
		SDL_GLContext context;

		ShaderProgram * program; //one shaderprogram for one app, pass from app to entity
		
		Matrix projectionMatrix;
		Matrix viewMatrix;

		Entity player1;
		Entity player2;
		Entity ball;

		std::vector<Entity> entities;

		float xDir;
		float yDir;

		float positionX;
		float positionY;
		float angle;

};

/*
Another thing is you should only load the shader program
once and then share it between your entities.
Since your view and your projection matrix are (usually)
not going to vary between entities,
you can just set the model matrix in it and
use the same instance of shader program to draw everything
(pass it around by pointer).
*/