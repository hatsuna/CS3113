#include "Entity.h"

Entity::Entity(){
	// WIP figure out width and height of entity
	width = 0.0f;
	height = 0.0f;
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
	/*
Entity::Entity(const char *image_path){
	
	textureID = LoadTexture(image_path);
	
	
}
*/
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
	
	//loop through vector of entities to check collisions
	//adjust position and velocity(?) depending on collisions

	switch (entityType){
	case TYPE_PLAYER:
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

		//check collision with enemies
		for (int i = 0; i < entities.size(); i++){
			//check if player collides with a non-player entity
			if ((y - height * 2 >= entities[i].y + entities[i].height * 2) ||
				(y + height * 2 <= entities[i].y - entities[i].height * 2) ||
				(x - width * 2 >= entities[i].x + entities[i].width * 2) ||
				(x + width * 2 <= entities[i].x - entities[i].width * 2)){
				//if true, then not intersecting
			}
			else{
				//intersecting
				if (entities[i].entityType == TYPE_ENEMY){
					entities[i].visible = false;
					visible = false;
				}
			}
		}
		break;
	case TYPE_ENEMY:
		x += velocity_x * fixedElapsed;

		//check collision with sides
		if (x + 2 * width >= 3.5f){
			y -= height * 4;
			x = 3.0f;
			velocity_x = -velocity_x;
		}
		else if (x - 2 * width <= -3.5){
			y -= height * 4;
			x = -3.0f;
			velocity_x = -velocity_x;
		}

		break;
	case TYPE_BULLET:
		//	Bullet::timeAlive += fixedElapsed;
		//x += sin(rotation) * speed * elapsed;
		y += speed * fixedElapsed;

		for (int i = 0; i < entities.size(); i++){
			//check if bullet collides with an entity
			if ((y - height * 2 >= entities[i].y + entities[i].height * 2) ||
				(y + height * 2 <= entities[i].y - entities[i].height * 2) ||
				(x - width * 2 >= entities[i].x + entities[i].width * 2) ||
				(x + width * 2 <= entities[i].x - entities[i].width * 2)){
				//if true, then not intersecting
			}
			else{
				//intersecting
				if (entities[i].entityType == TYPE_ENEMY){
					entities[i].visible = false;
					visible = false;
					
				}
			}
		}
	break;
	}
}
void Entity::Render(){
	//call according draw call depending on texture

	//using non-uniform sprite sheet
	Draw();
}


Bullet::Bullet(){}

void Bullet::erase(){

}
