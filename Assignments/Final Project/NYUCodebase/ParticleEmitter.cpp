#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(unsigned int particleCount){
	// ParticleEmitter values (x, y, velocity etc) need to be set
	// by object creating it 
	position_x = 0.0f;
	position_y = 0.0f;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	velocityDeviation_x = 0.0f;
	velocityDeviation_y = 0.0f;
	gravity_x = 0.0f;
	gravity_y = 0.0f;
	maxLifetime = 0.0f;

	// Make sure the particles' lifetime is set to a random value up to
	// the maxiumum lifetime, so they dont all reset together
	for (int i = 0; i < particleCount; i++){
		Particle particle;
		particle.lifetime = rand() % ((int)maxLifetime + 1);
		particles.push_back(particle);
	}
}
ParticleEmitter::ParticleEmitter(){
	position_x = 0.0f;
	position_y = 0.0f;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	velocityDeviation_x = 0.0f;
	velocityDeviation_y = 0.0f;
	gravity_x = 0.0f;
	gravity_y = 0.0f;
	maxLifetime = 0.0f;
}
ParticleEmitter::~ParticleEmitter(){

}

void ParticleEmitter::Update(float elapsed){
	for (int i = 0; i < particles.size(); i++){
		particles[i].lifetime += elapsed;
		particles[i].velocity_x += gravity_x * elapsed;
		particles[i].velocity_y += gravity_y * elapsed;
		particles[i].position_x += particles[i].velocity_x * elapsed;
		particles[i].position_y += particles[i].velocity_y * elapsed;

		if (particles[i].lifetime > maxLifetime){
			particles[i].velocity_x = velocity_x + velocityDeviation_x;
			particles[i].velocity_y = velocity_y + velocityDeviation_y;
			particles[i].position_x = position_x;
			particles[i].position_y = position_y;
			particles[i].lifetime = 0.0f;
		}
	}
}
void ParticleEmitter::Render(ShaderProgram * program){
	std::vector<float> particleColors;
	std::vector<float> vertices;
	std::vector<float> texCoords;
	
	float size = 0.01f; //lerp (startSize, endSize, m) + particles[i].sizeDeviation;

	for (int i = 0; i < particles.size(); i++) {
		vertices.insert(vertices.end(), { 
			particles[i].position_x - size, particles[i].position_y + size, 
			particles[i].position_x - size, particles[i].position_y - size,
			particles[i].position_x + size, particles[i].position_y + size,
			particles[i].position_x + size, particles[i].position_y + size,
			particles[i].position_x - size, particles[i].position_y - size, 
			particles[i].position_x + size, particles[i].position_y - size
		});

		texCoords.insert(texCoords.end(), { 
			0.0f, 0.0f, 
			0.0f, 1.0f, 
			1.0f, 0.0f,
			1.0f, 0.0f, 
			0.0f, 1.0f, 
			1.0f, 1.0f 
		});

		particleColors.push_back(startColor.r);
		particleColors.push_back(startColor.g);
		particleColors.push_back(startColor.b);
		particleColors.push_back(startColor.a);
		/*	float m = (particles[i].lifetime / maxLifetime);
		particleColors.push_back(lerp(startColor.r, endColor.r, m);
		particleColors.push_back(lerp(startColor.g, endColor.g, m));
		particleColors.push_back(lerp(startColor.b, endColor.b, m));
		particleColors.push_back(lerp(startColor.a, endColor.a, m));*/
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	GLuint colorAttribute = glGetAttribLocation(program->programID, "color");
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, 0, particleColors.data());
	glEnableVertexAttribArray(colorAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	
	glColor3f(0.5f, 0.1, 0.1);
	glDrawArrays(GL_POINT, 0, vertices.size() / 2);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

