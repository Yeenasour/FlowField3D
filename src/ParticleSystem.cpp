#include <ParticleSystem.h>
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>
#include <VectorField.h>
#include <Shader.h>
#include <Camera.h>


ParticleSystem::ParticleSystem()
{
	initParticles();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	const float quad[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f,
	};

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, lifetime)));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &instanceVBO);
}

void ParticleSystem::initParticles()
{
	std::srand(std::time({}));
	this->particles.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		glm::vec3 pos = generateRandomPosition();
		glm::vec3 vel = glm::vec3(0.0f);
		float lifetime = ((float)i / (float)50);
		particles.push_back({pos, vel, lifetime});
	}
}

float ParticleSystem::randf()
{
	constexpr float bound = 5.0f;
	float r = ((float)std::rand() / (float)RAND_MAX);
	return (2 * r - 1) * bound;
}

glm::vec3 ParticleSystem::generateRandomPosition()
{
	float x = randf();
	float y = randf();
	float z = randf();
	return glm::vec3(x, y ,z);
}

void ParticleSystem::update(const VectorField& field, float dt)
{
	for (auto &&p : particles)
	{
		if (p.lifetime < 0)
		{
			p.pos = generateRandomPosition();
			p.vel.x = 0;
			p.vel.y = 0;
			p.vel.z = 0;
			p.lifetime = 2;
		}

		glm::vec3 force = field.evalAt(p.pos.x, p.pos.y, p.pos.z);
		
		p.vel += force * dt;
		p.pos += p.vel * dt;

		p.lifetime -= dt;
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * particles.size(), particles.data());
	glBindVertexArray(0);
}

void ParticleSystem::Draw(Camera &camera, Shader &shaderProgram)
{
	shaderProgram.use();
	glm::mat4 V = camera.getViewMatrix();
	glm::mat4 P = camera.getProjectionMatrix();

	shaderProgram.setUniform4fv("viewMatrix", &V[0][0]);
	shaderProgram.setUniform4fv("projectionMatrix", &P[0][0]);

	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 100);
	glBindVertexArray(0);
}