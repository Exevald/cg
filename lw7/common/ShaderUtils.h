#pragma once

#include <GL/glew.h>

#include <stdexcept>
#include <string>

inline GLuint CompileShader(const GLenum type, const char* source)
{
	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_TRUE)
	{
		return shader;
	}

	GLint logLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::string log(static_cast<size_t>(logLength), '\0');
	glGetShaderInfoLog(shader, logLength, nullptr, log.data());
	glDeleteShader(shader);
	throw std::runtime_error("Shader compilation failed: " + log);
}

inline GLuint CreateProgram(const char* vertexSource, const char* fragmentSource)
{
	const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	const GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint success = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_TRUE)
	{
		return program;
	}

	GLint logLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::string log(static_cast<size_t>(logLength), '\0');
	glGetProgramInfoLog(program, logLength, nullptr, log.data());
	glDeleteProgram(program);
	throw std::runtime_error("Program link failed: " + log);
}

inline GLuint CreateProgram(const char* vertexSource, const char* geometrySource, const char* fragmentSource)
{
	const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	const GLuint geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource);
	const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	const GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	GLint success = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_TRUE)
	{
		return program;
	}

	GLint logLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::string log(static_cast<size_t>(logLength), '\0');
	glGetProgramInfoLog(program, logLength, nullptr, log.data());
	glDeleteProgram(program);
	throw std::runtime_error("Program link failed: " + log);
}
