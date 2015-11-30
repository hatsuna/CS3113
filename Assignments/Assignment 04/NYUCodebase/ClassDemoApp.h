#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"


#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

// 60 FPS (1.0f / 60.0f)
#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22

#define SPRITE_COUNT_X 13
#define SPRITE_COUNT_Y 13

#define TILE_SIZE 0.5f

enum GameState { STATE_TITLE_SCREEN, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp {
	public:
		ClassDemoApp(); 
		~ClassDemoApp ();
	
		GLuint LoadTexture(const char *image_path);
		void SpawnPlayer();
		void SpawnEnemies();
		void Setup();
		void ProcessEvents(); 
		bool UpdateAndRender();
		void shootBullet();
		bool shouldRemoveBullet(Bullet bullet);
		void DrawText(int fontTexture, std::string text, float size, float spacing);
		void buildLevel();
		bool isSolid(int x, int y);
		void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
		void worldCollisions(Entity * entity);

		void titleScreenRender();
		void titleScreenUpdate();
		void gameRender();
		void gameUpdate(float elapsed);
		void gameOverRender();
		void gameOverUpdate();

		void Render(); 
		void Update(float elapsed);

	private:
		int state;
		int score;
		float timeLeftOver;

		bool done;
		SDL_Event event;
		float lastFrameTicks;
		SDL_Window* displayWindow;

		SDL_Joystick* joystick;
		SDL_GLContext context;

		ShaderProgram * program; //one shaderprogram for one app, pass from app to entity
		
		Matrix modelMatrix;
		Matrix projectionMatrix;
		Matrix viewMatrix;

		GLuint textureID;
		GLuint LevelTexture;

		int numEnemies;

		std::vector<Entity> entities;
		std::vector<Bullet> bullets;

		unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
				
		float xDir;
		float yDir;

		float positionX;
		float positionY;
		float angle;

};
