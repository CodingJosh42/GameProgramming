#include "Map.h"
#include "TextureManager.h"

int lvl1[20][25] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,2,2,2,1,1,1,4,4},
};

Map::Map() {
	dirt = TextureManager::LoadTexture("assets/dirt.png");
	grass = TextureManager::LoadTexture("assets/grass.png");
	water = TextureManager::LoadTexture("assets/water.png");
	metal = TextureManager::LoadTexture("assets/metal.png");
	sky = TextureManager::LoadTexture("assets/sky.png");
	cloud = TextureManager::LoadTexture("assets/cloud.png");

	loadMap(lvl1);

	src.x = 0;
	src.y = 0;

	src.w = 32;
	src.h = 32;

	dest.x = 0;
	dest.y = 0;

	dest.w = 32;
	dest.h = 32;
}
Map::~Map() {

}

void Map::loadMap(int loadedMap[20][25]) {
	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {
			map[row][col] = loadedMap[row][col];
		}
	}
}
void Map::drawMap() {
	int type = 0;

	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {

			dest.x = col * 32;
			dest.y = row * 32;
			type = map[row][col];

			switch (type) {
			case 0:
				TextureManager::DrawTexture(sky, src, dest);
				break;
			case 1:
				TextureManager::DrawTexture(grass, src, dest);
				break;
			case 2:
				TextureManager::DrawTexture(water, src, dest);
				break;
			case 3:
				TextureManager::DrawTexture(metal, src, dest);
				break;
			case 4:
				TextureManager::DrawTexture(dirt, src, dest);
				break;
			case 5:
				TextureManager::DrawTexture(cloud, src, dest);
				break;
			default:
				break;
			}
		}
	}
}