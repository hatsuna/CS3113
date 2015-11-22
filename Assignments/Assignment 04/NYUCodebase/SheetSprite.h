#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include "ShaderProgram.h"
#include "Matrix.h"

class SheetSprite{
	public:
		SheetSprite();
		SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);

		void Draw(ShaderProgram * program);

		float size;
		unsigned int textureID;
		float u;
		float v;
		float width;
		float height;
};

/*
The aspect ratio of the spritesheet images will have an effect
on the aspect ratio of each sprite if you are using
the sprite's UV width and height to figure out the aspect ratio
(UV coordinates are always 0.0-1.0 for both U and V
even when the image is not square, so when you have a non-square image, 
it distorts the normalized aspect ratio of each sprite).
*/