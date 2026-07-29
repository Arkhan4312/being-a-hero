#pragma once
#include <vector>
#include <string>

class GlobalMap {
public:
	struct Location {
		std::string name;
		std::string mapToLoad;
		bool visited = false; // need to change
	};

	GlobalMap(int sizeX, int sizeY);

	Location& getLocation(int x, int y);
	int getSizeX() const;
	int getSizeY() const;

private:
	int size_x_;
	int size_y_;
	std::vector<Location> locations_;
};