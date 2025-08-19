#ifndef NG_TRANSFORM_HPP
#define NG_TRANSFORM_HPP

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ng
{

// NGine's world-space uses the left-handed coordinate system i.e. +x is right, +y is up and +z is forward.
class Transform
{
private :
	glm::quat _eulerYXZtoQuaternion(float yaw, float pitch, float roll);

public:
	glm::vec3 position {0.0f};
	glm::quat rotation {1.0f, 0.0f, 0.0f, 0.0f};
	glm::vec3 scale {1.0f};

	// applied in the specified order : yaw-pitch-roll
	// +ve yaw -> right, +ve pitch -> down, +ve roll -> CCW (when +x is right, +y is up and +z is forward)
	// note that this is a left-handed system, as used by NGine
	// more generally (for any coordinate system), +yaw sends +z to +x, +pitch sends +y to +z, and +roll sends +x to +y
	void rotateLocal(float yaw, float pitch, float roll);
	void rotateGlobal(float yaw, float pitch, float roll);

	glm::mat4 generateMatrix() const;
};

}

#endif