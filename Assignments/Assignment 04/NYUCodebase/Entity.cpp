#include "Entity.h"

// Linear Interpolation
float lerp(float v0, float v1, float t){
	return (1.0 - t) * v0 + t * v1;
}

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
	friction_x = 0.75f;
	friction_y = -0.1f;
	gravity_x = 0.0f;
	gravity_y = -.5f;
}


// Assignment 04 WIP
/*When updating entities, loop through, updating each entity
if not static, update entity position
loop through and check if current entity is colliding with any other entity
do not check if colliding with self
only check if colliding with isStatic entities

REALIZE THAT OTHER BULLETS AND ENEMIES ARE CURRENTLY DYNAMIC
*/

//Fixed_TimeStep gets passed to each entity via elapsed
void Entity::Update(float elapsed, std::vector<Entity> entities){
	//loop through vector of entities to check collisions
	//adjust position and velocity(?) depending on collisions
	if (!isStatic){ //check if an object is dynamic (and needs to be updated)
		switch (entityType){
		case TYPE_PLAYER:			
			if (velocity_y != 0.0f){
				collidedBottom == false;
				collidedTop = false;
			}
			if (velocity_x != 0.0f){
				collidedLeft = false;
				collidedRight = false;
			}

			if (acceleration_y = 0.0f && collidedBottom == true){
				acceleration_y = 0.0;
				velocity_y = 0.0f;
			}
			else if (acceleration_y > 0){
				acceleration_y = -gravity_y;
			}
			else{
				acceleration_y = gravity_y;
			}

			velocity_x = lerp(velocity_x, 0.0f, elapsed * friction_x);
			velocity_y = lerp(velocity_y, 0.0f, elapsed * friction_y);

			velocity_x += acceleration_x * elapsed;
			velocity_y += acceleration_y * elapsed;

			//WIP Assigment 04 Penetration adjustments
			y += velocity_y * elapsed;
			//Checking collision with static entities on y axis
			for (int i = 0; i < entities.size(); i++){
				Entity * tempEntity = &(entities[i]);
				if (tempEntity->isStatic == true){
					if (collidesWith(tempEntity)){
						float penetration = fabs(fabs(y - tempEntity->y) - height / 2.0f - tempEntity->height / 2.0f);
						if (y > tempEntity->y){
							y += penetration + 0.0001;
							collidedBottom = true;
						}
						else{
							y -= penetration + 0.0001;
							collidedTop = true;
						}
						velocity_y = 0.0f;
					}
				}
			}
			x += velocity_x * elapsed;
			// Checking collision with static entities on x axis
			for (int i = 0; i < entities.size(); i++){
				Entity * tempEntity = &(entities[i]);
				if (tempEntity->isStatic == true){
					if (collidesWith(tempEntity)){
						float penetration = fabs(fabs(x - tempEntity->x) - width / 2.0f - tempEntity->width / 2.0f);
						if (x > tempEntity->x){
							x += penetration + 0.0001;
							collidedLeft = true;
						}
						else{
							x -= penetration + 0.0001;
							collidedRight = true;
						}
						velocity_x = 0.0f;
					}
				}
			}

			//check collision with sides
			if (x + 2 * width >= 3.5f && velocity_x > 0){
				x = 3.5f - 2 * width;
			}
			else if (x - 2 * width <= -3.5f && velocity_x < 0){
				x = -3.5f + 2 * width;
			}
			//check collision with top and bottom
			/*if (y + 2 * height >= 1.75f && velocity_y > 0){
				y = 1.75f - 2 * height;
			}*/
			else if (y - 2 * height <= -1.75f && velocity_y < 0){
				y = -1.75f + 2 * height;
			}

			//WIP Entity Interaction
			//check list of entities and do something based on their type
			//check collision with enemies
			for (int i = 0; i < entities.size(); i++){
				//check if player collides with a non-player entity
				if (collidesWith(&(entities[i]))){
					if (entities[i].entityType == TYPE_ENEMY){
						entities[i].visible = false;
						visible = false;
					}
				}
				else{
					// not intersecting
				}
			}
			break;
		case TYPE_ENEMY:
			x += velocity_x * elapsed;

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
			y += velocity_y * elapsed;

			for (int i = 0; i < entities.size(); i++){
				//check if bullet collides with an entity
				if (collidesWith(&(entities[i]))){
					//intersecting
					if (entities[i].entityType == TYPE_ENEMY){
						entities[i].visible = false;
						visible = false;
					}
				}
				else{
					// not intersecting
				}
			}
			break;
		}
	}
}
void Entity::Render(ShaderProgram * program){
	//using non-uniform sprite sheet
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0f);
	modelMatrix.Rotate(rotation);

	program->setModelMatrix(modelMatrix);
	sprite.Draw(program);
}

bool Entity::collidesWith(Entity * entity){
	if ((y - height * 2 >= entity->y + entity->height * 2) ||
		(y + height * 2 <= entity->y - entity->height * 2) ||
		(x - width * 2 >= entity->x + entity->width * 2) ||
		(x + width * 2 <= entity->x - entity->width * 2)){
		return false;
	}
	else {
		return true;
	}


}

Bullet::Bullet(){}

void Bullet::erase(){

}
