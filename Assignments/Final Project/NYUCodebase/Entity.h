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

enum EntityType {TYPE_PLAYER, TYPE_ENEMY, TYPE_BULLET, TYPE_LEVEL};

class Entity {
	public:
		Entity();

		void Update(float elapsed, std::vector<Entity> entities);
		void Render(ShaderProgram * program);
		bool collidesWith(Entity * entity);

		EntityType entityType;
		SheetSprite sprite; // use if sprite is on a sprite sheet

		Matrix modelMatrix;

		bool visible;
		// Dynamic - gravity applied and collision with other entities checked
		// Static - no gravity, no movement, no collision checking
		bool isStatic;
		int playerNum;

		float width;
		float height;
		float x;
		float y;
		float rotation;
		float velocity_x;
		float velocity_y;
		float velocity_rotation;
		float acceleration_x;
		float acceleration_y;
		float acceleration_rotation;
		float friction_x;
		float friction_y;
		float friction_rotation;
		float gravity_x;
		float gravity_y;

		// Contact Flags - set based on collision direction
		// otherwise, set false during every frame
		bool collidedTop;
		bool collidedBottom;
		bool collidedLeft;
		bool collidedRight;

};

class Bullet : public Entity{
	public:
		Bullet();
		void erase();

		float timeAlive;
		int playerHit;
};