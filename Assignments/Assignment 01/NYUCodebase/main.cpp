#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

/*
Matthew Pon
NYU Fall 2015 Intro to Game Programming
Assignment 01:

Create a simple 2D scene using textured polygons.
You can use any images you want, but feel free to use the assets in the class github repo.
At least one element must be animated.
You must use at least 3 different textures.
Commit the source to your github repository and email me the link.
*/

SDL_Window* displayWindow;

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
}

void DrawSprite(GLint texture, float x, float y, float rotation){
	/*
	Set position attributes for 2 triangles.
	Set texture coordinate attributes for 2 triangles.
	Bind the texture we want to use.
	Draw arrays.
	Disable attribute arrays.
	*/
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	
	//Setup
	glViewport(0, 0, 640, 360); //Sets pixel size and offset of rendering area
	//????

	GLuint shipTexture = LoadTexture(RESOURCE_FOLDER"playerShip2_red.png");
	GLuint asteroidTexture = LoadTexture(RESOURCE_FOLDER"meteorGrey_big4.png");
	GLuint ufoTexture = LoadTexture(RESOURCE_FOLDER"ufoBlue.png");

	//Enabling Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE); // or additive blending
	
	//Keeping Time
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		//Keeping TIme
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks; //elapsed is how many seconds have elapsed since last frame
		lastFrameTicks = ticks;
		
		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		angle += elapsed;
	
		DrawSprite(shipTexture, 1.0f, 0.0f, angle);
		DrawSprite(asteroidTexture, 1.5f, 1.0f, 0.0f);
		DrawSprite(ufoTexture, -2.4f, -1.4f, 0.0f);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
