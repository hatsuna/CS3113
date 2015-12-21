#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <string> 
#include <iostream> 
#include <sstream>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"
#include "PerlinNoise.h"
#include "ParticleEmitter.h"


#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define WINDOW_WIDTH 1280 //640
#define WINDOW_HEIGHT 720 //360
// 60 FPS (1.0f / 60.0f)
#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22
#define SPRITE_COUNT_X 13
#define SPRITE_COUNT_Y 13
#define TILE_SIZE 0.5f

#define MAXENEMIES 10
#define MAXSTARS 10
#define ROTATIONACCELERATION 40.0f
#define MOVEMENTACCELERATION 0.05f 
#define BULLETSPEED 1.0f
#define BULLETLIFETIME 1.5f


using namespace std;

enum GameState { STATE_TITLE_SCREEN, STATE_GAME_LEVEL, STATE_GAME_OVER };

class MixerSound {
public:
	Uint32 offset;
	Uint32 length;
	Uint8 *buffer;
	float volume;
	SDL_AudioFormat format;
	bool loaded;
	bool playing;
	bool loop;
};

class ClassDemoApp {
	public:
		ClassDemoApp(); 
		~ClassDemoApp ();
	
		GLuint LoadTexture(const char *image_path);
		void SpawnPlayer();
		void SpawnEnemies();
		void SpawnStars();
		void Setup();
		void ProcessEvents(); 
		bool UpdateAndRender();
		void shootBullet(Entity * entity);
		bool shouldRemoveBullet(Bullet bullet);
		void DrawText(int fontTexture, std::string text, float size, float spacing);
		
		int loadSound(const char *soundFile);
	//	void appAudioCallback(void *userdata, Uint8 *stream, int len);
		void playSound(int soundIndex, bool loop);
		bool readHeader(std::ifstream &stream);
		bool readLayerData(std::ifstream &stream);
		bool readEntityData(std::ifstream &stream);
		void loadLevelData();
		void buildLevel();
		bool isSolid(int x, int y);
		void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
		void worldCollisions(Entity * entity);

		void titleScreenRender();
		void titleScreenUpdate(float elapsed);
		void gameRender();
		void gameUpdate(float elapsed);
		void gameOverRender();
		void gameOverUpdate(float elapsed);

		void Render(); 
		void Update(float elapsed);

		std::vector<MixerSound> mixSounds;

	private:
		int state;
		float timeLeftOver;

		float perlinValue;

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

		int LaserSound;
		int music;
		SDL_AudioSpec deviceSpec;

		//simple sound
		Mix_Chunk * laserSound1;
		Mix_Chunk * laserSound2;
		Mix_Chunk * hitSound;
		Mix_Chunk * bgmusic;
		//Mix_Music *bgmusic;

		int mapWidth;
		int mapHeight;

		float animationTime;
		float animationStart = 0;
		float animationEnd = 5;

		int numEnemies;
		int numStars;
		string GameEndBuffer;

		std::vector<Entity> entities;
		std::vector<ParticleEmitter> emitters;
		std::vector<Bullet> bullets;
		

		unsigned char** levelData;
				
		float xDir;
		float yDir;

		float positionX;
		float positionY;
		float angle;

};
