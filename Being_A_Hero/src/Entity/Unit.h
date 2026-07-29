#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Ability.h"
#include <glm/glm.hpp>
struct Stats {
	int strength = 10;
	int agility = 10;
	int intelligence = 10;
	int vitality = 10;

	int maxHP() const;
	int physicalDamage() const;
	int magicDamage() const;
	int defence() const;
};

class Ability;

class Unit {
public:
	Unit(const std::string& name, int level);
	virtual ~Unit() = default;

	virtual void update(float deltaTime);
	virtual void takeDamage(int amount);
	virtual void heal(int amount);
	virtual void onDeath();

	const std::string& getName() const;
	int getHp() const;
	int getMaxHp() const;
	int getLevel() const;
	Stats& getStats();
	bool isAlive() const;
	
	void addAbility(std::unique_ptr<Ability> ability);
	const std::vector<std::unique_ptr<Ability>>& getAbilities() const;

	glm::vec2 getPosition() const;
	void setPosition(const glm::vec2& pos);
protected:
	std::string name_;
	int hp_;
	int maxHP_;
	int level_;
	Stats stats_;
	std::vector<std::unique_ptr<Ability>> abilities_;
	glm::vec2 position_{ 0.0f,0.0f };
};