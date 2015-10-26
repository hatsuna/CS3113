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
Press Space to serve
Player 1: W + S
Player 2: Up + Down
*/
// CTRL+F WIP or Assignment # for unimplemented code
// Assignment 04 WIP
// memcpy(array, array, arraysize) to copy one array to another


// Todo?
// Finish Bullet class
// Bullet Management
// entity types
// enemy implementation
// Game states

class Bullet{
public:
	Bullet(){

	}

	void erase(){

	}

	float x;
	float y;
	float angle;
	float speed;
	float timeAlive;
	bool visible;
};

ClassDemoApp::ClassDemoApp() {
	Setup();
}

void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("Assignment 03 Space Invaders - Matthew Pon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);	
	#ifdef _WINDOWS
		glewInit();
	#endif
	// Setup
	//Sets pixel size and offset of rendering area
	glViewport(0, 0, 640, 360);

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	//Pass the matrices to our program
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	player1 = Entity(RESOURCE_FOLDER"sheet.png");
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	player1.sprite = SheetSprite(player1.textureID, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.5f);
	player1.rotation = 0.0f;
	// currently settting player dimensions to sprite dimensions,
	// should tweak later
	player1.width = player1.sprite.width;
	player1.height = player1.sprite.height;
	player1.x = 0.0f;
	player1.y = -1.5f;
	entities.push_back(player1);

	// Assignment 04 WIP
	// When adding a character spritesheet, divide by sheet size
	// spriteSheetTexture = LoadTexture("sheet.png");
	// player.sprite = SheetSprite(spriteSheetTexture, 67.0f / 512.0f, ... )
	//When setting collision, sprite and collision box might not need to be the same

	// push back player & other entities onto vector of entities

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

ClassDemoApp::~ClassDemoApp() {
	// SDL and and OpenGL cleanup (joysticks, textures, etc.)
	SDL_JoystickClose(joystick);
	SDL_Quit();
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

void ClassDemoApp::Render() {
	// Clear, render, and swap the window
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < entities.size(); i++){
		// Pass shaderprogram and call entity renders
		entities[i].program = program;
		entities[i].Render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		} // Input Events: Action Events, Jumping, Shooting
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
				ball.speed = 2.0f;
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
	}	
}

void ClassDemoApp::Update(float elapsed) {
	// move things based on time passed
	// check for collisions and respond to them

	//replace all with
	for(int i = 0; i < entities.size(); i++){
		entities[i].Update(elapsed, entities);
	}

	//Controller support
	/*positionX += elapsed * 0.5f * xDir;
	positionY += elapsed * 0.5f * -yDir;
	*/
	/*
	//ball collision detection
	//if ball goes off the screen at the sides, re-serve the ball
	if (ball.x > 3.55f){
		ball.x = 1.0f;
		ball.y = 0.0f;
		ball.speed = 0.0f;
		ball.rotation = (rand() % 90 + 135) * (3.1415926 / 180); // 135 to 225
	}
	else if (ball.x < -3.55f){
		ball.x = -1.0f;
		ball.y = 0.0f;
		ball.speed = 0.0f;
		ball.rotation = (rand() % 90 - 45) * (3.1415926 / 180); // -45 to 45
	}
	// if ball hits the top or bottom of the screen bounce it
	if (ball.y + (ball.height / 2) >= 2.0f || ball.y - (ball.height / 2) <= - 2.0f){
		ball.rotation = -ball.rotation;
	}

	//ball collision with paddles
	if ((ball.y - ball.height / 2 >= player1.y + player1.height / 2) ||
		(ball.y + ball.height / 2 <= player1.y - player1.height / 2) ||
		(ball.x - ball.width / 2 >= player1.x + player1.width / 2) ||
		(ball.x + ball.width / 2 <= player1.x - player1.width / 2)){
		//if true, then not intersecting
	}
	else{
		//intersecting
		ball.x = player1.x + player1.width;
		ball.rotation = 180 * (3.1415926 / 180) - ball.rotation;
	}

	if ((ball.y - ball.height / 2 >= player2.y + player2.height / 2) ||
		(ball.y + ball.height / 2 <= player2.y - player2.height / 2) ||
		(ball.x - ball.width / 2 >= player2.x + player2.width / 2) ||
		(ball.x + ball.width / 2 <= player2.x - player2.width / 2)){
		//if true, then not intersecting
	}
	else{
		//intersecting
		ball.x = player2.x - player2.width;
		ball.rotation = 180 * (3.1415926 / 180) - ball.rotation;
	}

	ball.x += elapsed * ball.speed * cos(ball.rotation);
	ball.y += elapsed * ball.speed * sin(ball.rotation);
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

bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ProcessEvents();
	Update(elapsed);
	Render();

	return done;
}