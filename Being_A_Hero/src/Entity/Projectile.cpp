#include "Projectile.h"

Projectile::Projectile(const glm::vec2& startPos, const glm::vec2& direction, float speed, float lifetime, int damage, Unit* shooter)
	: position_(startPos),
	direction_(glm::normalize(direction)),
	speed_(speed),
	lifetime_(lifetime),
	damage_(damage),
	shooter_(shooter) {}

void Projectile::update(float deltaTime)
{
	age_ += deltaTime;
	if (age_ >= lifetime_) {
		expired_ = true;
		return;
	}
	position_ += direction_ * speed_ * deltaTime;
}

bool Projectile::isExpired() const
{
	return expired_;
}

glm::vec2 Projectile::getPosition() const
{
	return position_;
}

int Projectile::getDamage() const
{
	return damage_;
}

Unit* Projectile::getShooter() const
{
	return shooter_;
}

float Projectile::getRadius() const
{
	return 8.0f;
}
