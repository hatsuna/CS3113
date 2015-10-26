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

	timeLeftOver = 0.0f;
}

GLuint Entity::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	//Creates a new OpenGL texture ID
	GLuint textureID;
	glGenTextures(1, &textureID);
	//Bind a texture to a texture target
	glBindTexture(GL_TEXTURE_2D, textureID);
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
	return textureID;
}

Entity::Entity(const char *image_path){
	
	textureID = LoadTexture(image_path);
	
	// WIP figure out width and height of entity
	width = 100.0f;
	height = 100.0f;
	rotation = 0.0f;

	x = 0.0f;
	y = 0.0f;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.7f;
	friction_y = 0.7f;

	timeLeftOver = 0.0f;
}

// Call if non-uniform sprite sheet is used
void Entity::Draw(){
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0f);
	modelMatrix.Rotate(rotation);

	program->setModelMatrix(modelMatrix);

	sprite.program = program;
	sprite.Draw();	
}

// Call if single texture is used
void Entity::DrawSprite(){
	// Set position attributes for 2 triangles.
	// Set texture coordinate attributes for 2 triangles.
	// Bind the texture we want to use.
	// Draw arrays.
	// Disable attribute arrays.
	
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0f);
	modelMatrix.Rotate(rotation);

	program->setModelMatrix(modelMatrix);

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

// Call if uniform sprite sheet is used
void Entity::DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY) {
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX; 
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY; 
	float spriteWidth = 1.0 / (float)spriteCountX; 
	float spriteHeight = 1.0 / (float)spriteCountY;
	GLfloat texCoords[] = { u, v + spriteHeight, u + spriteWidth, v, u, v,
		u + spriteWidth, v, u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight };
	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	// Add our regular sprite drawing
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0f);
	modelMatrix.Rotate(rotation);
	program->setModelMatrix(modelMatrix);
	//Use the specified program id
	glUseProgram(program->programID);

	//Defines an array of vertex data (counter-clockwise!)
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	//Enables a vertex attribute array
	glEnableVertexAttribArray(program->positionAttribute);
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
//void Entity::Animate(){
/*animationElapsed += elapsed;
	if (animationElapsed > 1.0 / framesPerSecond) {
		currentIndex++; animationElapsed = 0.0;
		if (currentIndex > numFrames - 1) {
			currentIndex = 0;
		}
	}
	DrawSpriteSheetSprite(runAnimation[currentIndex], 8, 4);
}*/

//WIP Assignment 03 Add Velocity and Acceleration to Entity class
//WIP Assignment 03 Add Friction "Rate of decrease in velocity"
//WIP lerp

// WIP Assignment03 Fixed update for physics and collision detection
void Entity::Update(float elapsed, std::vector<Entity> entities){
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;

	//apply acceleration, velocity, and position changes according to fixed elapsed
	//using the friction number as a hack cutoff for when it just stops moving
	if (velocity_x > 0 && acceleration_x == 0.0f){
		acceleration_x = -friction_x;
	}
	else if (velocity_x < 0 && acceleration_x == 0.0f){
		acceleration_x = friction_x;
	}
	
	if ((velocity_x > 0 && acceleration_x * fixedElapsed < -(velocity_x)) ||
		(velocity_x < 0 && acceleration_x * fixedElapsed > -(velocity_x))){
		acceleration_x = 0.0f;
		velocity_x = 0.0f;
	}
	else{
		velocity_x += acceleration_x * fixedElapsed;
	}
	x += velocity_x * fixedElapsed;

	//check collision with sides
	if (x + 2 * width >= 3.5f && velocity_x > 0){
		x = 3.5f - 2 * width;
	}
	else if (x - 2 * width <= -3.5f && velocity_x < 0){
		x = -3.5f + 2 * width;
	}


	//loop through vector of entities to check collisions
	//adjust position and velocity(?) depending on collisions
	
}

void Entity::Render(){
	//call according draw call depending on texture

	//using non-uniform sprite sheet
	Draw();
}