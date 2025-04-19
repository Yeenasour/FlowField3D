#include <Engine/FreeCamera.h>
#include <glm/gtc/matrix_transform.hpp>


FreeCamera::FreeCamera(const glm::vec3& position, const glm::vec3& dir, const ViewFrustum& f)
{
	this->pitch = asinf(dir.y);
	this->yaw = atan2f(dir.x, dir.z);
	this->pos = position;
	this->dir = dir;
	this->vf = f;
	this->up = glm::vec3(0, 1, 0);
	glm::mat4 V = glm::lookAt(position, position + dir, up);
	glm::mat4 P = glm::perspective(f.fov, f.aspectRatio, f.near, f.far);
	glm::mat4 VP = P * V;
	this->viewMatrix = V;
	this->projectionMatrix = P;
	this->viewProjectionMatrix = VP;
}

void FreeCamera::setPos(const glm::vec3& position)
{
	this->pos = position;
	this->viewMatrix = glm::lookAt(pos, pos + dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void FreeCamera::move(const glm::vec3& velocity)
{
	this->pos += velocity * glm::mat3(viewMatrix);
	//glm::vec3 forward = glm::normalize(dir);
    //glm::vec3 right = glm::normalize(glm::cross(forward, up));
    //glm::vec3 upCorrected = glm::normalize(glm::cross(right, forward));
    //pos += velocity.x * right +
    //       velocity.y * upCorrected +
    //       velocity.z * -forward;
	this->viewMatrix = glm::lookAt(pos, pos + dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void FreeCamera::lookAt(const glm::vec3& target)
{
	glm::vec3 newDir = glm::normalize(target - pos);
	this->dir = newDir;
	this->viewMatrix = glm::lookAt(pos, pos + dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void FreeCamera::rotate(float dp, float dy)
{
	float nPitch = this->pitch - dp;
	if (nPitch > -1.5533 && nPitch < 1.5533)
		this->pitch = nPitch;
	this->yaw += dy;

	glm::vec3 newDir = glm::vec3(0.0f);
	newDir.x = cos(pitch) * sin(yaw);
	newDir.y = sin(pitch);
	newDir.z = cos(pitch) * cos(yaw);

	this->dir = glm::normalize(newDir);

	this->viewMatrix = glm::lookAt(pos, pos + dir, up);
	this->viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void FreeCamera::zoom(float zoomFactor)
{
	return;
}