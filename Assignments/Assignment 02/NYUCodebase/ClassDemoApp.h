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

class ClassDemoApp {
	public:
		ClassDemoApp(); 
		~ClassDemoApp();
	
		void Setup();
		void ProcessEvents(); 
		bool UpdateAndRender();
	
		void Render(); 
		void Update(float elapsed);

		/*incorporate into Entity class
		GLuint LoadTexture(const char *image_path);
		void DrawSprite(GLint texture, float x, float y, float rotation);
		*/

	private:
		bool done;
		SDL_Event event;
		float lastFrameTicks; 
		SDL_Window* displayWindow;

		SDL_Joystick* joystick;
		SDL_GLContext context;

		/* in entity code
		Matrix projectionMatrix;
		Matrix modelMatrix;
		Matrix viewMatrix;
		
		ShaderProgram* program;
		*/

		Entity ship;
		Entity asteroid;
		Entity ufo;

		float xDir;
		float yDir;

		float positionX;
		float positionY;
		float angle;

};