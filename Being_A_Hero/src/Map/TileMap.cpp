#include "TileMap.h"
#include <stdexcept>

TileMap::TileMap(int width, int height)
	: width_(width), height_(height) {
	tiles_.resize(width * height);
}

Tile& TileMap::getTile(int x, int y)
{
	return tiles_[index(x, y)];
}

const Tile& TileMap::getTile(int x, int y) const {
	return tiles_[index(x, y)];
}

int TileMap::getWidth() const
{
	return width_;
}

int TileMap::getHeight() const
{
	return height_;
}

void TileMap::fill(TileType type)
{
	// placeholder
	for (auto& tile : tiles_) {
		tile.type = type;
		tile.walkable = (type != TileType::Water);
	}
}

int TileMap::index(int x, int y) const
{
	return y * width_ + x;
}
