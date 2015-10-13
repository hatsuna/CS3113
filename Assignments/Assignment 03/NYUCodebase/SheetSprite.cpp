#include "SheetSprite.h"

SheetSprite::SheetSprite():
size(0.0f), textureID(0), u(0.0f), v(0.0f), width(0.0f), height(0.0f){}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v,
	float width, float height, float size) : 
	size(size), textureID(textureID), u(u), v(v), width(width), height(height){}


void SheetSprite::Draw() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texCoords[] = {
		u, v + height, 
		u + width, v, 
		u, v, 
		u + width, v,
		u, v + height,
		u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size, 
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size, 
		0.5f * size * aspect, 0.5f * size, 
		-0.5f * size * aspect, -0.5f * size, 
		0.5f * size * aspect, -0.5f * size };
	// draw our arrays 
}