#ifndef FLOWFIELD_ORBITALCAMERA
#define FLOWFIELD_ORBITALCAMERA

#include <Camera.h>


class OrbitalCamera : public Camera
{
private:
	/* data */
public:
	OrbitalCamera(const glm::vec3& position, const glm::vec3& target, const ViewFrustum& f);
	void setPos(const glm::vec3& position) override;
	void lookAt(const glm::vec3& target) override;
	void rotate(float dp, float dy) override;
};

#endif