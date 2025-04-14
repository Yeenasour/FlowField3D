#ifndef FLOWFIELD_CAMERA
#define FLOWFIELD_CAMERA

#include <glm/glm.hpp>


struct ViewFrustum
{
	float fov;
	float aspectRatio;
	float near;
	float far;
};

class Camera
{
protected:
	float pitch;
	float yaw;
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;
	ViewFrustum vf;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	void updateMatricies();
public:
	virtual ~Camera() {};
	virtual void setPos(const glm::vec3& pos) = 0;
	virtual void move(const glm::vec3& velocity) = 0;
	virtual void lookAt(const glm::vec3& target) = 0;
	virtual void rotate(float dp, float dy) = 0;
	virtual void zoom(float zoomFactor) = 0;
	void setAspectRatio(float ratio);
	glm::mat4 getViewProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
};

#endif