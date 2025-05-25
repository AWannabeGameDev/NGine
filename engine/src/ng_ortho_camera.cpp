#include "ngine/util/glm_clip_control.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ngine/renderer/ortho_camera.hpp"

glm::mat4 ng::OrthographicCamera::generateMatrix() const
{
	glm::mat4 projection {glm::ortho(-dimensions.x / 2 / transform.scale.x, dimensions.x / 2 / transform.scale.x, 
									 -dimensions.y / 2 / transform.scale.y, dimensions.y / 2 / transform.scale.y,
									 0.0f, dimensions.z)};

	glm::mat4 view {1.0f};
	view = view * glm::mat4_cast(glm::conjugate(transform.rotation));
	view = glm::translate(view, -transform.position);

	return projection * view;
}