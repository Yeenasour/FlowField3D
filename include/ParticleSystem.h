#ifndef FLOWFIELD_PARTICLESYSTEM
#define FLOWFIELD_PARTICLESYSTEM

#include <glm/glm.hpp>
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

class ParticleSystem
{
private:
	int n;
	GLuint VAO, quadVBO, instanceVBO, EBO;
	std::vector<Particle> particles;
	float randf();
	glm::vec3 generateRandomPosition();
	void initParticles();
public:
	ParticleSystem(int n);
	~ParticleSystem();
	void update(const VectorField& field, float dt);
	void Draw(Camera &camera, Shader &shaderProgram);
};

#endif