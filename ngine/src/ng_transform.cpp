#include "ngine/util/transform.hpp"

glm::quat ng::Transform::_eulerYXZtoQuaternion(float yaw, float pitch, float roll)
{
	// convert euler angles to quaternion directly, without intermediate quaternions 
	// doing it individually along each axis.
	// first yaw, then pitch, then roll.

	float cx {cos(pitch / 2.0f)};
	float cy {cos(yaw / 2.0f)};
	float cz {cos(roll / 2.0f)};

	float sx {sin(pitch / 2.0f)};
	float sy {sin(yaw / 2.0f)};
	float sz {sin(roll / 2.0f)};

	return glm::quat 
	{
		cx*cy*cz - sx*sy*sz,
		sx*cy*cz - cx*sy*sz,
		cx*sy*cz + sx*cy*sz,
		cx*cy*sz + sx*sy*cz
	};
}

void ng::Transform::rotateLocal(float yaw, float pitch, float roll)
{
	rotation = _eulerYXZtoQuaternion(yaw, pitch, roll) * rotation;
}

void ng::Transform::rotateGlobal(float yaw, float pitch, float roll)
{
	rotation = rotation * _eulerYXZtoQuaternion(yaw, pitch, roll);
}

glm::mat4 ng::Transform::generateMatrix() const
{
	glm::mat4 transform {1.0f};
	transform = glm::translate(transform, position);
	transform = transform * glm::mat4_cast(rotation);
	transform = glm::scale(transform, scale);

	return transform;
}