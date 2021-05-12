#include "../include/GameObject.h"
#include "../include/TextureManager.h"

using namespace std;

GameObject::GameObject(const char* texture, int x, int y, void(*move) (int* xpos, int* ypos) ) :  xpos{ x }, ypos{ y }, move(move) {
	GameObject::texture = TextureManager::LoadTexture(texture);
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
	SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}