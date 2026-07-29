#include "Game.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Map/MapGenerator.h"
Game& Game::getInstance()
{
    static Game instance;
    return instance;
}

bool Game::init()
{
    renderer_ = std::make_unique<Renderer>();
    if (!renderer_->init(1280, 720, "Being a hero")) {
        return false;
    }

    camera_ = std::make_unique<Camera2D>(40 * tile_size_, 22.5f * tile_size_);

    current_map_ = MapGenerator::generateRandomMap(250, 250);
    global_map_ = std::make_unique<GlobalMap>(20, 15);

    player_ = std::make_unique<Player>("Hero");

    bool spawned = false;
    for (int y = 0; y < current_map_->getHeight() && !spawned; ++y) {
        for (int x = 0; x < current_map_->getWidth(); ++x) {
            if (current_map_->getTile(x, y).walkable) {
                glm::vec2 startPos(x * tile_size_ + tile_size_ * 0.5f,
                    y * tile_size_ + tile_size_ * 0.5f);
                player_->setPosition(startPos);
                spawned = true;
                break;
            }
        }
    }
    if (!spawned) {
        std::cerr << "Map has no walkable tiles!\n";
        return false;
    }
    spawnEnemies(100);
    running_ = true;
    return true;
}

void Game::run() {
    if (!running_) {
        return;
    }
    float lastTime = (float)glfwGetTime();
    while (!renderer_->windowShouldClose()) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        renderer_->pollEvents();
        processInput(deltaTime);
        update(deltaTime);
        render();
    }
    shutdown();
}

void Game::shutdown() {
    player_.reset();
    enemies_.clear();
    projectiles_.clear();
    current_map_.reset();
    global_map_.reset();
    camera_.reset();
    renderer_->shutdown();
}

Player& Game::getPlayer()
{
    return *player_;
}

TileMap* Game::getCurrentMap() {
    return current_map_.get();
}

GlobalMap* Game::getGlobalMap() {
    return global_map_.get();
}
void Game::spawnEnemies(int count)
{
    for (int i = 0; i < count;++i) {
        auto enemy = std::make_unique<Enemy>("Goblin", 1);

        bool placed = false;
        for (int attempt = 0; attempt < 100; ++attempt) {
            int x = rand() % current_map_->getWidth();
            int y = rand() % current_map_->getHeight();
            if (current_map_->getTile(x, y).walkable) {
                glm::vec2 pos(x * tile_size_ + tile_size_ * 0.5f,
                    y * tile_size_ + tile_size_ * 0.5f);
                if (glm::distance(pos, player_->getPosition()) > 200.0f) {
                    enemy->setPosition(pos);
                    placed = true;
                    break;
                }
            }
        }
        if (placed) {
            enemies_.push_back(std::move(enemy));
        }
        else {
            enemy->setPosition(player_->getPosition() + glm::vec2(50, 50));
            enemies_.push_back(std::move(enemy));
        }
    }
}

void Game::shootFireball(const glm::vec2& direction)
{
    glm::vec2 startPos = player_->getPosition() + direction * 20.0f;
    auto proj = std::make_unique<Projectile>(startPos, direction, 400.0f, 2.0f, 25, player_.get());
    projectiles_.push_back(std::move(proj));
}

void Game::doMeleeAttack(Unit& attacker, Unit& target)
{
    int dmg = attacker.getStats().physicalDamage();
    target.takeDamage(dmg);
    std::cout << attacker.getName() << " attacks " << target.getName()
        << " for " << dmg << " damage. HP left: " << target.getHp() << "\n";
}

Game::Game() = default;

namespace {
    static bool isWalkable(int tileX, int tileY, const TileMap& map) {
        if (tileX < 0 || tileX >= map.getWidth() || tileY < 0 || tileY >= map.getHeight()) {
            return false;
        }
        return map.getTile(tileX, tileY).walkable;
    }
} // local namespace

void Game::processInput(float deltaTime)
{
    if (!renderer_->getWindow()) {
        return;
    }

    float speed = 200.0f * deltaTime;
    glm::vec2 pos = player_->getPosition();
    glm::vec2 newPos = pos;

    bool keyW = glfwGetKey(renderer_->getWindow(), GLFW_KEY_W) == GLFW_PRESS;
    bool keyS = glfwGetKey(renderer_->getWindow(), GLFW_KEY_S) == GLFW_PRESS;
    bool keyA = glfwGetKey(renderer_->getWindow(), GLFW_KEY_A) == GLFW_PRESS;
    bool keyD = glfwGetKey(renderer_->getWindow(), GLFW_KEY_D) == GLFW_PRESS;

    static glm::vec2 lastMoveDir(1, 0);
    if (keyW)
    {
        newPos.y += speed;
        lastMoveDir = glm::vec2(0,1);
    }

    if (keyS) {
        newPos.y -= speed;
        lastMoveDir = glm::vec2(0,-1);
    }

    if (keyA) {
        newPos.x -= speed;
        lastMoveDir = glm::vec2(-1,0);
    }

    if (keyD) {
        newPos.x += speed;
        lastMoveDir = glm::vec2(1.0);
    }

    int tileX = static_cast<int>(std::floor(newPos.x / tile_size_));
    int tileY = static_cast<int>(std::floor(pos.y / tile_size_));
    if (isWalkable(tileX, tileY, *current_map_)) {
        pos.x = newPos.x;
    }

    tileX = static_cast<int>(std::floor(pos.x / tile_size_));
    tileY = static_cast<int>(std::floor(newPos.y / tile_size_));
    if (isWalkable(tileX, tileY, *current_map_)) {
        pos.y = newPos.y;
    }
    
    player_->setPosition(pos);

    // fireball

    static bool firePressed = false;
    bool fireNow = glfwGetKey(renderer_->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS;
    if (fireNow && !firePressed) {
        firePressed = true;
        glm::vec2 dir;
        dir = lastMoveDir;
        if (glm::length(dir) > 0.001f) {
            shootFireball(glm::normalize(dir));
        }
    }
    if (!fireNow) {
        firePressed = false;
    }
    static bool meleePressed = false;
    bool mouseLeft = glfwGetMouseButton(renderer_->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
    if (mouseLeft) {
        meleePressed = true;
        for (auto& enemy : enemies_) {
            if (enemy->isAlive() && glm::distance(player_->getPosition(), enemy->getPosition()) < 40.0f) {
                doMeleeAttack(*player_, *enemy);
                break;
            }
        }
    }
    if (glfwGetKey(renderer_->getWindow(), GLFW_MOUSE_BUTTON_1) != GLFW_PRESS) meleePressed = false;
}
void Game::update(float deltaTime)
{
    updateEnemies(deltaTime);
    updateProjectiles(deltaTime);
    camera_->setPosition(player_->getPosition());
    player_->update(deltaTime);

}
void Game::render()
{
    renderer_->beginFrame(*camera_);
    SpriteBatch& sb = renderer_->getSpriteBatch();

    TileMap& map = *current_map_;
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            Tile& tile = map.getTile(x, y);
            glm::vec2 pos(x * tile_size_, y * tile_size_);
            sb.draw(pos, glm::vec2(tile_size_), getTileColor(tile.type));
        }
    }
    for (auto& enemy : enemies_) {
        if (!enemy->isAlive()) {
            continue;
        }
        glm::vec2 epos = enemy->getPosition();
        sb.draw(epos - glm::vec2(tile_size_ * 0.5f), glm::vec2(tile_size_), glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));
    }

    for (auto& proj : projectiles_) {
        float r = proj->getRadius();
        sb.draw(proj->getPosition() - glm::vec2(r), glm::vec2(r * 2), glm::vec4(1.0f, 0.6f, 0.0f, 1.0f));
    }

    glm::vec2 playerPos = player_->getPosition();
    sb.draw(playerPos - glm::vec2(tile_size_ * 0.5f), glm::vec2(tile_size_), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));

    renderer_->endFrame();
}
void Game::updateProjectiles(float deltaTime)
{
    for (auto& proj : projectiles_) {
        proj->update(deltaTime);
    }
    for (auto it = projectiles_.begin(); it != projectiles_.end(); ) {
        bool hit = false;
        for (auto& enemy : enemies_) {
            if (enemy->isAlive() && glm::distance((*it)->getPosition(), enemy->getPosition()) <
                ((*it)->getRadius() + tile_size_ * 0.4f)) {
                enemy->takeDamage((*it)->getDamage());
                hit = true;
                std::cout << "Fireball hits " << enemy->getName()
                    << " for " << (*it)->getDamage() << " damage. \n";
                break;
            }
        }
        if (hit || (*it)->isExpired()) {
            it = projectiles_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::updateEnemies(float deltaTime)
{
    for (auto& enemy : enemies_) {
        if (!enemy->isAlive()) {
            continue;
        }
        enemy->updateAi(deltaTime, player_->getPosition(), *current_map_, tile_size_);

        if (enemy->getState() == Enemy::State::Attack) {
            enemy->attack(deltaTime, *player_);
        }
    }
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return !e->isAlive();
            }),
        enemies_.end()
    );
}

glm::vec4 Game::getTileColor(TileType type) const
{
    switch (type) {
    case TileType::Grass: 
        return glm::vec4(0.3f, 0.8f, 0.2f, 1.0f);
    case TileType::Water:
        return glm::vec4(0.2f, 0.4f, 0.9f, 1.0f);
    case TileType::Road:
        return glm::vec4(0.7f, 0.7f, 0.3f, 1.0f);
    case TileType::Forest:
        return glm::vec4(0.1f, 0.6f, 0.1f, 1.0f);
    default:
        return glm::vec4(1.0f);
    }
}
