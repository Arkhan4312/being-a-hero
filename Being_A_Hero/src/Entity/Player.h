#pragma once
#include "Unit.h"
#include "Inventory.h"
#include "QuestLog.h"

class Player : public Unit {
public:
	Player(const std::string& name);

	void update(float deltaTime) override;

	Inventory& getInventory();
	QuestLog& getQuestLog();

	void addExp(int xp);
	int getExp() const;
	int getExpToNextLvl() const;

private:
	Inventory inventory_;
	QuestLog quest_log_;
	int experience_ = 0;
	int experience_to_next_ = 100;

	void levelUp();
};