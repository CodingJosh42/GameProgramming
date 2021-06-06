#include "../include/TextureManager.h"
#include "../include/Game.h"

/*
* Loads texture from given file
* @param filename Path of file
* @return Returns SDL_Texture
*/
SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	SDL_Surface* tmpSurface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return texture;
}

/*
* Draws texture to the screen
* @param texture Texture that should be drawn
* @param src Source rect of texture
* @param dest Destination rect of texture
*/
void TextureManager::DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}

/*
* Draws texture to screen with additional flip flag
* @param texture Texture that should be drawn
* @param src Source rect of texture
* @param dest Destination rect of texture 
* @param flip Set flag if image needs to be flipped
*/
void TextureManager::DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, texture, &src, &dest, NULL, NULL, flip);
}