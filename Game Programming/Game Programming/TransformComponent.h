#ifndef TRANSFORM
#define TRANSFORM

#include "ESC.h"
#include "Vector2D.h"

class TransformComponent : public Component {
private:
	void (*move) (float* xpos, float* ypos);

public:
	Vector2D position;
	
	TransformComponent()  {
		position = Vector2D(0, 0);
	}

	TransformComponent(int x, int y, void(*move) (float* xpos, float* ypos)) {
		position = Vector2D(float(x), float(y));
		this->move = move;
	}

	 void update() override {
		 move(&(position.x), &(position.y));
	}

};

#endif
