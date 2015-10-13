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

	// WIP Spritesheet stuff probably need to re-implement LoadTexture
	// spriteSheetTexture = LoadTexture("sheet.png");
	// mySprite = SheetSprite(spriteSheetTexture, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2);

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

//WIP Assignment 03
void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY) {
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX; 
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY; 
	float spriteWidth = 1.0 / (float)spriteCountX; 
	float spriteHeight = 1.0 / (float)spriteCountY;
	GLfloat texCoords[] = { u, v + spriteHeight, u + spriteWidth, v, u, v,
		u + spriteWidth, v, u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight };
	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	// Add our regular sprite drawing
	// DrawSprite(); ???
}

//WIP Assignment 03
/*
void Animate(){
	animationElapsed += elapsed;
	if (animationElapsed > 1.0 / framesPerSecond) {
		currentIndex++; animationElapsed = 0.0;
		if (currentIndex > numFrames - 1) {
			currentIndex = 0;
		}
	}
	DrawSpriteSheetSprite(runAnimation[currentIndex], 8, 4);
}

void DrawText(int fontTexture, std::string text, float size, float spacing) {
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
*/