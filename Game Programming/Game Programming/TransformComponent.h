#ifndef TRANSFORM
#define TRANSFORM

#include "ESC.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int speed = 4;
	
	TransformComponent()  {
		position = Vector2D(0, 0);
	}

	TransformComponent(int x, int y) {
		position = Vector2D(float(x), float(y));

	}

	 void update() override {
		 position.x += velocity.x * speed;
		 position.y += velocity.y * speed;
	}

};

#endif
