#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
//#include "Sheetsprite.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

// WIP Assignment 
// enum Entity types
// add entity variable

class Entity {
	public:
		Entity();
		Entity(const char *image_path);

		void Draw();
		void DrawSprite();
		void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY);
		void Animate();
		void DrawText(int fontTexture, std::string text, float size, float spacing);

		//WIP assignment 03
		//Animation variables
		const int runAnimation[] = { 9, 10, 11, 12, 13 };
		const int numFrames = 5;
		float animationElapsed = 0.0f;
		float framesPerSecond = 30.0f;
		int currentIndex = 0;

		ShaderProgram * program; //delete this
		//one shaderprogram for one app, pass from app to entity
		
		//these shouldn't be here either?
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

		/*Assignment 04 WIP Additions, need to implement
		void Update(float elapsed);
		void Render(ShaderProgram * program); // pass shaderprogram
		bool collidesWith(Entity * entity);
		
		Sheetsprite sprite;
		
		float velocity_x;
		float velocity_y;

		float acceleration_x;
		float acceleration_y;
		
		bool isStatic;
		*/
};