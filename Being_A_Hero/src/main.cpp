#include "Core/Game.h"

int main() {
	if (!Game::getInstance().init()) {
		return -1;
	}
	Game::getInstance().run();
	return 0;
}