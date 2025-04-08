#ifndef FLOW_CAMERA
#define FLOW_CAMERA

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
private:
	bool targetLock;
	float yaw;
	float pitch;
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 target;
	ViewFrustum frustum;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, ViewFrustum frustum);
	~Camera();
	void lookAt(glm::vec3 target);
	void setPos(glm::vec3 newPos);
	void rotate(float dp, float dy);
	void setAspectRatio(float ratio);
	glm::mat4 getViewProjectionMatrix();
};

#endif