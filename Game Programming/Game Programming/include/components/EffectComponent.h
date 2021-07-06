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
	EffectComponent(Animation anim, int width) : anim{ anim }, width{ width } {

	}
	void update() override {
		int currentIndex = width * (int)((SDL_GetTicks() / anim.delay) % anim.frames);
		if (currentIndex > 0) {
			firstTime = false;
		}
		if (!firstTime && currentIndex == 0) {
			entity->destroy();
		}
	}
};
