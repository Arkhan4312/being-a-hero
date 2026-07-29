#pragma once
#include <glm/glm.hpp>
#include "Texture2D.h"
// VAO/BAO need to realize. Now only placeholders
class Texture2D;

class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();


	void begin(const glm::mat4& projection, const Texture2D& texture);
	void draw(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void end();
	void flush();
private:
	struct Vertex {
		glm::vec2 pos;
		glm::vec2 uv;
		glm::vec4 color;
	};

	static const size_t MaxVertices = 4096;
	std::vector<Vertex> vertices_;
	const Texture2D* current_texture_ = nullptr;
	glm::mat4 projection_;

	unsigned int vao_ = 0;
	unsigned int vbo_ = 0;
	bool initialized_ = 0;

	void setupGL();
};