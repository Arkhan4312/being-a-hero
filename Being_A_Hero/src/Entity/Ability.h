#pragma once
#include <string>

class Unit;

class Ability {
public:
	Ability(const std::string& name, int mana_cost, float cooldown);
	virtual ~Ability() = default;

	virtual void execute(Unit& caster, Unit& target) = 0;

	bool isOnCooldown() const;
	void startCooldown();
	void updateCooldown(float deltaTime);
	const std::string& getName() const;

protected:
	std::string name_;
	int mana_cost_;
	float cooldown_;
	float current_cooldown_ = 0.0f;
};