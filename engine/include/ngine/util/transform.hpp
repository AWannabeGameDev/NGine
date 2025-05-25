#ifndef NG_TRANSFORM_HPP
#define NG_TRANSFORM_HPP

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ng
{

class Transform
{
private :
	glm::quat eulerYXZtoQuaternion(float yaw, float pitch, float roll);

public:
	glm::vec3 position {0.0f};
	glm::quat rotation {0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec3 scale {1.0f};

	// applied in the specified order : yaw-pitch-roll
	// +ve yaw -> right, +ve pitch -> down, +ve roll -> CCW (in a left-handed system, as used by NGine)
	void rotateLocal(float yaw, float pitch, float roll);
	void rotateGlobal(float yaw, float pitch, float roll);

	glm::mat4 generateMatrix();
};

}

#endif