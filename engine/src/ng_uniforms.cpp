#include "ngine/gl_base/uniforms.hpp"

std::size_t ng::Uniforms::_ShaderUniformPairHash::operator()(const RegularPair& key) const noexcept
{
    return _hashPair(key.first, key.second);
}

std::size_t ng::Uniforms::_ShaderUniformPairHash::operator()(const ViewPair& key) const noexcept
{
    return _hashPair(key.first, key.second);
}

bool ng::Uniforms::_ShaderUniformPairEquals::operator()(const RegularPair& pair1, const RegularPair& pair2) const noexcept
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

bool ng::Uniforms::_ShaderUniformPairEquals::operator()(const ViewPair& pair1, const ViewPair& pair2) const noexcept
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

void ng::Uniforms::addUniform(GLuint shaderId, std::string_view name)
{
    if(_locations.contains(std::make_pair(shaderId, name)))
    {
        return;
    }

    _locations.try_emplace(std::make_pair(shaderId, std::string {name}), glGetUniformLocation(shaderId, name.data()));
}

GLint ng::Uniforms::_getLocation(GLuint shaderId, std::string_view name)
{
    auto it {_locations.find(std::make_pair(shaderId, name))};

    if(it == _locations.end())
    {
        throw std::out_of_range {"This shader-id pair does not exist."};
    }

    return it->second;
}

void ng::Uniforms::setUniform(GLuint shaderId, std::string_view name, const glm::mat4& mat4)
{
    glProgramUniformMatrix4fv(shaderId, _getLocation(shaderId, name), 1, GL_FALSE, glm::value_ptr(mat4));
}

void ng::Uniforms::setUniform(GLuint shaderId, std::string_view name, const glm::vec4& vec4)
{
    glProgramUniform4fv(shaderId, _getLocation(shaderId, name), 1, glm::value_ptr(vec4));
}

void ng::Uniforms::setUniform(GLuint shaderId, std::string_view name, const glm::vec2& vec2)
{
    glProgramUniform2fv(shaderId, _getLocation(shaderId, name), 1, glm::value_ptr(vec2));
}

void ng::Uniforms::setBindlessHandle(GLuint shaderId, std::string_view name, GLuint64 handle)
{
    glProgramUniformHandleui64vARB(shaderId, _getLocation(shaderId, name), 1, &handle);
}