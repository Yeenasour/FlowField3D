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
	float lifetime;
};

class ParticleSystem : public Renderable
{
private:
	unsigned int n;
	std::vector<Particle> particles;
	float randf();
	glm::vec3 generateRandomPosition();
	void initParticles();
public:
	ParticleSystem(unsigned int n);
	~ParticleSystem();
	void update(const VectorField& field, float dt);
	unsigned int getCount() const;
};

#endif