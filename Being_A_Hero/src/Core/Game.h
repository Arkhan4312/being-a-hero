#pragma once
#include "Entity/Player.h"
#include "Entity/Enemy.h"
#include "Entity/Projectile.h"
#include "Graphics/Camera2D.h"
#include "Graphics/Renderer.h"
#include "Map/GlobalMap.h"
#include "Map/TileMap.h"
#include <memory>
#include <vector>


class Game {
public:
	static Game& getInstance();
	bool init();
	void run();
	void shutdown();

	Player& getPlayer();
	TileMap* getCurrentMap();
	GlobalMap* getGlobalMap();

	void spawnEnemies(int count);
	void shootFireball(const glm::vec2& direction);
	void doMeleeAttack(Unit& attacker, Unit& target);

private:
	Game();
	void processInput(float deltaTime);
	void update(float deltaTime);
	void render();
	void updateProjectiles(float deltaTime);
	void updateEnemies(float deltaTime);
	std::unique_ptr<Renderer> renderer_;
	std::unique_ptr<Camera2D> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<TileMap> current_map_;
	std::unique_ptr<GlobalMap> global_map_;
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::vector<std::unique_ptr<Projectile>> projectiles_;
	bool running_ = false;
	float tile_size_ = 32.0f;
	glm::vec4 getTileColor(TileType type) const;
};