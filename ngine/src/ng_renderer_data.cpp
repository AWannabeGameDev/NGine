#include "ngine/renderer/renderer_data.hpp"

ng::Prefab::Prefab(GLuint indexCount, GLuint indexOffset, GLint vertexOffset) :
	_indexCount {indexCount}, _indexOffset {indexOffset}, _vertexOffset {vertexOffset}
{}

ng::Prefab::Prefab()
{}