#ifndef TRANSFORM
#define TRANSFORM

#include "ESC.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int width = 32;
	int height = 32;
	int scale = 4;

	int speed = 4;
	
	TransformComponent()  {
		position = Vector2D(0, 0);
	}

	TransformComponent(int x, int y) {
		position = Vector2D(float(x), float(y));

	}
	TransformComponent(Vector2D position, int width, int height, int scale) : position{ position } ,width { width }, height{ height }, scale{ scale } {
	}

	TransformComponent(int x, int y, int width, int height, int scale) : width{ width }, height{ height }, scale{ scale } {
		position = Vector2D((float)x, (float)y);

	}

	 void update() override {
		 position.x += velocity.x * speed;
		 position.y += velocity.y * speed;
	}

};

#endif
