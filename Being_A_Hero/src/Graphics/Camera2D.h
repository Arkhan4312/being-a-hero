#pragma once
#include <glm/glm.hpp>

class Camera2D {
public:
	Camera2D(float width, float height);
	glm::mat4 getProjectionMatrix() const;
	void setPosition(const glm::vec2& position);
	glm::vec2 getPosition() const;
	float getWidth() const;
	float getHeight() const;

private:
	glm::vec2 position_{ 0.0f,0.0f };
	float width_;
	float height_;
};