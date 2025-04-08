#include <Camera.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, ViewFrustum f)
{
	glm::vec3 offset = pos - target;
	float distance = glm::length(offset);
	this->pitch = asinf(offset.y / distance);
	this->yaw = atan2f(offset.x, offset.z);
	this->pos = pos;
	glm::vec3 dir = glm::normalize(offset);
	this->dir = dir;
	glm::vec3 right = glm::normalize(glm::cross(worldUp, dir));
	this->right = right;
	this->up = glm::cross(dir, right);
	this->target = target;
	this->frustum = f;
	glm::mat4 V = glm::lookAt(pos, target, up);
	glm::mat4 P = glm::perspective(f.fov, f.aspectRatio, f.near, f.far);
	glm::mat4 VP = P * V;
	this->viewMatrix = V;
	this->projectionMatrix = P;
	this->viewProjectionMatrix = VP;
	this->targetLock = true;
}

Camera::~Camera() {
    
}

void Camera::lookAt(glm::vec3 target)
{
	this->target = target;
	this->viewMatrix = glm::lookAt(pos, target, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::setPos(glm::vec3 pos)
{
	this->pos = pos;
	this->viewMatrix = glm::lookAt(pos, target, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::rotate(float dp, float dy)
{
	float nPitch = this->pitch + dp;
	if (nPitch > -1.5533 && nPitch < 1.5533)
		this->pitch = nPitch;
	this->yaw += dy;

	if (targetLock)
	{
		float r = glm::length(pos - target);

		pos.x = target.x + r * cos(pitch) * sin(yaw);
		pos.y = target.y + r * sin(pitch);
		pos.z = target.z + r * cos(pitch) * cos(yaw);
	}
	this->viewMatrix = glm::lookAt(pos, target, glm::vec3(0,1,0));
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::setAspectRatio(float ratio)
{
	this->frustum.aspectRatio = ratio;
	this->projectionMatrix = glm::perspective(frustum.fov, frustum.aspectRatio, frustum.near, frustum.far);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
	return viewProjectionMatrix;
}