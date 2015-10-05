#include "Entity.h"

Entity::Entity(){
	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;

	width = 0.0f;
	height = 0.0f;

	speed = 0.0f;
	direction_x = 0.0f;
	direction_y = 0.0f;
}

Entity::Entity(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);
	//Creates a new OpenGL texture ID
	GLuint textID;
	glGenTextures(1, &textID);
	//Bind a texture to a texture target
	glBindTexture(GL_TEXTURE_2D, textID);
	//Sets the texture data of the specified texture target, RGBA or RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	//Sets a texture parameter of the specified texture target, must be set before texture can be used
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	textureID = textID;

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	
	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;

	width = surface->w;
	height = surface->h;

	speed = 0.0f;
	direction_x = 0.0f;
	direction_y = 0.0f;

}

void Entity::Draw(){
	
}

void Entity::DrawSprite(){
	// Set position attributes for 2 triangles.
	// Set texture coordinate attributes for 2 triangles.
	// Bind the texture we want to use.
	// Draw arrays.
	// Disable attribute arrays.
	
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0f);
	modelMatrix.Rotate(rotation);
	

	//Pass the matrices to our program
	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	//Use the specified program id
	glUseProgram(program->programID);

	//Defines an array of vertex data (counter-clockwise!)
	float vertices[] = { width, height, -width, height, -width, -height, -width, -height, width, -height, width, height };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	//Enables a vertex attribute array
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, textureID);
	//Render previously defined arrays
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disables a vertex attribute array
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}