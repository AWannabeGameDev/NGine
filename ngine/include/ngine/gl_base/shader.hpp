#ifndef NG_SHADER_HPP
#define NG_SHADER_HPP

#include <glad/glad.h>

#include <string_view>
#include <initializer_list>

namespace ng
{

unsigned int compileShader(GLenum type, std::string_view path);
unsigned int createShaderProgram(const std::initializer_list<std::string_view>& shaderPaths);

}

#endif