#include "../include/Map.h"
#include "../include/TextureManager.h"
#include "../include/TileComponent.h"
#include "../include/Game.h"
#include "../include/ESC.h"
#include "../include/ColliderComponent.h"
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
			TileComponent* tc = &tile->getComponent<TileComponent>();
			tile->addGroup(Game::groupTile);
			file.ignore();
		}
		file.ignore();
	}
}