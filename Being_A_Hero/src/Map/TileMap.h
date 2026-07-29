#pragma once
#include <vector>

enum class TileType {
	Grass,
	Water,
	Forest,
	Road,
	//...
};

struct Tile {
	TileType type = TileType::Grass;
	bool walkable = true;
};

class TileMap {
public:
	TileMap(int width, int height);

	Tile& getTile(int x, int y);
	const Tile& getTile(int x, int y) const;
	int getWidth() const;
	int getHeight() const;
	void fill(TileType type);
	
private:
	int width_;
	int height_;
	std::vector<Tile> tiles_;
	int index(int x, int y) const;
};