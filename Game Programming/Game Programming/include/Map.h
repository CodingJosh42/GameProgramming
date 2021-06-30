#ifndef MAP
#define MAP
#include "Game.h"

class Map {
public:

	void loadMap(int map[20][50]);
	static void loadMap(const char* path, int x, int y);


private:
	int map[20][25];
};

#endif
