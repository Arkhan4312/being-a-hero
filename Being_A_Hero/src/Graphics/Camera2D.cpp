#include "Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>
Camera2D::Camera2D(float width, float height)
	: width_(width),height_(height){}

glm::mat4 Camera2D::getProjectionMatrix() const
{
	float halfW = width_ * 0.5f;
	float halfH = height_ * 0.5f;
	return glm::ortho(position_.x - halfW , position_.x + halfW,
		position_.y - halfH, position_.y + halfH,
		-1.0f,1.0f);
}

void Camera2D::setPosition(const glm::vec2& position)
{
	position_ = position;
	// need to update
}

glm::vec2 Camera2D::getPosition() const
{
	return position_;
}

float Camera2D::getWidth() const
{
	return width_;
}

float Camera2D::getHeight() const
{
	return height_;
}
