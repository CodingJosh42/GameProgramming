#ifndef MAP
#define MAP
#include "Game.h"

class Map {
public:
	Map();
	~Map();

	void loadMap(int map[20][25]);
	void drawMap();

private:
	SDL_Rect src, dest;
	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* water;
	SDL_Texture* metal;
	SDL_Texture* sky;
	SDL_Texture* cloud;

	int map[20][25];
};

#endif
