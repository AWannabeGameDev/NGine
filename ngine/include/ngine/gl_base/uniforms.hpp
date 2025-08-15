#ifndef NG_UNIFORMS_HPP
#define NG_UNIFORMS_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <stdexcept>

namespace ng
{

class Uniforms
{
private:
    using RegularPair = std::pair<GLuint, std::string>;
    using ViewPair = std::pair<GLuint, std::string_view>;

    class _ShaderUniformPairHash
    {
    private:
        template <typename T1, typename T2>
        std::size_t _hashPair(const T1& key1, const T2& key2) const noexcept
        {
            std::hash<T1> hash1;
            std::hash<T2> hash2;

            std::size_t seed = hash1(key1);
            seed ^= hash2(key2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }

    public:
        // enable heterogeneous lookup
        using is_transparent = void;

        std::size_t operator()(const RegularPair& key) const noexcept;
        std::size_t operator()(const ViewPair& key) const noexcept;
    };

    class _ShaderUniformPairEquals
    {
    public :
        // enable heterogeneous lookup
        using is_transparent = void;

        bool operator()(const RegularPair& pair1, const RegularPair& pair2) const noexcept;
        bool operator()(const ViewPair& pair1, const ViewPair& pair2) const noexcept;
    };

    std::unordered_map<RegularPair, GLint, _ShaderUniformPairHash, _ShaderUniformPairEquals> _locations {};

    GLint _getLocation(GLuint shaderId, std::string_view name);

public:
    Uniforms() = default;

    void addUniform(GLuint shaderId, std::string_view name);

    // Uniforms must be registered with addUniform before setting them.
    void setUniform(GLuint shaderId, std::string_view name, const glm::mat4& mat4);
    void setUniform(GLuint shaderId, std::string_view name, const glm::vec4& vec4);
    void setUniform(GLuint shaderId, std::string_view name, const glm::vec2& vec2);
    void setUniform(GLuint shaderId, std::string_view name, int integ);
};

}

#endif