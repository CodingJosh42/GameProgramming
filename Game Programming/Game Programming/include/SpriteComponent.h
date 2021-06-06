#ifndef SPRITE
#define SPRITE

#include "ESC.h"
#include <SDL.h>
#include "TextureManager.h"
#include "TransformComponent.h"
#include <iostream>
#include "Animation.h"
#include <map>

using namespace std;

class SpriteComponent : public Component {
private:
	SDL_Texture* texture;
	TransformComponent* position;
	SDL_Rect src;
	SDL_Rect dest;

	// Animation
	int frames;
	int delay;
	int animationIndex = 0;
	map<const char*, Animation> animations;

public:
	bool animated = false;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
	SpriteComponent() = default;
	SpriteComponent(string id) {
		setTexture(id);

	}
	SpriteComponent(string id, bool animated) : animated{ animated } {
		setTexture(id);

		Animation standing = Animation(0, 2, 200);
		Animation walking = Animation(1, 6, 150);
		Animation jumping = Animation(2, 1, 100);
		Animation crouching = Animation(3, 2, 500);

		animations.emplace("standing", standing);
		animations.emplace("walking", walking);
		animations.emplace("jumping", jumping);
		animations.emplace("crouching", crouching);

		setAnimation("standing");
	}

	void setTexture(string id) {
		texture = Game::assetManager->getTexture(id);
	}


	void init() override {

		position = &entity->getComponent<TransformComponent>();

		src.x = 0;
		src.y = 0;
		src.w = position->width;
		src.h = position->height;

		dest.x = (int) position->position.x;
		dest.y = (int) position->position.y;
		dest.w = position->width * position->scale;
		dest.h = position->height * position->scale;
	}

	void update() override {
		if (animated) {
			src.x = src.w * (int)((SDL_GetTicks() / delay) % frames);
		}
		src.y = position->height * animationIndex;

		dest.x = (int)position->position.x - Game::camera.x;
		dest.y = (int)position->position.y - Game::camera.y;
	}

	void draw() override {
		TextureManager::DrawTexture(texture, src, dest, flip);
	}

	void setAnimation(const char* name) {
		frames = animations[name].frames;
		delay = animations[name].delay;
		animationIndex = animations[name].index;
	}
};

#endif