#include "Ability.h"

Ability::Ability(const std::string& name, int mana_cost, float cooldown)
	:name_(name), mana_cost_(mana_cost), cooldown_(cooldown), current_cooldown_(0.0f) {}

bool Ability::isOnCooldown() const
{
	return current_cooldown_ > 0.0f;
}

void Ability::startCooldown()
{
	current_cooldown_ = cooldown_;
}

void Ability::updateCooldown(float deltaTime)
{
	if (current_cooldown_ > 0.0f) {
		current_cooldown_ -= deltaTime;
		if (current_cooldown_ < 0.0f) {
			current_cooldown_ = 0.0f;
		}
	}
}

const std::string& Ability::getName() const
{
	return name_;
}

