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

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	void setTexture(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override {

		position = &entity->getComponent<TransformComponent>();

		src.x = 0;
		src.y = 0;
		src.w = position->width * 2;
		src.h = position->height * 2;

		dest.x = (int) position->position.x;
		dest.y = (int) position->position.y;
		dest.w = position->width * position->scale;
		dest.h = position->height * position->scale;
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