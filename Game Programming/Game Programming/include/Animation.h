#ifndef ANIMATION
#define ANIMATION

class Animation {
public:
	int frames;
	int delay;
	int index;

	Animation() = default;

	Animation(int index, int frames, int delay) : index{ index }, frames{ frames }, delay{ delay } {}

};

#endif
