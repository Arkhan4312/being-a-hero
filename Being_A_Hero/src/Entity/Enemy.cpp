#include "Enemy.h"
#include "Map/TileMap.h"
#include <cmath>
#include <iostream>
Enemy::Enemy(const std::string& name, int level)
	: Unit(name,level), patrol_target_(position_) {
	hp_ = 60;
	maxHP_ = 60;
	state_ = State::Idle;
}

void Enemy::updateAi(float deltaTime, const glm::vec2& playerPos, const TileMap& map, float tileSize)
{
	if (!isAlive()) {
		return;
	}
	Unit::update(deltaTime);
	if (attack_timer_ < 0) {
		attack_timer_ = 0;
	}
	float dist = glm::distance(position_, playerPos);
	switch (state_) {
	case State::Idle:
		if (dist < getDetectionRange()) {
			setState(State::Chase);
		}
		else {
			patrol(deltaTime);
		}
		break;
	case State::Chase:
		if (dist < getAttackRange() && attack_timer_ <= 0) {
			setState(State::Attack);
		}
		else if (dist > getDetectionRange() * 1.5f) {
			setState(State::Idle);
		}
		else {
			chase(deltaTime, playerPos, map, tileSize);
		}
		break;
	case State::Attack:
		if (dist > getAttackRange() * 1.2f) {
			setState(State::Chase);
		}
		else if (attack_timer_ <= 0) {
			attack_timer_ = attack_cooldown_;
		}
		break;
	}
}

Enemy::State Enemy::getState() const
{
	return state_;
}

float Enemy::getAttackRange() const
{
	return 35.0f;
}

float Enemy::getDetectionRange() const
{
	return 200.0f;
}

float Enemy::getAttackTimer() const
{
	return attack_timer_;
}

void Enemy::resetAttackTimer()
{
	attack_timer_ = 1.5f;
}

void Enemy::takeDamage(int amount)
{
	Unit::takeDamage(amount);
	if (!isAlive()) {
		// something to drop
	}
}

void Enemy::setState(State newState)
{
	if (state_ == newState) {
		return;
	}
	state_ = newState;
	state_timer_ = 0.0f;
}

void Enemy::patrol(float deltaTime)
{
	state_timer_ += deltaTime;
	if (state_timer_ > 2.0f) {
		state_timer_ = 0.0f;
		patrol_target_ = position_ + glm::vec2((rand()% 100 - 50) / 10.0f,
			(rand() % 100 - 50) / 10.0f);
	}

	glm::vec2 dir = patrol_target_ - position_;
	float len = glm::length(dir);
	if (len > 1.0f) {
		dir /= len;
		position_ += dir * 30.0f * deltaTime;
	}
}

void Enemy::chase(float deltaTime, const glm::vec2& playerPos, const TileMap& map, float tileSize)
{
	glm::vec2 dir = playerPos - position_;
	float len = glm::length(dir);
	if (len > 0.1f) {
		dir /= len;
		glm::vec2 newPos = position_ + dir * 80.0f * deltaTime;

		float ts = tileSize;

		int tx = static_cast<int>(std::floor(newPos.x / ts));
		int ty = static_cast<int>(std::floor(position_.y / ts));
		if (tx >= 0 && tx < map.getWidth() && ty >= 0 && ty < map.getHeight() && map.getTile(tx, ty).walkable) {
			position_.x = newPos.x;
		}
		tx = static_cast<int>(std::floor(position_.x / ts));
		ty = static_cast<int>(std::floor(newPos.y / ts));
		if (tx >= 0 && tx < map.getWidth() && ty >= 0 && ty < map.getHeight() && map.getTile(tx, ty).walkable) {
			position_.y = newPos.y;
		}
	}
}

void Enemy::attack(float deltaTime, Unit& target)
{
	if (attack_timer_ > 0.0f) {
		attack_timer_ -= deltaTime;
		if (attack_timer_ < 0.0f) {
			attack_timer_ = 0.0f;
			return;
		}
	}
	int dmg = stats_.physicalDamage();
	target.takeDamage(dmg);
	std::cout << name_ << " attacks " << target.getName()
		<< " for " << dmg << " damage. HP left: " << target.getHp() << "\n";
	attack_timer_ = attack_cooldown_;
}

bool Enemy::canMoveTo(const glm::vec2& pos, const TileMap& map, float tileSize) const
{
	int tx = static_cast<int>(std::floor(pos.x / tileSize));
	int ty = static_cast<int>(std::floor(position_.y / tileSize));
	return (tx >= 0 && tx < map.getWidth() && ty >= 0 && ty < map.getHeight() && map.getTile(tx, ty).walkable);
}
