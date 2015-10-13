#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif



class SheetSprite{
	public:
		SheetSprite();
		SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);

		void Draw();

		float size;
		unsigned int textureID;
		float u;
		float v;
		float width;
		float height;
};