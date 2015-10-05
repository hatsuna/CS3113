#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class Entity {
	public:
		Entity();
		Entity(const char *image_path);

		void Draw();
		void DrawSprite();

		ShaderProgram * program;
		Matrix projectionMatrix;
		Matrix modelMatrix;
		Matrix viewMatrix;

		float x;
		float y;

		float rotation;

		GLuint textureID;

		float width;
		float height;

		float speed;
		float direction_x;
		float direction_y;
};