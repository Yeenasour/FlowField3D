#ifndef ENGINE_FREECAM
#define ENGINE_FREECAM

#include <Engine/Camera.h>


class FreeCamera : public Camera
{
private:
	/* data */
public:
	FreeCamera(const glm::vec3& position, const glm::vec3& dir, const ViewFrustum& f);
	void setPos(const glm::vec3& position) override;
	void move(const glm::vec3& velocity) override;
	void lookAt(const glm::vec3& target) override;
	void rotate(float dp, float dy) override;
	void zoom(float zoomFactor) override;
};

#endif