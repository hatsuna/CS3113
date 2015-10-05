#include "ClassDemoApp.h"

/* 
 * Matthew Pon
 * CS3113 Assignment 02
 * Pong
 * - Doesn’t need to keep score
 * - Must detect player wins
 * - Images or untextured polygons
 * - Keyboard, mouse or joystick input
 */


// CTRL+F WIP for unimplemented code

ClassDemoApp::ClassDemoApp() {
	Setup();
}

void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("Assignment 02 Pong - Matthew Pon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);	
	#ifdef _WINDOWS
		glewInit();
	#endif


	// Setup
	glViewport(0, 0, 640, 360); //Sets pixel size and offset of rendering area

	/*
	ship = Entity(RESOURCE_FOLDER"playerShip2_red.png");
	asteroid = Entity(RESOURCE_FOLDER"meteorGrey_big4.png");
	ufo = Entity(RESOURCE_FOLDER"ufoBlue.png");
	*/

/*	int * window_height;
	int * window_width;

	SDL_GetWindowSize(displayWindow, window_width, window_height);
	*/
	player1 = Entity(RESOURCE_FOLDER"paddleBlu.png");
	player1.rotation = (90 * (3.1415926 / 180));
	player1.width = player1.width / 640 * 2;
	player1.height = player1.height / 360 * 2;
	player1.x = -3.55f + player1.width / 2;

	player2 = Entity(RESOURCE_FOLDER"paddleRed.png");
	player2.rotation = (90 * (3.1415926 / 180));
	player2.width = player2.width / 640 * 2;
	player2.height = player2.height / 360 * 2;
	player2.x = 3.55f - player2.width / 2;

	//Enabling Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE); // or additive blending
	
	//Keeping Time
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

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

void ClassDemoApp::Render() {
	// Clear, render, and swap the window
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*ship.DrawSprite();
	asteroid.DrawSprite();
	ufo.DrawSprite();*/

	player1.DrawSprite();
	player2.DrawSprite();

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		} // Input Events: Action Events, Jumping, Shooting
		/*else if(event.type == SDL_JOYAXISMOTION){ //Controller support, ADD Buttons under here
			if(event.jaxis.which == 0){
				//printDebugOutput or something here to check event.jaxis.axis
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
		} else if (event.type == SDL_JOYBUTTONDOWN){
			if(event.jbutton.button == 0){
				angle += 3.1415926536 * 0.25f;
			} 
		} else if (event.type == SDL_MOUSEMOTION){
			float unitX = (((float)event.motion.x / 640.0f) * 3.554) - 1.777f;
			float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0) - 1.0f;
			
			positionX = unitX;
			positionY = unitY;
		} else if (event.type == SDL_MOUSEBUTTONDOWN){
			angle += 3.1415926536 * 0.25f;
		}*/
		
		//Keyboard Controls
		/*
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_W){
				player1.direction_y = 1.0f;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
				player2.direction_y = -1.0f;
			}

		}*/
		// Polling Input: Continuous Input, movement, modifiers
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_W]) {
			player1.direction_y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_S]) {
			player1.direction_y = -1.0f;
		}
		else {
			player1.direction_y = 0.0f;
		}

		// SEPARATE IF, ELSE IF STRUCTURES FOR DIFFERENT PLAYERS

		if (keys[SDL_SCANCODE_UP]) {
			player2.direction_y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			player2.direction_y = -1.0f;
		}
		else {
			player2.direction_y = 0.0f;
		}
	}	
}

void ClassDemoApp::Update(float elapsed) {
	// move things based on time passed
	// check for collisions and respond to them

	//Controller support
	/*positionX += elapsed * 0.5f * xDir;
	positionY += elapsed * 0.5f * -yDir;
	*/
	/*
	ship.Update(positionX, positionY, angle);
	asteroid.Update(1.5f, 1.0f, 0.0f);
	ufo.Update(-2.4f, -1.4f, 0.0f);
	*/

	player1.y += elapsed * 1.0f * player1.direction_y;
	player2.y += elapsed * 1.0f * player2.direction_y;
}

//WIP Assignment 03 Add Velocity and Acceleration to Entity class
//WIP Assignment 03 Add Friction "Rate of decrease in velocity"
//WIP lerp


// WIP Assignment03 Fixed update for physics and collision detection
void ClassDemoApp::FixedUpdate(float elapsed){
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
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