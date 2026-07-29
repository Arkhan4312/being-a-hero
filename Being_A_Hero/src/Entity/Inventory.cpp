#include "Inventory.h"
#include <algorithm>
Item::Item(const std::string& id, const std::string& name)
	: id_(id), name_(name) {}

const std::string& Item::getId() const
{
	return id_;
}

const std::string& Item::getName() const
{
	return name_;
}

void Inventory::addItem(std::unique_ptr<Item> item)
{
	items_.push_back(std::move(item));
}

void Inventory::removeItem(const std::string& itemId)
{
	auto it = std::find_if(items_.begin(), items_.end(), 
		[&](const std::unique_ptr<Item>& item) {
			return item->getId() == itemId;
		});
	if (it != items_.end()) {
		items_.erase(it);
	}
}

Item* Inventory::getItem(const std::string& itemId) const
{
	auto it = std::find_if(items_.begin(),items_.end(),
		[&](const std::unique_ptr<Item>& item) {
			return item->getId() == itemId;
		});
	return (it != items_.end() ? it->get() : nullptr);
}

size_t Inventory::getItemCount() const
{
	return items_.size();
}

void Inventory::useItem(size_t index, Unit& user)
{
	if (index < items_.size()) {
		items_[index]->use(user);
	}
}

const std::vector<std::unique_ptr<Item>>& Inventory::getItems() const
{
	return items_;
}

