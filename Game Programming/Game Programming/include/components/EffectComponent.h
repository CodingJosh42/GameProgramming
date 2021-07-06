#pragma once
#include "ESC.h"
#include "../Animation.h"
#include <SDL.h>

class EffectComponent : public Component {
private:
	bool firstTime = true;
	Animation anim;
	int width;
public:
	/*
	* Constructor
	* @param anim Animation of effect
	* @param width Width of single frame sprites
	*/
	EffectComponent(Animation anim, int width) : anim{ anim }, width{ width } {

	}
	void update() override {
		int currentIndex = width * (int)((SDL_GetTicks() / anim.delay) % anim.frames);
		if (currentIndex > 0) {
			firstTime = false;
		}
		// Destroy entity after animation has run one time
		if (!firstTime && currentIndex == 0) {
			entity->destroy();
		}
	}
};
