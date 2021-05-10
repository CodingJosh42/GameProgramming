#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Game.h"

class GameObject {
public:
	GameObject(const char* texture, SDL_Renderer* renderer, int x, int y, void (*move) (int* xpos, int* ypos) );
	~GameObject();

	void update();
	void render();
	void (*move) (int* xpos, int* ypos);

private:
	int xpos, ypos;

	SDL_Texture* texture;
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Renderer* renderer;

};

#endif