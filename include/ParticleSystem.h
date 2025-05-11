#ifndef FLOWFIELD_PARTICLESYSTEM
#define FLOWFIELD_PARTICLESYSTEM

#include <glm/glm.hpp>
#include <Engine/Renderable.h>
#include <vector>


typedef unsigned int GLuint;
class Camera;
class Shader;
class VectorField;

struct Particle
{
	glm::vec3 pos;
	glm::vec3 vel;
	float age;
};

class ParticleSystem : public Renderable
{
private:
	unsigned int n;
	float maxLifetime;
	std::vector<Particle> particles;
	float randf();
	glm::vec3 generateRandomPosition();
	void initParticles();
public:
	ParticleSystem(unsigned int n, float t);
	~ParticleSystem();
	void update(const VectorField& field, float dt);
	void setParticleCount(int count);
	void setLifetime(float t);
	unsigned int getCount() const;
};

#endif