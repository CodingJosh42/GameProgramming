#ifndef MAP
#define MAP
#include "Game.h"

class Map {
public:
	Map();

	void loadMap(int map[20][25]);


private:
	int map[20][25];
};

#endif
