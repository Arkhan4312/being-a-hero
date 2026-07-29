#pragma once
#include <glm/glm.hpp>

class Unit;

class Projectile {
public:
	Projectile(const glm::vec2& startPos, const glm::vec2& direction,
		float speed, float lifetime, int damage, Unit* shooter);

	void update(float deltaTime);
	bool isExpired() const;
	glm::vec2 getPosition() const;
	int getDamage() const;
	Unit* getShooter() const;
	float getRadius() const;

private:
	glm::vec2 position_;
	glm::vec2 direction_;
	float speed_;
	float lifetime_;
	float age_ = 0.0f;
	int damage_;
	Unit* shooter_;
	bool expired_ = false;
};