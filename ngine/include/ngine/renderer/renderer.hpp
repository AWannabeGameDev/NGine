#ifndef NG_RENDERER_HPP
#define NG_RENDERER_HPP

#include <stdexcept>

#include "ngine/gl_base/shader.hpp"
#include "ngine/gl_base/uniforms.hpp"
#include "ngine/renderer/image.hpp"
#include "ngine/renderer/ortho_camera.hpp"
#include "ngine/renderer/renderer_data.hpp"
#include "ngine/window/window.hpp"

namespace ng
{

// The associated window must live at least until the lifetime of the renderer
class Renderer
{
private :
    struct _DrawCmd
    {
        GLuint indexCount;
        GLuint instanceCount;
        GLuint indexOffset;
        GLint vertexOffset;
        GLuint instanceOffset;
    };

    static constexpr size_t _MAX_VERTICES {1024};
    static constexpr size_t _MAX_INDICES {4096};
    static constexpr size_t _MAX_INSTANCES {2048};

    static constexpr GLuint _BINDING_POINT_VERTICES {0};
    static constexpr GLuint _BINDING_POINT_INSTANCE {1};

    static constexpr GLuint _ATTRIB_IDX_POSITION {0};
    static constexpr GLuint _ATTRIB_IDX_UV {1};
    static constexpr GLuint _ATTRIB_IDX_TRANSFORM {2};
    static constexpr GLuint _ATTRIB_IDX_SAMPLE_BOTTOM_LEFT {6};
    static constexpr GLuint _ATTRIB_IDX_SAMPLE_WIDTH {7};
    static constexpr GLuint _ATTRIB_IDX_SAMPLE_HEIGHT {8};

    static constexpr GLint _SAMPLER_TEXTURE_UNIT {0};

    GLuint _vertexBuffer;
    GLuint _instanceVertexBuffer;
    GLuint _indexBuffer;
    GLuint _vertexArray;

    size_t _totalVertexCount {0};
    size_t _totalIndexCount {0};

    GLuint _defaultShader {createShaderProgram({"ngine/src/shaders/default.vxs", "ngine/src/shaders/default.fms"})};

    Uniforms _uniforms {};

    Image _whiteImage {glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f}};
    Viewport _viewport {glm::vec2 {0.0f}, 2.0f, 2.0f};

public :
    // The initial viewport fills the window area.
    Renderer(const Window& window);

    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    const Viewport& viewport() const;

    Prefab newPrefab(const Vertex* vertices, size_t vertexCount, const Index* indices, size_t indexCount);

    void setCamera(const glm::mat4& cameraMatrix);
    void setViewport(const Window& window, const Viewport& newViewport);

    // Sets the viewport to fill the current window area.
    void resetViewport(const Window& window);

    void setGlobalTransform(const glm::mat4& transform);
    void resetGlobalTransform();

    void clear(const glm::vec4& color);
    void draw(const Prefab& prefab, const ModelData* modelDatas, size_t modelCount, const Material& material);
    void draw(const Prefab& prefab, const ModelData* modelDatas, size_t modelCount, const glm::vec4& color);
    void draw(const Prefab& prefab, const ModelData* modelDatas, size_t modelCount, const Image& image);
};

}

#endif