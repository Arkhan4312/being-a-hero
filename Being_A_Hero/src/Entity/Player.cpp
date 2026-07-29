#include "Player.h"
#include <iostream>
Player::Player(const std::string& name)
	: Unit(name, 1) {
	std::cout << "Player " << name << " created.\n";
}

void Player::update(float deltaTime) {
	Unit::update(deltaTime);

	// other logic btw
}

Inventory& Player::getInventory()  {
	return inventory_;
}

QuestLog& Player::getQuestLog()  {
	return quest_log_;
}

void Player::addExp(int xp)
{
	experience_ += xp;
	while (experience_ >= experience_to_next_) {
		levelUp();
	}
}

int Player::getExp() const
{
	return experience_;
}

int Player::getExpToNextLvl() const
{
	return experience_to_next_;
}

void Player::levelUp()
{
	experience_ -= experience_to_next_;
	++level_;
	experience_to_next_ = (experience_to_next_ * 2);
	maxHP_ = stats_.maxHP();
	hp_ = maxHP_;
	std::cout << "Level up! Now level " << level_ << "\n";
}

