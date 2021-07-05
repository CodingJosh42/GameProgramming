#ifndef MAP
#define MAP
#include "Game.h"
#include <tuple>
#include <vector>
#include "components/EnemyComponent.h"
class Map {
public:
	static void loadMap(const char* path, int x, int y);
	static vector<tuple<int, int, EnemyComponent::EnemyType>> loadSpawnPoints(const char* path, int x, int y);
};

#endif
