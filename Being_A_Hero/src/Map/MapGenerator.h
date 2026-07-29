#pragma once
#include <memory>
#include "TileMap.h"

class MapGenerator {
public:
	static std::unique_ptr<TileMap> generateRandomMap(int width, int height, unsigned int seed = 0);
};