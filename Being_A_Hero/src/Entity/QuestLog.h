#pragma once
#include <vector>
#include <memory>
#include <string>

class Quest {
public:
	Quest(const std::string& id, const std::string& title, const std::string& desc);
	virtual ~Quest() = default;

	bool isCompleted() const;
	void complete();
	const std::string& getId() const;

private:
	std::string id_;
	std::string title_;
	std::string description_;
	bool completed_ = false;
};

class QuestLog {
public:
	void addQuest(std::unique_ptr<Quest> quest);
	void completeQuest(const std::string& questID);
	const std::vector<std::unique_ptr<Quest>>& getActiveQuests() const;
	const std::vector<std::unique_ptr<Quest>>& getCompletedQuests() const;

private:
	std::vector<std::unique_ptr<Quest>> active_quests_;
	std::vector<std::unique_ptr<Quest>> completed_quests_;

};