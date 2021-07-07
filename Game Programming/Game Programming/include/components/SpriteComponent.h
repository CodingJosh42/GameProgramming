#ifndef SPRITE
#define SPRITE

#include "ESC.h"
#include <SDL.h>
#include "../TextureManager.h"
#include "TransformComponent.h"
#include <iostream>
#include "../Animation.h"
#include <map>
#include <string>

using namespace std;

class SpriteComponent : public Component {
private:
	SDL_Texture* texture;
	TransformComponent* position;
	SDL_Rect src;
	SDL_Rect dest;
	// Tiles
	int xOffset = 0;

	// Animation
	int frames;
	int delay;
	int animationIndex = 0;
	map<const char*, Animation> animations;

public:
	bool animated = false;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	double angle = 0;
	
	SpriteComponent() = default;

	/*
	* Constructor of SpriteComponent
	* @param id Id of texture
	*/
	SpriteComponent(const char* id) {
		setTexture(id);
	}

	/*
	* Constructor of SpriteComponent
	* @param id Id of texture
	* @param xOffset xOffset of src
	*/
	SpriteComponent(const char* id, int xOffset) : xOffset{ xOffset } {
		setTexture(id);
	}

	/*
	* Constructor of Sprite with animation
	* @param id Id of texture
	* @param animated If true texture is animated, if false texture is not animated
	*/
	SpriteComponent(const char* id, bool animated) : animated{ animated } {
		setTexture(id);

		Animation standing = Animation(0, 2, 200);
		Animation walking = Animation(1, 6, 150);
		Animation jumping = Animation(2, 1, 100);
		Animation crouching = Animation(3, 6, 200);
		Animation shielding = Animation(0, 4, 50);

		animations.emplace("standing", standing);
		animations.emplace("walking", walking);
		animations.emplace("jumping", jumping);
		animations.emplace("crouching", crouching);
		animations.emplace("shielding", shielding);
		
		setAnimation("standing");
	}

	/*
	* Constructor of Sprite with animation
	* @param id Id of texture
	* @param animated If true texture is animated, if false texture is not animated
	* @param startAnimation Id of the starting animation
	*/
	SpriteComponent(const char* id, bool animated, map<const char*, Animation>& anims, const char* startAnimation) : animated{ animated } {
		setTexture(id);

		animations = anims;

		setAnimation(startAnimation);
	}

	/*
	* Loads texture from assetManager
	* @param id Id of requested texture
	*/
	void setTexture(const char* id) {
		texture = Game::assetManager->getTexture(id);
	}


	void init() override {

		position = &entity->getComponent<TransformComponent>();

		src.x = xOffset;
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
		TextureManager::DrawTexture(texture, src, dest, flip, angle);
	}

	/*
	* Change animation
	* @param name Name of animation in list
	*/
	void setAnimation(const char* name) {
		frames = animations[name].frames;
		delay = animations[name].delay;
		animationIndex = animations[name].index;
	}
};

#endif