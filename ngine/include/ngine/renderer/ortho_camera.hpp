#ifndef NG_ORTHO_CAMERA_HPP
#define NG_ORTHO_CAMERA_HPP

#include <glm/vec3.hpp>

#include "ngine/util/transform.hpp"

namespace ng
{

// The part of the world the camera renders fills the entire viewport of the renderer.
class OrthographicCamera
{
public :
	glm::vec3 dimensions;
	Transform transform {};

	glm::mat4 generateMatrix() const;
};

}

#endif