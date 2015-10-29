#include "ClassDemoApp.h"
/* 
 * Matthew Pon
 * CS3113 Assignment 03
 * Space Invaders
 *	It must have 3 states: 
		TITLE SCREEN, GAME and GAME OVER 
	It must display text
	It must use sprite sheets
	You can use any graphics you want (it doesn’t have to be in space! :)
 */
/*
Controls:
Press Space to shoot
Player 1: A + D
*/
// CTRL+F WIP or Assignment # for unimplemented code
// Assignment 04 WIP
// memcpy(array, array, arraysize) to copy one array to another


// Todo?
// Finish Bullet class, extend entities maybe?
// Bullet Management
// entity types
// enemy implementation
// Game states

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
void ClassDemoApp::SpawnPlayer(){
	Entity player1;
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	player1.sprite = SheetSprite(textureID, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.5f);
	player1.rotation = 0.0f;
	// currently settting player dimensions to sprite dimensions,
	// should tweak later
	player1.width = player1.sprite.width;
	player1.height = player1.sprite.height;
	player1.x = 0.0f;
	player1.y = -1.5f;
	player1.visible = true;
	player1.entityType = TYPE_PLAYER;
	entities.push_back(player1);
}
void ClassDemoApp::SpawnEnemies(){
	for (int i = 0; i < numEnemies; i++){
		Entity newEnemy;
		//<SubTexture height = "84" width = "93" y = "468" x = "425" name = "enemyBlue1.png" / >
		newEnemy.sprite = SheetSprite(textureID, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.3f);
		newEnemy.rotation = 0.0f;
		newEnemy.width = newEnemy.sprite.width;
		newEnemy.height = newEnemy.sprite.height;
		//need to be tweaked if there are multiple lines of enemies
		newEnemy.x = -3.55 + ((3 * 2) / numEnemies) * (i + 1);
		newEnemy.y = 1.5f;
		newEnemy.velocity_x = 0.5f;
		newEnemy.visible = true;
		newEnemy.entityType = TYPE_ENEMY;
		entities.push_back(newEnemy);
	}
}
void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("Assignment 03 Space Invaders - Matthew Pon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
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

	// Assignment 04 WIP
	// When adding a character spritesheet, divide by sheet size
	// spriteSheetTexture = LoadTexture("sheet.png");
	// player.sprite = SheetSprite(spriteSheetTexture, 67.0f / 512.0f, ... )
	//When setting collision, sprite and collision box might not need to be the same

	//Enabling Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE); // or additive blending
	
	//Keeping Time
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
	newBullet.width = newBullet.sprite.width;
	newBullet.height = newBullet.sprite.height;
	newBullet.visible = true;
	newBullet.timeAlive = 0.0f;
	newBullet.x = entities[0].x;
	newBullet.y = entities[0].y;
	newBullet.rotation = 0.0f;//(float)(45 - (rand() % 90));
	newBullet.speed = 0.5f;
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
void ClassDemoApp::titleScreenRender(){
	modelMatrix.identity();
	modelMatrix.Translate(-3.25f, 1.25f, 0.0f);
	modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), "SPACE INVADERS", 0.5f, 0.001f);
}
void ClassDemoApp::gameRender(){
	for (int i = 0; i < entities.size(); i++){
		// Pass shaderprogram and call entity renders
		entities[i].program = program;
		entities[i].Render();
	}
	for (int i = 0; i < bullets.size(); i++){
		bullets[i].program = program;
		bullets[i].Render();
	}
	/*
	modelMatrix.identity();
	modelMatrix.Translate(-3.25f, 1.25f, 0.0f);
	modelMatrix.Rotate(0);

	program->setModelMatrix(modelMatrix);

	DrawText(LoadTexture(RESOURCE_FOLDER"font1.png"), std::to_string(score), 0.1f, 0.001f);
	*/
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
					SpawnEnemies();
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
				}
			}
			// Polling Input: Continuous Input, movement, modifiers
			const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_D]) {
				entities[0].acceleration_x = 1.0f;
			}
			else if (keys[SDL_SCANCODE_A]) {
				entities[0].acceleration_x = -1.0f;
			}
			else{
				entities[0].acceleration_x = 0.0f;
			}

			/* SEPARATE IF, ELSE IF STRUCTURES FOR DIFFERENT PLAYERS
			if (keys[SDL_SCANCODE_UP]) {
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
	// Assignment 04 WIP
	/*When updating entities, loop through, updating each entity
	if not static, update entity position
	loop through and check if current entity is colliding with any other entity
	do not check if colliding with self
	only check if colliding with isStatic entities
	*/
	//WIP Assigment 04 Penetration adjustments
	/*
	player.y += player.velocity_y * elapsed;
	if( player.collidesWith(block)){
	float penetration = fabs( fabs(player.y - block.y) - player.height / 2.0f - block.height / 2.0f);
	if player.y > block.y){
	player.y += penetration + 0.0001;
	} else{
	player.y -= penetration + 0.0001;
	}
	player.velocity_y = 0.0f;
	}
	player.x += player.velocity_x * elapsed;
	if( player.collidesWith(block)){
	float penetration = fabs( fabs(player.x - block.x) - player.width / 2.0f - block.width / 2.0f);
	if player.x > block.x){
	player.x += penetration + 0.0001;
	} else{
	player.x -= penetration + 0.0001;
	}
	player.velocity_x = 0.0f;
	}*/
}
void ClassDemoApp::gameOverUpdate(){
	
}
void ClassDemoApp::Update(float elapsed) {
	switch (state){
	case STATE_TITLE_SCREEN:
		titleScreenUpdate();
		break;
	case STATE_GAME_LEVEL:
		gameUpdate(elapsed);
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