#include <OrbitalCamera.h>
#include <glm/gtc/matrix_transform.hpp>


OrbitalCamera::OrbitalCamera(const glm::vec3& position, const glm::vec3& target, const ViewFrustum& f)
{
	glm::vec3 offset = position - target;
	float distance = glm::length(offset);
	this->pitch = asinf(offset.y / distance);
	this->yaw = atan2f(offset.x, offset.z);
	this->pos = position;
	this->dir = target;
	this->vf = f;
	this->up = glm::vec3(0, 1, 0);
	glm::mat4 V = glm::lookAt(position, target, up);
	glm::mat4 P = glm::perspective(f.fov, f.aspectRatio, f.near, f.far);
	glm::mat4 VP = P * V;
	this->viewMatrix = V;
	this->projectionMatrix = P;
	this->viewProjectionMatrix = VP;
}

void OrbitalCamera::setPos(const glm::vec3& position)
{
	this->pos = pos;
	this->viewMatrix = glm::lookAt(pos, dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrbitalCamera::move(const glm::vec3& velocity)
{
	return;
}

void OrbitalCamera::lookAt(const glm::vec3& target)
{
	this->dir = target;
	this->viewMatrix = glm::lookAt(pos, dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrbitalCamera::rotate(float dp, float dy)
{
	float nPitch = this->pitch + dp;
	if (nPitch > -1.5533 && nPitch < 1.5533)
		this->pitch = nPitch;
	this->yaw += dy;

	float r = glm::length(pos - dir);

	pos.x = dir.x + r * cos(pitch) * sin(yaw);
	pos.y = dir.y + r * sin(pitch);
	pos.z = dir.z + r * cos(pitch) * cos(yaw);

	this->viewMatrix = glm::lookAt(pos, dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrbitalCamera::zoom(float zoomFactor)
{
	float r = glm::length(pos - dir);

	if ((r < 1.0f && zoomFactor < 1.0f) || 
		(r > 50.0f && zoomFactor > 1.0f)) return;

	r *= zoomFactor;

	pos.x = dir.x + r * cos(pitch) * sin(yaw);
	pos.y = dir.y + r * sin(pitch);
	pos.z = dir.z + r * cos(pitch) * cos(yaw);

	this->viewMatrix = glm::lookAt(pos, dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}