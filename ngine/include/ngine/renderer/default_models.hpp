#ifndef NG_DEFAULT_MODELS_HPP
#define NG_DEFAULT_MODELS_HPP

#include <array>

#include "ngine/renderer/renderer_data.hpp"

namespace ng
{

// A unit quad centered at origin in the xy-plane
inline constexpr std::array<Vertex, 4> quadVertices
{
	Vertex {glm::vec3 {-0.5f, -0.5f, 0.0f}, glm::vec2 {0.0f, 0.0f}},
	Vertex {glm::vec3 {-0.5f, 0.5f, 0.0f}, glm::vec2 {0.0f, 1.0f}},
	Vertex {glm::vec3 {0.5f, 0.5f, 0.0f}, glm::vec2 {1.0f, 1.0f}},
	Vertex {glm::vec3 {0.5f, -0.5f, 0.0f}, glm::vec2 {1.0f, 0.0f}}
};

// A unit quad centered at origin in the xy-plane
inline constexpr std::array<Index, 6> quadIndices
{
	0, 3, 2, 1, 0, 2
};

}

#endif