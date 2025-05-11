#include <ParticleSystem.h>
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>
#include <VectorField.h>
#include <Engine/Shader.h>
#include <Engine/ShaderUtils.h>
#include <Engine/Buffer.h>
#include <Engine/Camera.h>


ParticleSystem::ParticleSystem(unsigned int n, float t)
	: n(n), maxLifetime(t), Renderable()
{
	initParticles();

	VAO->bind();
	
	const float quad[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
	};

	VAO->addVBO((VertexBuffer*) new StaticVertexBuffer(quad, sizeof(quad)));
	VAO->setAttribPointer(0, 2, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 0, (void*)0);
	VAO->enableAttribPointer(0);

	//VAO->addVBO((VertexBuffer*) new DynamicVertexBuffer(particles.data(), particles.size()*sizeof(Particle)));
	VAO->addVBO((VertexBuffer*) new DynamicVertexBuffer(nullptr, 1000*sizeof(Particle)));
	VAO->getVBO(0)->subData(particles.data(), particles.size()*sizeof(Particle));
	VAO->setAttribPointer(1, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, sizeof(Particle), (void*)0);
	VAO->enableAttribPointer(1);
	VAO->setAttribDivisor(1, 1);
	VAO->setAttribPointer(2, 1, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, sizeof(Particle), (void*)(offsetof(Particle, age)));
	VAO->enableAttribPointer(2);
	VAO->setAttribDivisor(2, 1);

	unsigned int indecies[] = {
		0, 1, 2,
		1, 3, 2
	};

	VAO->setEBO(new IndexBuffer(indecies, sizeof(indecies)));

	VAO->unbind();
}

ParticleSystem::~ParticleSystem()
{
	
}

void ParticleSystem::initParticles()
{
	std::srand(std::time({}));
	this->particles.reserve(1000);
	for (int i = 0; i < n; i++)
	{
		glm::vec3 pos = generateRandomPosition();
		glm::vec3 vel(0.0f);
		float offset = maxLifetime * i / n;
		particles.push_back({pos, vel, offset});
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
		if (p.age > maxLifetime)
		{
			p.pos = generateRandomPosition();
			p.vel.x = 0;
			p.vel.y = 0;
			p.vel.z = 0;
			p.age = 0;
		}

		glm::vec3 force = field.evalAt(p.pos.x, p.pos.y, p.pos.z);
		
		float t = 0.5f * dt;
		p.vel = p.vel * (1 - t) + force * t;
		p.pos += p.vel * dt;

		p.age += dt;
	}
	
	VAO->bind();
	VAO->getVBO(1)->bind();
	VAO->getVBO(1)->subData(particles.data(), particles.size() * sizeof(Particle));
	VAO->unbind();
}

void ParticleSystem::setParticleCount(int count)
{
	/*int diff = count - n;
	if (diff == 0) return;
	float particleDelta = maxLifetime / (float) count;
	if (diff < 0) particleDelta *= -1.0f;
	float lifeScale = (float) n / (float) count;
	float prevAge = -1.0f;
	bool nonOrdered = (particles.at(0).age > particles.back().age);
	for (auto &&p : particles)
	{
		p.age *= lifeScale;
		float copy = p.age;
		if (nonOrdered && p.age > prevAge)
		{
			p.age += particleDelta;
			prevAge = copy;
		}
	}
	if (diff < 0)
	{
		for (int i = 0; i < -diff; i++)
		{
			particles.pop_back();
		}
	}
	else
	{
		float lastAge = particles.back().age;
		for (int i = 1; i <= diff; i++)
		{
			glm::vec3 pos = generateRandomPosition();
			glm::vec3 vel(0.0f);
			particles.push_back({pos, vel, lastAge + i * particleDelta});
		}
	}*/
	int diff = count - n;
	if (diff == 0) return;
	int i = 0;
	float particleDelta = maxLifetime / (float) count;
	for (auto &&p : particles)
	{
		p.age = i++ * particleDelta;
	}
	if (diff < 0)
	{
		for (int i = 0; i < -diff; i++)
		{
			particles.pop_back();
		}
	}
	else
	{
		float lastAge = particles.back().age;
		for (int i = 1; i <= diff; i++)
		{
			glm::vec3 pos = generateRandomPosition();
			glm::vec3 vel(0.0f);
			particles.push_back({pos, vel, lastAge + i * particleDelta});
		}
	}
	n = count;
}

void ParticleSystem::setLifetime(float t)
{
	float lifeScale = t / maxLifetime;
	for (auto &&p : particles)
	{
		p.age *= lifeScale;
	}
	maxLifetime = t;
}

unsigned int ParticleSystem::getCount() const
{
	return n;
}