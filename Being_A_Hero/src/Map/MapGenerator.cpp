#include "MapGenerator.h"
#include <cstdlib>
#include <ctime>

namespace {
	float hash(int x, int y, unsigned int seed) {
		int n = x * 143087104 + y * 104781098 + seed * 969588582;
		n = (n ^ (n >> 13)) * 5434818;
		return (float)(n & 0x7fffffff) / 0x7fffffff;
	}

	float fade(float t) {
		return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
	}

	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	float valueNoise(float x, float y, unsigned int seed) {
		int ix = (int)floor(x);
		int iy = (int)floor(y);
		float fx = x - ix;
		float fy = y - iy;

		float v00 = hash(ix, iy, seed);
		float v10 = hash(ix + 1, iy, seed);
		float v01 = hash(ix, iy + 1, seed);
		float v11 = hash(ix + 1, iy + 1, seed);

		float u = fade(fx);
		float v = fade(fy);
		
		float nx0 = lerp(v00, v10, u);
		float nx1 = lerp(v01, v11, u);
		return lerp(nx0, nx1, v);
	}
} // local namespace

std::unique_ptr<TileMap> MapGenerator::generateRandomMap(int width, int height, unsigned int seed)
{
	auto map = std::make_unique<TileMap>(width, height);
	if (seed == 0) {
		seed = (unsigned int)std::time(nullptr);
	}

	const float scale = 10.0f;
	const float waterThreshold = 0.3f;
	const float forestThreshold = 0.6f;
	const float roadThreshold = 0.7f;

	for (int y = 0; y < height;++y) {
		for (int x = 0; x < width; ++x) {
			float val = valueNoise(x / scale, y / scale, seed);
			Tile& tile = map->getTile(x, y);
			if (val < waterThreshold) {
				tile.type = TileType::Water;
				tile.walkable = false;
			}
			else if (val < forestThreshold) {
				tile.type = TileType::Forest;
				tile.walkable = true;
			}
			else if (val < roadThreshold) {
				tile.type = TileType::Road;
				tile.walkable = true;
			}
			else {
				tile.type = TileType::Grass;
				tile.walkable = true;
			}
		}
	}
	return map;
}