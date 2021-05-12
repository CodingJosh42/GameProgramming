#ifndef SPRITE
#define SPRITE

#include "ESC.h"
#include <SDL.h>
#include "TextureManager.h"
#include "TransformComponent.h"

class SpriteComponent : public Component {
private:
	SDL_Texture* texture;
	TransformComponent* position;
	SDL_Rect src;
	SDL_Rect dest;
public:
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		setTexture(path);
	}

	void setTexture(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override {

		position = &entity->getComponent<TransformComponent>();

		src.x = 0;
		src.y = 0;
		src.w = 64;
		src.h = 64;

		dest.x = (int) position->position.x;
		dest.y = (int) position->position.y;
		dest.w = src.w * 2;
		dest.h = src.h * 2;
	}

	void update() override {
		dest.x = (int)position->position.x;
		dest.y = (int)position->position.y;
	}
	void draw() override {
		TextureManager::DrawTexture(texture, src, dest);
	}
};

#endif