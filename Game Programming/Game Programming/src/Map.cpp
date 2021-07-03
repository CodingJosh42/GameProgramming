#include "../include/Map.h"
#include "../include/TextureManager.h"
#include "../include/components/TileComponent.h"
#include "../include/Game.h"
#include "../include/components/ESC.h"
#include "../include/components/ColliderComponent.h"
#include <fstream>
#include <iostream>
#include "../include/Numbers.h"

using namespace std;



void Map::loadMap(const char* path, int xSize, int ySize) {
	char tileId;
	fstream file;
	file.open(path);

	for (int col = 0; col < ySize; col++) {
		for (int row = 0; row < xSize; row++) {
			file.get(tileId);
			Entity* tile = Game::manager.addEntity();
			tile->addComponent<TileComponent>(row * TILESIZE, col * TILESIZE, TILESIZE, TILESIZE, atoi(&tileId));
			tile->addGroup(Game::groupTile);
			file.ignore();
		}
		file.ignore();
	}
}