#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

// 60 FPS (1.0f / 60.0f)
#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6

#define WINDOW_WIDTH = 640;
#define WINDOW_HEIGHT = 480;

class ClassDemoApp {
	public:
		ClassDemoApp(); 
		~ClassDemoApp();
	
		void Setup();
		void ProcessEvents(); 
		bool UpdateAndRender();
	
		void Render(); 
		void Update(float elapsed);
		void FixedUpdate(float elapsed);

	private:
		bool done;
		SDL_Event event;
		float lastFrameTicks;
		float timeLeftOver;
		SDL_Window* displayWindow;

		SDL_Joystick* joystick;
		SDL_GLContext context;

		Entity ship;
		Entity asteroid;
		Entity ufo;

		Entity player1;
		Entity player2;

		float xDir;
		float yDir;

		float positionX;
		float positionY;
		float angle;

};