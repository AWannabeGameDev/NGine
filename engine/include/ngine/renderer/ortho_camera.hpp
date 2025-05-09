#ifndef NG_ORTHO_CAMERA_HPP
#define NG_ORTHO_CAMERA_HPP

#include "ngine/renderer/glm_clip_control.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ng
{

class OrthographicCamera
{
public :
	float width;
	float height;
	float depth;

	glm::vec3 center;
	float rotation;
	float zoom;

	glm::mat4 generateMatrix() const;
};

}

#endif