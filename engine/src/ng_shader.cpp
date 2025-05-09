#include <glad/glad.h>

#include <stdio.h> 
#include <fstream>
#include <sstream>
#include <memory>

#include "ngine/gl_base/shader.hpp"

unsigned int ng::compileShader(GLenum type, std::string_view path)
{
	std::ifstream file {};
	file.open(path.data());
	std::stringstream sstream {};
	std::string source {};

	if(!file.is_open())
	{
		printf("Failed to open shader file %s.\n", path.data());
		return 0;
	} 
	else
	{
		sstream << file.rdbuf();
		source = sstream.str();
	}

	const char* sourceStr {source.c_str()};

	GLuint shader {glCreateShader(type)};
	glShaderSource(shader, 1, &sourceStr, nullptr);
	glCompileShader(shader);

	#ifdef _DEBUG

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(success != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::unique_ptr<GLchar> message {std::make_unique<GLchar>(logLength)};
		glGetShaderInfoLog(shader, logLength, nullptr, message.get());

		printf("Failed to compile shader %s : %s", path.data(), message.get());

		return 0;
	}

	#endif

	return shader;
}

unsigned int ng::createShaderProgram(const std::initializer_list<std::string_view>& shaderPaths)
{
	GLuint program {glCreateProgram()};

	for(const auto& path : shaderPaths)
	{
		std::string_view extension {path.substr(path.size() - 3, 3)};
		int extensionHash {extension[0] + (2 * extension[1]) + (3 * extension[2])};

		GLenum shaderType;

		switch(extensionHash)
		{
			case 'v' + (2 * 'x') + (3 * 's') : shaderType = GL_VERTEX_SHADER; break;
			case 'g' + (2 * 'm') + (3 * 's') : shaderType = GL_GEOMETRY_SHADER; break;
			case 'f' + (2 * 'm') + (3 * 's') : shaderType = GL_FRAGMENT_SHADER; break;
			case 'c' + (2 * 'p') + (3 * 's') : shaderType = GL_COMPUTE_SHADER; break;
			default : throw std::invalid_argument {"Shader extension not recognized."};
		}

		GLuint shader {compileShader(shaderType, path)};
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}

	glLinkProgram(program);

	#ifdef _DEBUG

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(success != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::unique_ptr<GLchar> message {std::make_unique<GLchar>(logLength)};
		glGetProgramInfoLog(program, logLength, nullptr, message.get());

		printf("Failed to link program : %s", message.get());

		return 0;
	}

	#endif

	return program;
}