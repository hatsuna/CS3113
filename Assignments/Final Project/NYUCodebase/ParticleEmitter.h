#pragma once
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include "ShaderProgram.h"

#include <vector>

struct Color{
	float r;
	float g;
	float b;
	float a;
};

class Particle {
public:
	float position_x;
	float position_y;
	float velocity_x;
	float velocity_y;
	float lifetime;
	float size;
};

class ParticleEmitter {
	public:
		ParticleEmitter(unsigned int particleCount);
		ParticleEmitter();
		~ParticleEmitter();

		void Update(float elapsed); 
		void Render(ShaderProgram * program);
	
		float position_x;
		float position_y;
		float velocity_x;
		float velocity_y;
		float velocityDeviation_x;
		float velocityDeviation_y;
		float gravity_x;
		float gravity_y;

		float maxLifetime;
		Color startColor;
		Color endColor;
	
		std::vector<Particle> particles;
};