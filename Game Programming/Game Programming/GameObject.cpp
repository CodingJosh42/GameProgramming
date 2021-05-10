#include "GameObject.h"
#include "TextureManager.h"

using namespace std;

GameObject::GameObject(const char* texture, SDL_Renderer* renderer, int x, int y, void(*move) (int* xpos, int* ypos) ) : renderer(renderer), xpos{ x }, ypos{ y }, move(move) {
	GameObject::texture = TextureManager::LoadTexture(texture, renderer);
}

GameObject::~GameObject() {

}

void GameObject::update() {

	move(&xpos, &ypos);

	src.w = 64;
	src.h = 64;
	src.x = 0;
	src.y = 0;


	dest.h = src.h * 2;
	dest.w = src.w * 2;
	dest.x = xpos;
	dest.y = ypos;

}
void GameObject::render() {
	SDL_RenderCopy(renderer, texture, &src, &dest);
}