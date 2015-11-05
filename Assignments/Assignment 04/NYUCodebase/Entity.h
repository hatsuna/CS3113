#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "SheetSprite.h"

// 60 FPS (1.0f / 60.0f)
#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6

enum EntityTypes {TYPE_PLAYER, TYPE_ENEMY, TYPE_BULLET};

class Entity {
	public:
		Entity();
	//	Entity(const char *image_path);
		
		void Draw();
		void DrawSprite();
		void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY);
		void Animate();
		float lerp(float v0, float v1, float t);
		
		void Update(float elapsed, std::vector<Entity> entities);
		void Render();
		
		//WIP assignment 04 Animation variables
		/*
		const int runAnimation[5] = { 9, 10, 11, 12, 13 };
		const int numFrames = 5;
		float animationElapsed = 0.0f;
		float framesPerSecond = 30.0f;
		int currentIndex = 0;
		*/

		int entityType;

		SheetSprite sprite; // use if sprite is on a sprite sheet
		GLuint textureID; // use if sprite file is just one sprite

		//one shaderprogram for one app, pass from app to entity
		ShaderProgram * program;
		Matrix modelMatrix;

		float timeLeftOver;
		bool visible;

		float rotation;
		float width;
		float height;
		float x;
		float y;
		float velocity_x;
		float velocity_y;
		float acceleration_x;
		float acceleration_y;
		float friction_x;
		float friction_y;

		//depreciated
		float speed;
		float direction_x;
		float direction_y;


		//Assignment 04 WIP Additions, need to implement
		/*
		void Update(float elapsed);
		void Render(ShaderProgram * program); // pass shaderprogrbuam
		bool collidesWith(Entity * entity);
		
		Sheetsprite sprite;
		
		float velocity_x;
		float velocity_y;

		float acceleration_x;
		float acceleration_y;
		
		bool isStatic;
		*/
};

class Bullet : public Entity{
	public:
		Bullet();
		void erase();

		float timeAlive;
		
		//inherited from entity
		//float x;
		//float y;
		//float speed;
};