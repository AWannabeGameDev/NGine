#include <glad/glad.h>

#include "ngine/renderer/renderer.hpp"
#include "ngine/window/window.hpp"

ng::Renderer::Renderer(const Window& window)
{
    #ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    #endif

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glViewport(0, 0, window.width(), window.height());
    glEnable(GL_DEPTH_TEST);

    glCreateVertexArrays(1, &_vertexArray);

    glCreateBuffers(1, &_vertexBuffer);
    glNamedBufferData(_vertexBuffer, _MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glVertexArrayVertexBuffer(_vertexArray, _BINDING_POINT_VERTICES, _vertexBuffer, 0, sizeof(Vertex));

    glCreateBuffers(1, &_indexBuffer);
    glNamedBufferData(_indexBuffer, _MAX_INDICES * sizeof(Index), nullptr, GL_DYNAMIC_DRAW);
    glVertexArrayElementBuffer(_vertexArray, _indexBuffer);

    glCreateBuffers(1, &_instanceVertexBuffer);
    glNamedBufferData(_instanceVertexBuffer, _MAX_INSTANCES * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);
    glVertexArrayVertexBuffer(_vertexArray, _BINDING_POINT_INSTANCE, _instanceVertexBuffer, 0, sizeof(ModelData));
    glVertexArrayBindingDivisor(_vertexArray, _BINDING_POINT_INSTANCE, 1);

    glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_POSITION, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_POSITION, _BINDING_POINT_VERTICES);
    glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_POSITION);

    glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_UV, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
    glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_UV, _BINDING_POINT_VERTICES);
    glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_UV);

    for(GLuint i {0}; i < 4; i++)
    {
        glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_TRANSFORM + i, 4, GL_FLOAT, GL_FALSE, 
                             offsetof(ModelData, transform) + (i * sizeof(glm::vec4)));
        glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_TRANSFORM + i, _BINDING_POINT_INSTANCE);
        glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_TRANSFORM + i);
    }

    glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_SAMPLE_BOTTOM_LEFT, 2, GL_INT, GL_FALSE,
                               offsetof(ModelData, sampleData.bottomLeft));
    glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_SAMPLE_BOTTOM_LEFT, _BINDING_POINT_INSTANCE);
    glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_SAMPLE_BOTTOM_LEFT);

    glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_SAMPLE_WIDTH, 1, GL_INT, GL_FALSE,
                               offsetof(ModelData, sampleData.width));
    glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_SAMPLE_WIDTH, _BINDING_POINT_INSTANCE);
    glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_SAMPLE_WIDTH);

    glVertexArrayAttribFormat(_vertexArray, _ATTRIB_IDX_SAMPLE_HEIGHT, 1, GL_INT, GL_FALSE,
                               offsetof(ModelData, sampleData.height));
    glVertexArrayAttribBinding(_vertexArray, _ATTRIB_IDX_SAMPLE_HEIGHT, _BINDING_POINT_INSTANCE);
    glEnableVertexArrayAttrib(_vertexArray, _ATTRIB_IDX_SAMPLE_HEIGHT);

    glBindVertexArray(_vertexArray);

    _uniforms.addUniform(_defaultShader, "camera");
    _uniforms.addUniform(_defaultShader, "color");
    _uniforms.addUniform(_defaultShader, "sampler");
    _uniforms.addUniform(_defaultShader, "imageDimensions");
    _uniforms.addUniform(_defaultShader, "globalTransform");

    _uniforms.setUniform(_defaultShader, "globalTransform", glm::mat4 {1.0f});
}

ng::Renderer::~Renderer()
{
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
    glDeleteBuffers(1, &_instanceVertexBuffer);
    glDeleteVertexArrays(1, &_vertexArray);
    glDeleteProgram(_defaultShader);
}

const ng::Viewport& ng::Renderer::viewport() const
{
    return _viewport;
}

ng::Prefab ng::Renderer::newPrefab(const Vertex* vertices, size_t vertexCount, const Index* indices, size_t indexCount)
{
    Prefab ret {(GLuint)indexCount, (GLuint)_totalIndexCount, (GLint)_totalVertexCount};

    glNamedBufferSubData(_vertexBuffer, _totalVertexCount * sizeof(Vertex), vertexCount * sizeof(Vertex), vertices);
    _totalVertexCount += vertexCount;

    glNamedBufferSubData(_indexBuffer, _totalIndexCount * sizeof(Index), indexCount * sizeof(Index), indices);
    _totalIndexCount += indexCount;

    return ret;
}

void ng::Renderer::setCamera(const glm::mat4& cameraMatrix)
{
    _uniforms.setUniform(_defaultShader, "camera", cameraMatrix);
}

void ng::Renderer::setViewport(const Window& window, const Viewport& newViewport)
{
    _viewport = newViewport;

    glm::vec2 bottomLeft {_viewport.center - glm::vec2 {_viewport.width / 2.0f, _viewport.height / 2.0f}};
    glViewport((GLint)((bottomLeft.x + 1) / 2.0f * window.width()), 
               (GLint)((bottomLeft.y + 1) / 2.0f * window.height()),
               (GLsizei)(_viewport.width / 2.0f * window.width()),
               (GLsizei)(_viewport.height / 2.0f * window.height()));
}

void ng::Renderer::resetViewport(const Window& window)
{
    _viewport = Viewport {glm::vec2 {0.0f}, 2.0f, 2.0f};
    glViewport(0, 0, window.width(), window.height());
}

void ng::Renderer::setGlobalTransform(const glm::mat4& transform)
{
    _uniforms.setUniform(_defaultShader, "globalTransform", transform);
}

void ng::Renderer::resetGlobalTransform()
{
    _uniforms.setUniform(_defaultShader, "globalTransform", glm::mat4 {1.0f});
}

void ng::Renderer::clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ng::Renderer::addModelToBatch(const ModelData& model)
{
    if(_totalInstanceCount == _MAX_INSTANCES)
    {
        throw std::out_of_range {"Cannot exceed maximum number of instances per batch"};
    }

    _currentInstances[_totalInstanceCount] = model;
    _totalInstanceCount++;
}

void ng::Renderer::addModelsToBatch(const ModelData* models, size_t count)
{
    if(_totalInstanceCount > (_MAX_INSTANCES - count))
    {
        throw std::out_of_range {"Cannot exceed maximum number of instances per batch"};
    }

    std::copy(models, models + count, _currentInstances.data());
    _totalInstanceCount += count;
}

void ng::Renderer::drawAndResetBatch(const Prefab& prefab, const Material& material)
{
    glNamedBufferSubData(_instanceVertexBuffer, 0, _totalInstanceCount * sizeof(ModelData), _currentInstances.data());
    _uniforms.setUniform(_defaultShader, "color", material.color);
    _uniforms.setUniform(_defaultShader, "imageDimensions", glm::vec2 {material.image._width, material.image._height});

    glActiveTexture(GL_TEXTURE0 + _SAMPLER_TEXTURE_UNIT);
    glBindTexture(GL_TEXTURE_2D, material.image._textureRef->textureId);
    _uniforms.setUniform(_defaultShader, "sampler", _SAMPLER_TEXTURE_UNIT);

    glUseProgram(_defaultShader);
    glDrawElementsInstancedBaseVertex(GL_TRIANGLES, prefab._indexCount, GL_UNSIGNED_INT,
                                      (const void*)(prefab._indexOffset * sizeof(Index)), (GLsizei)_totalInstanceCount, 
                                      prefab._vertexOffset);

    _totalInstanceCount = 0;
}

void ng::Renderer::drawAndResetBatch(const Prefab& prefab, const glm::vec4& color)
{
    drawAndResetBatch(prefab, Material {_whiteImage, color});
}

void ng::Renderer::drawAndResetBatch(const Prefab& prefab, const Image& image)
{
    drawAndResetBatch(prefab, Material {image, glm::vec4 {1.0f}});
}