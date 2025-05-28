#ifndef NG_RENDERER_DATA_HPP
#define NG_RENDERER_DATA_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "ngine/renderer/image.hpp"

namespace ng
{

using Index = unsigned int;

// Sampling via the given uv is done relative to the sampling window specified by ImageSampleData.
struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
};

class Prefab
{
    friend class Renderer;

private :
    const GLuint _indexCount;
    const GLuint _indexOffset;
    const GLint _vertexOffset;

    Prefab(GLuint indexCount, GLuint indexOffset, GLint vertexOffset);
};

struct ImageSampleData
{
    glm::uvec2 bottomLeft;
    unsigned int width;
    unsigned int height;
};

struct ModelData
{
    glm::mat4 transform;
    ImageSampleData sampleData;
};

struct Material
{
    Image image;
    glm::vec4 color;
};

// The coordinates and dimensions are in normalized window coordinates (verying in [-1, 1] on both axes)
struct Viewport
{
    glm::vec2 center;
    float width;
    float height;
};

}

#endif