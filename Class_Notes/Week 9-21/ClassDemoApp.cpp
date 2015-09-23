#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp() {
	
}

void ClassDemoApp::Setup(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_Joystick * joystick = SDL_JoystickOpen(0); // controller support

	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	
	// Setup
		glViewport(0, 0, 640, 360); //Sets pixel size and offset of rendering area
	//????


}