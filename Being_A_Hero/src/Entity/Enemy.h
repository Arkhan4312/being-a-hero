#pragma once
#include "Unit.h"
#include <glm/glm.hpp>
#include "Map/TileMap.h"
class Enemy : public Unit {
public:
	enum class State {
		Idle,
		Chase,
		Attack,
	};

	Enemy(const std::string& name, int level);

	void updateAi(float deltaTime, const glm::vec2& playerPos,
		const class TileMap& map, float tileSize);

	State getState() const;
	float getAttackRange() const;
	float getDetectionRange() const;
	float getAttackTimer() const;
	void resetAttackTimer();;
	void takeDamage(int amount) override;
	void attack(float deltaTime, Unit& target);
private:
	State state_ = State::Idle;
	glm::vec2 patrol_target_;
	float attack_cooldown_ = 1.5f;
	float attack_timer_ = 0.0f;
	float state_timer_ = 0.0f;

	void setState(State newState);
	void patrol(float deltaTime);
	void chase(float deltaTime, const glm::vec2& playerPos,
		const TileMap& map, float tileSize);
	
	bool canMoveTo(const glm::vec2& pos, const TileMap& map, float tileSize) const;
};