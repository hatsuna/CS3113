#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp() {
	Setup();
}

void ClassDemoApp::Setup(){
	// SDL and OpenGL init
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif


	// Setup
	glViewport(0, 0, 640, 360); //Sets pixel size and offset of rendering area
	
	/* in entity code
	projectionMatrix;
	modelMatrix;
	viewMatrix;

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	*program = ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	*/

	ship = Entity(RESOURCE_FOLDER"playerShip2_red.png");
	asteroid = Entity(RESOURCE_FOLDER"meteorGrey_big4.png");
	ufo = Entity(RESOURCE_FOLDER"ufoBlue.png");
	
	//Enabling Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE); // or additive blending
	
	//Keeping Time
	lastFrameTicks = 0.0f;

	xDir = 0.0f; //Controller support
	yDir = 0.0f;
	
	positionX = 0.0f; //Mouse Support
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

/*
GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	//Creates a new OpenGL texture ID
	GLuint textureID;
	glGenTextures(1, &textureID);
	//Bind a texture to a texture target
	glBindTexture(GL_TEXTURE_2D, textureID);
	//Sets the texture data of the specified texture target, RGBA or RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	//Sets a texture parameter of the specified texture target, must be set before texture can be used
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}*/
/*
void DrawSprite(GLint texture, float x, float y, float rotation){
	/*
	Set position attributes for 2 triangles.
	Set texture coordinate attributes for 2 triangles.
	Bind the texture we want to use.
	Draw arrays.
	Disable attribute arrays.
	*/

	/*ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	*/
/*
	modelMatrix.identity();
	modelMatrix.Rotate(rotation);
	modelMatrix.Translate(x, y, 0.0f);

	//Pass the matrices to our program
	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);

	//Use the specified program id
	glUseProgram(program.programID);

	//Defines an array of vertex data (counter-clockwise!)
	float vertices[] = { 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	//Enables a vertex attribute array
	glEnableVertexAttribArray(program.positionAttribute);

	float texCoords[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texture);
	//Render previously defined arrays
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disables a vertex attribute array
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}
*/

void ClassDemoApp::Render() {
	// Clear, render, and swap the window
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//should really be doing these updates as input happens

	ship.Update(positionX, positionY, angle);
	ship.DrawSprite();
	asteroid.Update(1.5f, 1.0f, 0.0f);
	asteroid.DrawSprite();
	ufo.Update(-2.4f, -1.4f, 0.0f);
	ufo.DrawSprite();

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		} // check for input events
		else if(event.type == SDL_JOYAXISMOTION){ //Controller support, ADD Buttons under here
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
			angle += 3.1415926536 * 0.25;
		}
	}
}

void ClassDemoApp::Update(float elapsed) {
	// move things based on time passed
	// check for collisions and respond to them

	//Controller support
	positionX += elapsed * 0.5f * xDir;
	positionY += elapsed * 0.5f * -yDir;
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