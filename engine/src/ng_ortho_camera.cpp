#include "ngine/renderer/ortho_camera.hpp"

glm::mat4 ng::OrthographicCamera::generateMatrix() const
{
	glm::mat4 projection {glm::ortho(-width / 2, width / 2, -height / 2, height / 2, 0.0f, depth)};

	glm::mat4 view {1.0f};
	view = glm::translate(view, -center);
	view = glm::rotate(view, -rotation, glm::vec3 {0.0f, 0.0f, -1.0f});
	view = glm::scale(view, glm::vec3 {zoom, zoom, 1.0f});

	return projection * view;
}