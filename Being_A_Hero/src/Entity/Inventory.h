#pragma once
#include <vector>
#include <memory>
#include <string>

class Item {
public:
	Item(const std::string& id, const std::string& name);
	virtual ~Item() = default;
	virtual void use(class Unit& user) = 0;

	const std::string& getId() const;
	const std::string& getName() const;

protected:
	std::string id_;
	std::string name_;
};

class Inventory {
public:
	void addItem(std::unique_ptr<Item> item);
	void removeItem(const std::string& itemId);
	Item* getItem(const std::string& itemId) const;
	size_t getItemCount() const;
	void useItem(size_t index, Unit& user);
	const std::vector<std::unique_ptr<Item>>& getItems() const;

private:
	std::vector<std::unique_ptr<Item>> items_;
};