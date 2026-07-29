#pragma once 
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniformMat4(const std::string& name, const glm::mat4& matrix);
	void setUniformInt(const std::string& name, int value);
private:
	unsigned int renderer_id_ = 0;
};