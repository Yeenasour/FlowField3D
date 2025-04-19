#include <Engine/Camera.h>
#include <glm/gtc/matrix_transform.hpp>


void Camera::updateMatricies()
{
	this->viewMatrix = glm::lookAt(pos, dir, glm::vec3(0, 1, 0));
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::setAspectRatio(float ratio)
{
	this->vf.aspectRatio = ratio;
	this->projectionMatrix = glm::perspective(vf.fov, vf.aspectRatio, vf.near, vf.far);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
	return viewProjectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}