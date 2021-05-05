#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER

#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* ren);


};

#endif