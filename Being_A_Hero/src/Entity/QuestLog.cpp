#include "QuestLog.h"

Quest::Quest(const std::string& id, const std::string& title, const std::string& desc)
	: id_(id), title_(title), description_(desc), completed_(false) {}

bool Quest::isCompleted() const
{
	return completed_;
}

void Quest::complete()
{
	completed_ = true;
}

const std::string& Quest::getId() const
{
	return id_;
}

void QuestLog::addQuest(std::unique_ptr<Quest> quest)
{
	active_quests_.push_back(std::move(quest));
}

void QuestLog::completeQuest(const std::string& questID)
{
	auto it = std::find_if(active_quests_.begin(), active_quests_.end(),
		[&](const std::unique_ptr<Quest>& q) {
			return q->getId() == questID;
		});
	if (it != active_quests_.end()) {
		(*it)->complete();
		completed_quests_.push_back(std::move(*it));
		active_quests_.erase(it);
	}
}

const std::vector<std::unique_ptr<Quest>>& QuestLog::getActiveQuests() const
{
	return active_quests_;
}

const std::vector<std::unique_ptr<Quest>>& QuestLog::getCompletedQuests() const
{
	return completed_quests_;
}


