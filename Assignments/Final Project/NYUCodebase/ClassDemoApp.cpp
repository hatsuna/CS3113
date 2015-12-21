#include "ClassDemoApp.h"
/* 
 * Matthew Pon
 * CS3113 Final Project
 */
/*
Controls
Player 1:						Player 2:
[W] to boost					[I] to boost
[A] + [D] to rotate				[J] + [L] to rotate
[Q] or [E] to shoot				[U] or [O] to shoot

[Space] to restart
[Esc] to Quit
*/

ClassDemoApp::ClassDemoApp() {
	Setup();
}
ClassDemoApp::~ClassDemoApp() {
	Mix_FreeChunk(laserSound1);
	Mix_FreeChunk(laserSound2);
	Mix_FreeChunk(hitSound);
	Mix_FreeChunk(bgmusic);
	//Mix_FreeMusic(bgmusic);

	// SDL and and OpenGL cleanup (joysticks, textures, etc.)
	SDL_JoystickClose(joystick);
	SDL_Quit();
}
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin)); if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	} return retVal;
}
GLuint ClassDemoApp::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	//Creates a new OpenGL texture ID
	GLuint textID;
	glGenTextures(1, &textID);
	//Bind a texture to a texture target
	glBindTexture(GL_TEXTURE_2D, textID);
	//Sets the texture data of the specified texture target, RGBA or RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	//Sets a texture parameter of the specified texture target,
	//must be set before texture can be used
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glClamp and glRepeat used to deal with textures on the outside

	//glClamp - non-tiled images with alpha
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//glRepeat - tiled images
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);
	return textID;
}

// When adding a character spritesheet, divide by sheet size
// spriteSheetTexture = LoadTexture("sheet.png");
// player.sprite = SheetSprite(spriteSheetTexture, 67.0f / 512.0f, ... )
// When setting collision, sprite and collision box might not need to be the same
void ClassDemoApp::SpawnPlayer(){
	Entity player1;
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	player1.sprite = SheetSprite(textureID, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.15f);
	player1.rotation = -2.0f;
	// currently settting player dimensions to sprite dimensions,
	// should tweak later
	player1.width = player1.sprite.width; // player1.sprite.size;
	player1.height = player1.sprite.height; // player1.sprite.size;;
	player1.x = -2.0f;
	player1.y = 1.0f;
	player1.visible = true;
	player1.isStatic = false;
	player1.entityType = TYPE_PLAYER;
	player1.playerNum = 1;
	entities.push_back(player1);
	ParticleEmitter p1(10);
	p1.maxLifetime = 5.0f;
	p1.startColor.r = 0.5f; p1.startColor.b = 0.2f; p1.startColor.g = 0.2f; p1.startColor.a = 0.1f;
	emitters.push_back(p1);

	Entity player2;
	//<SubTexture height="84" width="103" y="0" x="222" name="enemyBlue3.png"/>
	player2.sprite = SheetSprite(textureID, 222.0f / 1024.0f, 0.0f, 103.0f / 1024.0f, 84 / 1024.0f, 0.15f);
	player2.rotation = player1.rotation;
	player2.width = player2.sprite.width;
	player2.height = player2.sprite.height;
	player2.x = -player1.x;
	player2.y = -player1.y;
	player2.visible = true;
	player2.isStatic = false;
	player2.entityType = TYPE_PLAYER;
	player2.playerNum = 2;
	entities.push_back(player2);
	ParticleEmitter p2(10);
	p2.maxLifetime = 5.0f;
	emitters.push_back(p2);

	//Camera Center
	//viewMatrix.Translate(-entities[0].x, -entities[0].y, 0);
	//program->setViewMatrix(viewMatrix);
}
void ClassDemoApp::SpawnEnemies(){
	for (int i = 0; i < numEnemies; i++){
		Entity newEnemy;
		int size = rand() % 7 + 1;
		switch (size){
		case(7):
			//<SubTexture height="98" width="120" y="520" x="0" name="meteorBrown_big2.png"/>
			newEnemy.sprite = SheetSprite(textureID, 0.0f / 1024.0f, 520.0f / 1024.0f, 120.0f / 1024.0f, 98.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(6):
			//<SubTexture height = "82" width = "89" y = "728" x = "516" name = "meteorGrey_big3.png" / >
			newEnemy.sprite = SheetSprite(textureID, 516.0f / 1024.0f, 728.0f / 1024.0f, 89.0f / 1024.0f, 82.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(5):
			// <SubTexture height="96" width="98" y="452" x="327" name="meteorBrown_big4.png"/>
			newEnemy.sprite = SheetSprite(textureID, 327.0f / 1024.0f, 452.0f / 1024.0f, 98.0f / 1024.0f, 96.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(4):
			// <SubTexture height="84" width="101" y="748" x="224" name="meteorGrey_big1.png"/>
			newEnemy.sprite = SheetSprite(textureID, 224.0f / 1024.0f, 748.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(3):
			//<SubTexture height="40" width="45" y="452" x="237" name="meteorBrown_med3.png"/>
			newEnemy.sprite = SheetSprite(textureID, 237.0f / 1024.0f, 452.0f / 1024.0f, 45.0f / 1024.0f, 40.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(2):
			// <SubTexture height = "40" width = "45" y = "452" x = "282" name = "meteorGrey_med2.png" / >
			newEnemy.sprite = SheetSprite(textureID, 282.0f / 1024.0f, 452.0f / 1024.0f, 45.0f / 1024.0f, 40.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		case(1):
			//<SubTexture height = "43" width = "43" y = "447" x = "651" name = "meteorBrown_med1.png" / >
			newEnemy.sprite = SheetSprite(textureID, 651.0f / 1024.0f, 447.0f / 1024.0f, 43.0f / 1024.0f, 43.0f / 1024.0f, (size * 0.01f + 0.2f));
			break;
		}
		newEnemy.rotation = rand();
		newEnemy.width = newEnemy.sprite.width;// * newEnemy.sprite.size;
		newEnemy.height = newEnemy.sprite.height; // *newEnemy.sprite.size;
		//need to be tweaked if there are multiple lines of enemies
		newEnemy.x = rand() % 8 - 3.55f;
		newEnemy.y = rand() % 5 - 2.0f;
		newEnemy.velocity_x = (rand() % 8 - 4.0f) / 100.0f;
		newEnemy.velocity_y = (rand() % 8 - 4.0f) / 100.0f;
		newEnemy.friction_x = 0.0f;
		newEnemy.friction_y = 0.0f;
		newEnemy.visible = true;
		newEnemy.isStatic = false;
		newEnemy.entityType = TYPE_ENEMY;
		entities.push_back(newEnemy);
	}
}
void ClassDemoApp::SpawnStars(){
	for (int i = 0; i < numStars; i++){
		Entity star;
		int size = rand() % 3 + 1;
		switch (size){
		case(3) :
			//<SubTexture height="24" width="25" y="681" x="628" name="star1.png"/>
			star.sprite = SheetSprite(textureID, 628.0f / 1024.0f, 681.0f / 1024.0f, 25.0f / 1024.0f, 24.0f / 1024.0f, (size * 0.01f + 0.05f));
			break;
		case(2) :
			// SubTexture height="24" width="25" y="84" x="222" name="star2.png"/>
			star.sprite = SheetSprite(textureID, 222.0f / 1024.0f, 84.0f / 1024.0f, 25.0f / 1024.0f, 24.0f / 1024.0f, (size * 0.01f + 0.05f));
			break;
		case(1) :
			//<SubTexture height="24" width="24" y="300" x="576" name="star3.png"/>
			star.sprite = SheetSprite(textureID, 576.0f / 1024.0f, 300.0f / 1024.0f, 24.0f / 1024.0f, 24.0f / 1024.0f, (size * 0.01f + 0.05f));
			break;
		}		
		star.width = star.sprite.width / star.sprite.size;
		star.height = star.sprite.height / star.sprite.size;
		star.x = rand() % 8 - 3.55f;
		star.y = rand() % 5 - 2.0f;
		star.visible = true;
		star.isStatic = true;
		star.entityType = TYPE_LEVEL;
		entities.push_back(star);
	}
}
float mixSamples(float A, float B) {
	if (A < 0 && B < 0) {
		return (A + B) - (A * B) / -1.0;
	}
	else if (A > 0 && B > 0) {
		return (A + B) - (A * B) / 1.0;
	}
	else {
		return A + B;
	}
}
void appAudioCallback(void *userdata, Uint8 *stream, int len) {
	ClassDemoApp *app = (ClassDemoApp*)userdata;
	memset(stream, 0, len);
	for (int i = 0; i < app->mixSounds.size(); i++) {
		MixerSound *sound = &app->mixSounds[i];
		if (sound->loaded && sound->playing) {
			for (int s = 0; s < len / 4; s++) {
				float *sourceBuffer = (float*)(sound->buffer + sound->offset);
				((float*)stream)[s] = mixSamples(((float*)stream)[s], (sourceBuffer[s] * sound->volume));
			}
			sound->offset += len;
			if (sound->offset >= sound->length - len) {
				if (sound->loop) {
					sound->offset = 0;
				}
				else {
					sound->playing = false;
				}
			}
		}
	}
}
int ClassDemoApp::loadSound(const char *soundFile) {
	Uint8 *buffer;
	SDL_AudioSpec spec;
	Uint32 bufferSize;
	if (SDL_LoadWAV(soundFile, &spec, &buffer, &bufferSize) == NULL) {
		return -1;
	}
	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq,
		deviceSpec.format, deviceSpec.channels, deviceSpec.freq);
	cvt.len = bufferSize;
	cvt.buf = new Uint8[bufferSize * cvt.len_mult];
	memcpy(cvt.buf, buffer, bufferSize);

	// This line crashes my code for some reason
	//SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(buffer);

	MixerSound sound;
	sound.buffer = cvt.buf;
	sound.length = cvt.len_cvt;
	sound.loaded = true;
	sound.offset = 0;
	sound.format = deviceSpec.format;
	sound.volume = 1.0;
	sound.playing = false;
	mixSounds.push_back(sound);
	return mixSounds.size() - 1;
}
void ClassDemoApp::playSound(int soundIndex, bool loop){
	if (soundIndex >= 0 && soundIndex < mixSounds.size()) {
		mixSounds[soundIndex].playing = true;
		mixSounds[soundIndex].offset = 0;
		mixSounds[soundIndex].loop = loop;
	}
}
void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("Final Project - Matthew Pon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);	
	#ifdef _WINDOWS
		glewInit();
	#endif
	// Setup
	//Sets pixel size and offset of rendering area
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	//Pass the matrices to our program
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	textureID = LoadTexture(RESOURCE_FOLDER"sheet.png");
	LevelTexture = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");
	
	state = STATE_TITLE_SCREEN;

	/*SDL_AudioSpec deviceSpec;
	deviceSpec.freq = 44100;// sampling rate (samples a second)
	deviceSpec.format = AUDIO_F32; // audio format
	deviceSpec.channels = 2; // how many channels (1 = mono, 2 = stereo)
	deviceSpec.samples = 512; // audio buffer size in samples (power of 2)
	
	// This line is broken
	deviceSpec.callback = appAudioCallback;// our callback function
	deviceSpec.userdata = (void*)this;

	// open new audio device with our requested settings 
	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &deviceSpec, 0, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	SDL_PauseAudioDevice(dev, 0); // start playback on this device

	LaserSound = loadSound(RESOURCE_FOLDER"Laser_Shoot.wav");
	music = loadSound(RESOURCE_FOLDER"Podington_Bear_-_Seven_Up.mp3");	
	
	playSound(music, true);
	*/

	//Simple SOund
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	laserSound1 = Mix_LoadWAV(RESOURCE_FOLDER"Laser_Shoot.wav");
	laserSound2 = Mix_LoadWAV(RESOURCE_FOLDER"Laser_Shoot2.wav");
	hitSound = Mix_LoadWAV(RESOURCE_FOLDER"Hit_Hurt.wav");
	bgmusic = Mix_LoadWAV(RESOURCE_FOLDER"SevenUp.wav");
	
	//bgmusic = Mix_LoadMUS(RESOURCE_FOLDER"SevenUp.mp3");

	//Mix_PlayMusic(bgmusic, -1);
	Mix_PlayChannel(2, bgmusic, -1);


	//Enabling Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE); // or additive blending
	
	//Keeping Time
	timeLeftOver = 0.0f;
	lastFrameTicks = 0.0f;

	//Controller support
	xDir = 0.0f; 
	yDir = 0.0f;

	//Mouse Support
	positionX = 0.0f; 
	positionY = 0.0f;
	angle = 0.0f;	

	//SDL_Event event;
	done = false;
}
void ClassDemoApp::shootBullet(Entity * entity){
	Bullet newBullet;
	if (entity == &entities[0]){
		//	<SubTexture name="laserRed01.png" x="858" y="230" width="9" height="54"/>
		newBullet.sprite = SheetSprite(textureID, 858.0f / 1024.0f, 230.0f / 1024.0f, 9.0f / 1024.0f, 54 / 1024.0f, 0.2f);
	}
	else if (entity == &entities[1]){
		//<SubTexture height = "54" width = "9" y = "421" x = "856" name = "laserBlue01.png" / >
		newBullet.sprite = SheetSprite(textureID, 856.0f / 1024.0f, 421.0f / 1024.0f, 9.0f / 1024.0f, 54.0f / 1024.0f, 0.2f);
	}
	newBullet.width = newBullet.sprite.width; //  * newBullet.sprite.size;
	newBullet.height = newBullet.sprite.height; // *newBullet.sprite.size;
	newBullet.visible = true;
	newBullet.isStatic = false;
	newBullet.timeAlive = 0.0f;
	newBullet.rotation = entity->rotation;
	if (entity == &entities[0]){
		newBullet.velocity_x = sinf(newBullet.rotation) * -BULLETSPEED;
		newBullet.velocity_y = cosf(newBullet.rotation) * BULLETSPEED;
		Mix_PlayChannel(0, laserSound1, 0);
	}
	else if (entity == &entities[1]){
		newBullet.velocity_x = sinf(newBullet.rotation) * BULLETSPEED;
		newBullet.velocity_y = cosf(newBullet.rotation) * -BULLETSPEED;
		Mix_PlayChannel(0, laserSound2, 0);
	}
	newBullet.x = entity->x + newBullet.velocity_x * (FIXED_TIMESTEP * 20);
	newBullet.y = entity->y + newBullet.velocity_y * (FIXED_TIMESTEP * 20);
	newBullet.entityType = TYPE_BULLET;
	bullets.push_back(newBullet);

	//playSound(LaserSound, false);
	
}
bool ClassDemoApp::shouldRemoveBullet(Bullet bullet){
	if (bullet.timeAlive > BULLETLIFETIME || bullet.visible == false){
		if (bullet.visible == false){
			Mix_PlayChannel(1, hitSound, 0);
		}
		if (bullet.collidesWith(&(entities[0]))){
			entities[0].visible = false;
		}
		else if (bullet.collidesWith(&entities[1])){
			entities[1].visible = false;
		}
		return true;
	}
	else {
		return false;
	}
}
void ClassDemoApp::DrawText(int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
bool ClassDemoApp::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '='); 
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
} 
bool ClassDemoApp::readLayerData(std::ifstream &stream) {
	string line; 
	while (getline(stream, line)) {
		if (line == "") { break; } 
		istringstream sStream(line); 
		string key, value; 
		getline(sStream, key, '='); 
		getline(sStream, value); 
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line); 
				istringstream lineStream(line); 
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ','); 
					unsigned char val = (unsigned char)atoi(tile.c_str()); 
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0 
						levelData[y][x] = val-1;
					}
					else { 
						levelData[y][x] = 0; 
					}
				}
			}
		}
	} 
	return true;
}
bool ClassDemoApp::readEntityData(std::ifstream &stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line); 
		string key, value; 
		getline(sStream, key, '='); 
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition; 
			getline(lineStream, xPosition, ','); 
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
		//placeEntity(type, placeX, placeY);
		}
	} 
	return true;
}
void ClassDemoApp::loadLevelData(){
	ifstream infile("level1.txt");
	string line;
	while (getline(infile, line)){
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[ObjectsLayer]") {
			readEntityData(infile);
		}
	}
	/* Static / Dynamic Level
	Entity platform;
	//<SubTexture height="39" width="222" y="78" x="0" name="buttonBlue.png"/>
	platform.sprite = SheetSprite(textureID, 0.0f / 1024.0f, 78.0f / 1024.0f, 222.0f / 1024.0f, 39.0f / 1024.0f, 0.5f);
	platform.rotation = 0.0f;
	// currently settting player dimensions to sprite dimensions,
	// should tweak later
	platform.width = platform.sprite.width / platform.sprite.size;
	platform.height = platform.sprite.height / platform.sprite.size;
	platform.x = 0.0f;
	platform.y = -1.7f;
	platform.visible = true;
	platform.isStatic = true;
	platform.entityType = TYPE_LEVEL;
	entities.push_back(platform);
	*/

	/* example tile level
	unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] =
	{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 20, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 32, 33, 33, 34, 32, 33, 33, 34, 33, 35, 100, 101, 35, 32, 33, 32, 34, 32, 33, 32, 33, 33 }
	};

	memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
	*/
}
void ClassDemoApp::buildLevel(){
	int nonEmptySprites = 0;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			//if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					(float)TILE_SIZE * x, (float)-TILE_SIZE * y, (float)TILE_SIZE * x, (float)(-TILE_SIZE * y) - TILE_SIZE, (float)(TILE_SIZE * x) + TILE_SIZE, (float)(-TILE_SIZE * y) - TILE_SIZE,
					(float)TILE_SIZE * x, (float)-TILE_SIZE * y, (float)(TILE_SIZE * x) + TILE_SIZE, (float)(-TILE_SIZE * y) - TILE_SIZE, (float)(TILE_SIZE * x) + TILE_SIZE, (float)-TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight), u, v,
					u + spriteWidth, v + (spriteHeight), u + spriteWidth, v
				});
				nonEmptySprites++;
			//}
		}
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, LevelTexture);
	glDrawArrays(GL_TRIANGLES, 0, nonEmptySprites * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
void ClassDemoApp::titleScreenRender(){
	//modelMatrix.identity();
	//modelMatrix.Translate(-3.25f, 1.25f, 0.0f);
	//modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "X Fighter", 0.3f, 0.0f);

	modelMatrix.identity();
	modelMatrix.Translate(-2.75f, -0.8f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "Player1                     Player2", 0.15f, 0.0f);
	modelMatrix.identity();
	modelMatrix.Translate(-2.75f, -1.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "[Q][W][E]      [Space]      [U][I][O]", 0.15f, 0.0f);
	modelMatrix.identity();
	modelMatrix.Translate(-2.75f, -1.2f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "[A]   [D]      To start     [J]   [L]", 0.15f, 0.0f);
}
void ClassDemoApp::gameRender(){
	modelMatrix.identity();
	//modelMatrix.Translate(-4.0f, 3.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
//	buildLevel();

	for (int i = entities.size() - 1; i >= 0; i--){
		// Pass shaderprogram and call entity renders
		entities[i].Render(program);
	}
	for (int i = 0; i < bullets.size(); i++){
		bullets[i].Render(program);
	}
	for (int i = 0; i < emitters.size(); i++){
		//emitters currently broken
//		emitters[i].Render(program);
	}
	// Screen Scrolling
	//viewMatrix.setPosition(-entities[0].x, -entities[0].y, 0);
	//program->setViewMatrix(viewMatrix);
}
void ClassDemoApp::gameOverRender(){
	//modelMatrix.identity();
	modelMatrix.Translate(-1.0f, 0.0f, 0.0f);
	//modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), GameEndBuffer, 0.25f, 0.001f);

	modelMatrix.identity();
	modelMatrix.Translate(-1.0f, -1.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "[Space] to restart", 0.15f, 0.0f);
	modelMatrix.identity();
	modelMatrix.Translate(-1.0f, -1.2f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "[Esc] to quit", 0.15f, 0.0f);
}
void ClassDemoApp::Render() {
	// Clear, render, and swap the window
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state){
	case STATE_TITLE_SCREEN:
		titleScreenRender();
		break;
	case STATE_GAME_LEVEL:
		gameRender();
		break;
	case STATE_GAME_OVER:
		gameOverRender();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}
void ClassDemoApp::ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		switch (state){
		case STATE_TITLE_SCREEN:
		case STATE_GAME_OVER:
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
					state = STATE_GAME_LEVEL;
					numEnemies = rand() % (MAXENEMIES) + 3;
					numStars = rand() % (MAXSTARS) + 10;
					entities.clear();
					emitters.clear();
					bullets.clear();
					SpawnPlayer();
					SpawnEnemies();
					SpawnStars();
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					done = true;
				}
			}
			break; 
		case STATE_GAME_LEVEL:
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			// Input Events: Action Events, Jumping, Shooting
			//Controller support, ADD Buttons under here
			/*else if(event.type == SDL_JOYAXISMOTION){
				if(event.jaxis.which == 0){
				// event.jaxis.which tells us which controller (e.g. 0,1,etc.)
				// event.jaxis.axis tells us which axis moved (0 for x-axis ,1 for y, etc.)
				// event.jaxis.value tells us the new value of the axis from -32767 to 32767
				if(event.jaxis.axis == 0) {
				float floatVal = ((float)event.jaxis.value) / 32767.0f;
				if(fabs(floatVal) > 0.2f){
				xDir = floatVal;
				} else {
				xDir = 0.0f;
				}
				} else if(event.jaxis.axis == 1) {
				float floatVal = ((float)event.jaxis.value) / 32767.0f;
				if(fabs(floatVal) > 0.2f){
				yDir = floatVal;
				} else {
				yDir = 0.0f;
				}
				}
				}
				}
				// event.jbutton.which tells us which controller (e.g. 0,1,etc.)
				// event.jbutton.button tells us which button was pressed (0,1,2…etc)
				else if (event.type == SDL_JOYBUTTONDOWN){
				if(event.jbutton.button == 0){
				angle += 3.1415926536 * 0.25f;
				}
				} else if (event.type == SDL_MOUSEMOTION){
				float unitX = (((float)event.motion.x / 640.0f) * 3.554) - 1.777f;
				float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0) - 1.0f;

				positionX = unitX;
				positionY = unitY;
				} else if (event.type == SDL_MOUSEBUTTONDOWN){
				// event.button.x is the click x position
				// event.button.y is the click y position
				// event.button.button is the mouse button that was clicked (1,2,3,etc.)
				}*/
			//Keyboard Controls
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					//shootBullet(&entities[0]);
					//entities[0].velocity_y = 1.0f;
				}
				else if ((event.key.keysym.scancode == SDL_SCANCODE_Q) ||
					(event.key.keysym.scancode == SDL_SCANCODE_E)){
					shootBullet(&entities[0]);
				}
				
				if ((event.key.keysym.scancode == SDL_SCANCODE_U) ||
					(event.key.keysym.scancode == SDL_SCANCODE_O)){
					shootBullet(&entities[1]);
				}
			}
			// Polling Input: Continuous Input, movement, modifiers
			const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_D]) {
				entities[0].acceleration_rotation = -ROTATIONACCELERATION;
					;
			}
			else if (keys[SDL_SCANCODE_A]) {
				entities[0].acceleration_rotation = ROTATIONACCELERATION;
			}
			else {
				entities[0].acceleration_rotation = 0.0f;
			}
			if (keys[SDL_SCANCODE_W]) {
				entities[0].acceleration_x = sinf(entities[0].rotation) * -MOVEMENTACCELERATION;
				entities[0].acceleration_y = cosf(entities[0].rotation) * MOVEMENTACCELERATION;
			}
			else if (keys[SDL_SCANCODE_S]) {
				//entities[0].acceleration_y = -0.75f;
			}
			else {
				entities[0].acceleration_x = 0.0f;
				entities[0].acceleration_y = 0.0f;
			}
			// SEPARATE IF, ELSE IF STRUCTURES FOR DIFFERENT PLAYERS
			if (keys[SDL_SCANCODE_L]) {
				entities[1].acceleration_rotation = -ROTATIONACCELERATION;
			}
			else if (keys[SDL_SCANCODE_J]) {
				entities[1].acceleration_rotation = ROTATIONACCELERATION;
			}
			else {
				entities[1].acceleration_rotation = 0.0f;
			}
			if (keys[SDL_SCANCODE_I]) {
				entities[1].acceleration_x = sinf(entities[1].rotation) * MOVEMENTACCELERATION;
				entities[1].acceleration_y = cosf(entities[1].rotation) * -MOVEMENTACCELERATION;
			}
			else if (keys[SDL_SCANCODE_K]) {
				//entities[1].acceleration_y = -0.75f;
			}
			else {
				entities[1].acceleration_x = 0.0f;
				entities[1].acceleration_y = 0.0f;
			}
			
			break;
		}
	}	
}
void ClassDemoApp::titleScreenUpdate(float elapsed){
	animationTime = animationTime + elapsed; 
	float animationValue = mapValue(animationTime, animationStart,
		animationEnd, 0.0, 1.0);
	modelMatrix.identity(); 
	modelMatrix.Translate((1.0 - animationValue) * 0.0 + animationValue * 1.0, 0.0, 0.0);
	
}
bool ClassDemoApp::isSolid(int x, int y){
	switch (levelData[y][x]){
	case (103) :
	case (104) :
	case (128) :
	case (132) :
	case (152) :
			 return true;
		break;
	}
	return false;
}
void ClassDemoApp::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}
void ClassDemoApp::worldCollisions(Entity * entity){
	int tile_x;
	int tile_y;
	worldToTileCoordinates(entity->x, entity->y - entity->height / 2, &tile_x, &tile_y);
	if (-TILE_SIZE * tile_y > entity->y - entity->height / 2){
		if (isSolid(tile_x, tile_y)){
			entity->y += fabs((-TILE_SIZE * tile_y) - (entity->y - entity->height / 2) + 0.01f);
			entity->velocity_y = 0.0f;
			entity->collidedBottom = true;
		}
	}
	// COPY FOR TOP, LEFT AND RIGHT

	//is bottom of entity intersecting with a tile
	// -TILE_SIZE * tile_y > entity.y - entity.height/2
	// if intersecting, check if tile is solid
	// if solid, adjust Y coordinate by:
	// (-TILE_SIZE * tile_y) -  (entity.y - entity.height/2) + a little
	// reset velocity to 0
	// set collision flag

}
//gameUpdate is passed FixedTimeStep as elapsed
void ClassDemoApp::gameUpdate(float elapsed){
	// move things based on time passed
	// check for collisions and respond to them

	// CHECK thiS LINE
	//bullets.erase(std::remove_if(bullets.begin(), bullets.end(), ClassDemoApp::shouldRemoveBullet), bullets.end());
	for (int i = 0; i < bullets.size(); i++){
		bullets[i].timeAlive += elapsed;
		if (shouldRemoveBullet(bullets[i])){
			bullets.erase(bullets.begin() + i);
		}
		else{
			bullets[i].Update(elapsed, entities);
		}
	}

	for (int i = 0; i < entities.size(); i++){
		if (entities[i].visible == false){
			if (entities[i].entityType == TYPE_ENEMY){
				entities.erase(entities.begin() + i);
				Mix_PlayChannel(1, hitSound, 0);
			}
			else if (entities[i].entityType == TYPE_PLAYER){
				state = STATE_GAME_OVER;
				Mix_PlayChannel(1, hitSound, 0);
			}
		}
		else {
			entities[i].Update(elapsed, entities);
			//worldCollisions(&(entities[i]));
			
		}
	}

	//update individual emitters
	emitters[0].position_x = entities[0].x;
	emitters[0].position_y = entities[0].y;
	emitters[0].velocity_x = -entities[0].velocity_x;
	emitters[0].velocity_y = -entities[0].velocity_y;

	emitters[1].position_x = entities[1].x;
	emitters[1].position_y = entities[1].y;
	emitters[1].velocity_x = -entities[1].velocity_x;
	emitters[1].velocity_y = -entities[1].velocity_y;


	for (int i = 0; i < emitters.size(); i++){
		//Emitters currently broken
	//	emitters[i].Update(elapsed);
	}
	//Controller support
	/*positionX += elapsed * 0.5f * xDir;
	positionY += elapsed * 0.5f * -yDir;
	*/
}
void ClassDemoApp::gameOverUpdate(float elapsed){
	animationTime = animationTime + elapsed;
	
	if (entities[0].visible == false && entities[1].visible == false){
		GameEndBuffer = "Game Over - Draw";
		modelMatrix.identity();
		modelMatrix.Translate(-1.0, 0.0, 0.0);
	}
	else if (entities[0].visible == false){
		GameEndBuffer = "Player 2 Wins!";
		float animationValue = mapValue(animationTime, animationStart,
			animationEnd, 0.0, 1.0);
		modelMatrix.identity();
		modelMatrix.Translate((1.0 - animationValue) * 0.0 + animationValue * 1.0, 0.0, 0.0);
	}
	else if (entities[1].visible == false){
		GameEndBuffer = "Player 1 Wins!";
		float animationValue = mapValue(animationTime, animationStart,
			animationEnd, 1.0, 2.0);
		modelMatrix.identity();
		modelMatrix.Translate((1.0 - animationValue) * 0.0 - animationValue * 1.0, 0.0, 0.0);
	}
}
void ClassDemoApp::Update(float elapsed) {
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;

	switch (state){
	case STATE_TITLE_SCREEN:
		titleScreenUpdate(FIXED_TIMESTEP);
		break;
	case STATE_GAME_LEVEL:
	//	loadLevelData();
		animationTime = 0;
		gameUpdate(FIXED_TIMESTEP);
		break;
	case STATE_GAME_OVER:
		gameOverUpdate(FIXED_TIMESTEP);
		break;
	}
}
bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ProcessEvents();
	Update(elapsed);
	Render();

	return done;
}