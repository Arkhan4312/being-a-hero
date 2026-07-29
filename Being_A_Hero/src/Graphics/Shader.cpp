#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> message(length);
		glGetProgramInfoLog(id, length, &length, message.data());
		std::cerr << "Shader compilation error: " << message.data() << "\n";
		glDeleteShader(id);
		return 0;
	}
	return id;
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	: renderer_id_(0) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> message(length);
		glGetProgramInfoLog(program, length, &length, message.data());
		std::cerr << "Shader linking error: " << message.data() << "\n";

		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);

		throw std::runtime_error("Failed to link shader program");
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	renderer_id_ = program;
}

Shader::~Shader()
{
	if (renderer_id_) {
		glDeleteProgram(renderer_id_);
	}
}

void Shader::bind() const
{
	glUseProgram(renderer_id_);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	int location = glGetUniformLocation(renderer_id_, name.c_str());
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}
}

void Shader::setUniformInt(const std::string& name, int value)
{
	int location = glGetUniformLocation(renderer_id_, name.c_str());
	if (location != -1) {
		glUniform1i(location, value);
	}
}
