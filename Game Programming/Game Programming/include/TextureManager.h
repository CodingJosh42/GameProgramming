#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER

#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* filename);
	static void DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);
	static void DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};

#endif