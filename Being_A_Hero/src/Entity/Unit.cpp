#include "Unit.h"
const int CBASE_HP = 100;
const int CMULT_HP = 10;
const int CMULT_STATS = 2;
int Stats::maxHP() const
{
    return CBASE_HP + vitality * CMULT_HP;
}

int Stats::physicalDamage() const {
    return strength * CMULT_STATS;
}

int Stats::magicDamage() const {
    return intelligence * CMULT_STATS;
}

int Stats::defence() const {
    return agility / CMULT_STATS;
}

Unit::Unit(const std::string& name, int level)
    : name_(name), level_(level), hp_(0), maxHP_(0)
{
    maxHP_ = stats_.maxHP();
    hp_ = maxHP_;
}

void Unit::update(float deltaTime)
{
    for (auto& ability : abilities_) {
        ability->updateCooldown(deltaTime);
    }
}

void Unit::takeDamage(int amount)
{
    hp_ = std::max(0, hp_ - amount);
    if (hp_ == 0) {
        onDeath();
    }
}

void Unit::heal(int amount)
{
    hp_ = std::min(maxHP_, hp_ + amount);
}

void Unit::onDeath()
{
    // Later
}

const std::string& Unit::getName() const {
    return name_;
}

int Unit::getHp() const {
    return hp_;
}

int Unit::getMaxHp() const
{
    return maxHP_;
}

int Unit::getLevel() const
{
    return level_;
}

Stats& Unit::getStats()
{
    return stats_;
}

bool Unit::isAlive() const
{
    return hp_ > 0;
}

void Unit::addAbility(std::unique_ptr<Ability> ability)
{
    abilities_.push_back(std::move(ability));
}

const std::vector<std::unique_ptr<Ability>>& Unit::getAbilities() const
{
    return abilities_;
}

glm::vec2 Unit::getPosition() const
{
    return position_;
}

void Unit::setPosition(const glm::vec2& pos)
{
    position_ = pos;
}

