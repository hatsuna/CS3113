#include "ClassDemoApp.h"
/* 
 * Matthew Pon
 * CS3113 Assignment 04
 * SideScoller
 - Make a simple scolling platformer game demo
 - It must use a tilemap or static/dynamic entities
 - It must scroll
 - It must be either procedurally generated or load levels from a file
 */
/*
Controls
Press Space to shoot
Player 1: A + D
*/
// CTRL+F WIP or Assignment # for unimplemented code
// Assignment 04 WIP
// memcpy(array, array, arraysize) to copy one array to another

ClassDemoApp::ClassDemoApp() {
	Setup();
}
ClassDemoApp::~ClassDemoApp() {
	// SDL and and OpenGL cleanup (joysticks, textures, etc.)
	SDL_JoystickClose(joystick);
	SDL_Quit();
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
	player1.sprite = SheetSprite(textureID, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.5f);
	player1.rotation = 0.0f;
	// currently settting player dimensions to sprite dimensions,
	// should tweak later
	player1.width = player1.sprite.width; // player1.sprite.size;
	player1.height = player1.sprite.height; // player1.sprite.size;;
	player1.x = 0.0f;
	player1.y = 0.0f;
	player1.visible = true;
	player1.isStatic = false;
	player1.entityType = TYPE_PLAYER;
	entities.push_back(player1);
}
void ClassDemoApp::SpawnEnemies(){
	for (int i = 0; i < numEnemies; i++){
		Entity newEnemy;
		//<SubTexture height = "84" width = "93" y = "468" x = "425" name = "enemyBlue1.png" / >
		newEnemy.sprite = SheetSprite(textureID, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.3f);
		newEnemy.rotation = 0.0f;
		newEnemy.width = newEnemy.sprite.width;// * newEnemy.sprite.size;
		newEnemy.height = newEnemy.sprite.height; // *newEnemy.sprite.size;
		//need to be tweaked if there are multiple lines of enemies
		newEnemy.x = -3.55 + ((3 * 2) / numEnemies) * (i + 1);
		newEnemy.y = 1.5f;
		newEnemy.velocity_x = 0.5f;
		newEnemy.visible = true;
		newEnemy.isStatic = false;
		newEnemy.entityType = TYPE_ENEMY;
		entities.push_back(newEnemy);
	}
}
void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("Assignment 04 Platformer - Matthew Pon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
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

	state = STATE_TITLE_SCREEN;
	score = 0;
	numEnemies = 5;

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
void ClassDemoApp::shootBullet(){
	Bullet newBullet;
	//	<SubTexture name="laserRed01.png" x="858" y="230" width="9" height="54"/>
	newBullet.sprite = SheetSprite(textureID, 858.0f / 1024.0f, 230.0f / 1024.0f, 9.0f / 1024.0f, 54 / 1024.0f, 0.3f);
	newBullet.width = newBullet.sprite.width; //  * newBullet.sprite.size;
	newBullet.height = newBullet.sprite.height; // *newBullet.sprite.size;
	newBullet.visible = true;
	newBullet.isStatic = false;
	newBullet.timeAlive = 0.0f;
	newBullet.x = entities[0].x;
	newBullet.y = entities[0].y;
	newBullet.rotation = 0.0f;
	newBullet.velocity_y = 0.5f;
	newBullet.entityType = TYPE_BULLET;
	bullets.push_back(newBullet);
}
bool ClassDemoApp::shouldRemoveBullet(Bullet bullet){
	if (bullet.timeAlive > 5.0f || bullet.visible == false){
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
void ClassDemoApp::buildLevel(){
	

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

	/* tiled level
	unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] =
	{
		{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
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
		{ 0, 20, 125, 118, 0, 0, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 0, 127, 20, 0 },
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 32, 33, 33, 34, 32, 33, 33, 34, 33, 35, 100, 101, 35, 32, 33, 32, 34, 32, 33, 32, 33, 33 }
	};
	memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);

	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X; float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X; float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y, TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE, (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					TILE_SIZE * x, -TILE_SIZE * y, (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE, (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight), u, v,
					u + spriteWidth, v + (spriteHeight), u + spriteWidth, v
				});
			}
		}
	}*/
}
void ClassDemoApp::titleScreenRender(){
	modelMatrix.identity();
	modelMatrix.Translate(-3.25f, 1.25f, 0.0f);
	modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "Platformer", 0.3f, 0.0f);
}
void ClassDemoApp::gameRender(){
	for (int i = 0; i < entities.size(); i++){
		// Pass shaderprogram and call entity renders
		entities[i].Render(program);
	}
	for (int i = 0; i < bullets.size(); i++){
		bullets[i].Render(program);
	}
}
void ClassDemoApp::gameOverRender(){
	modelMatrix.identity();
	modelMatrix.Translate(-1.0f, 0.0f, 0.0f);
	modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	if (score > 0){
		DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "You Win", 0.25f, 0.001f);
	}

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "Game Over", 0.25f, 0.001f);
}
void ClassDemoApp::Render() {
	// Clear, render, and swap the window
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
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
					entities.clear();
					SpawnPlayer();
				//	SpawnEnemies();
					buildLevel();
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
					shootBullet();
					entities[0].velocity_y = 1.0f;
				}
			}
			// Polling Input: Continuous Input, movement, modifiers
			const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_D]) {
				entities[0].acceleration_x = .75f;
			}
			else if (keys[SDL_SCANCODE_A]) {
				entities[0].acceleration_x = -.75f;
			}
			else {
				entities[0].acceleration_x = 0.0f;
			}
			if (keys[SDL_SCANCODE_W]) {
				//entities[0].acceleration_y = 0.75f;
			}
			else if (keys[SDL_SCANCODE_S]) {
				//entities[0].acceleration_y = -0.75f;
			}
			else {
				entities[0].acceleration_y = 0.0f;
			}
			/* SEPARATE IF, ELSE IF STRUCTURES FOR DIFFERENT PLAYERS
			if (keys[SDL_SCANCODE_UP]) {s
			player2.direction_y = 1.0f;
			}
			else if (keys[SDL_SCANCODE_DOWN]) {
			player2.direction_y = -1.0f;
			}
			*/
			break;
		}
	}	
}
void ClassDemoApp::titleScreenUpdate(){
}
//gameUpdate is passed FixedTimeStep as elapsed
void ClassDemoApp::gameUpdate(float elapsed){
	// WIP Screen Scrolling
	/*viewMatrix.Translate(entities[0].modelMatrix.inverse);
	program->setViewMatrix(viewMatrix);
	*/

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
				score += 100;
			}
			else if (entities[i].entityType == TYPE_PLAYER){
				state = STATE_GAME_OVER;
			}
		}
		else {
			entities[i].Update(elapsed, entities);
		}
	}
	//Controller support
	/*positionX += elapsed * 0.5f * xDir;
	positionY += elapsed * 0.5f * -yDir;
	*/
}
void ClassDemoApp::gameOverUpdate(){
	
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
		titleScreenUpdate();
		break;
	case STATE_GAME_LEVEL:
		gameUpdate(FIXED_TIMESTEP);
		break;
	case STATE_GAME_OVER:
		gameOverUpdate();
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