#include "GlobalMap.h"

GlobalMap::GlobalMap(int sizeX, int sizeY)
	: size_x_(sizeX), size_y_(sizeY) {
	locations_.resize(size_x_ * size_y_);
}

GlobalMap::Location& GlobalMap::getLocation(int x, int y)
{
	return locations_[y * size_x_ + x];
}

int GlobalMap::getSizeX() const
{
	return size_x_;
}

int GlobalMap::getSizeY() const
{
	return size_y_;
}
