#include "SpriteBatch.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

SpriteBatch::SpriteBatch()
{
	vertices_.reserve(MaxVertices);
}

SpriteBatch::~SpriteBatch()
{
	if (initialized_) {
		glDeleteVertexArrays(1, &vao_);
		glDeleteBuffers(1, &vbo_);
	}
}

void SpriteBatch::begin(const glm::mat4& projection, const Texture2D& texture)
{
	projection_ = projection;
	current_texture_ = &texture;
	vertices_.clear();
}

void SpriteBatch::draw(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (vertices_.size() + 4 > MaxVertices) {
		flush();
	}

	float x = position.x;
	float y = position.y;
	float w = size.x;
	float h = size.y;
	glm::vec2 p0(x, y);
	glm::vec2 p1(x + w, y);
	glm::vec2 p2(x + w, y + h);
	glm::vec2 p3(x, y + h);
	glm::vec2 uv0(0, 0);
	glm::vec2 uv1(1, 0);
	glm::vec2 uv2(1, 1);
	glm::vec2 uv3(0, 1);

	vertices_.push_back({ p0,uv0,color });
	vertices_.push_back({ p1,uv2,color });
	vertices_.push_back({ p2,uv2,color });
	vertices_.push_back({ p0,uv0,color });
	vertices_.push_back({ p2,uv2,color });
	vertices_.push_back({ p3,uv3,color });
}

void SpriteBatch::end()
{
	flush();
}

void SpriteBatch::flush()
{
	if (vertices_.empty() || !current_texture_) {
		return;
	}
	if (!initialized_) {
		setupGL();
	}

	current_texture_->bind(0);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size()));
	glBindVertexArray(0);

	vertices_.clear();
}

void SpriteBatch::setupGL()
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	// layout: position(0), uv(1), color(2)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindVertexArray(0);
	initialized_ = true;
}
