#ifndef TRANSFORM
#define TRANSFORM

#include "ESC.h"
#include "Vector2D.h"
#include "Stats.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int width = 32;
	int height = 32;
	int scale = 4;

	int speed = 4;
	
	/*
	* Constructor of TransformComponent. Default values are: position = 0x0, width = 32, height = 32, scale = 4, speed = 4
	*/
	TransformComponent()  {
		position = Vector2D(0, 0);
	}

	/*
	* Constructor of TransformComponent
	* @param x x coordinate of entity
	* @param y y coordinate of entity
	*/
	TransformComponent(int x, int y) {
		position = Vector2D(float(x), float(y));

	}

	/*
	* Constructor of TransformComponent
	* @param position Position of an entity
	* @param width Width of entity
	* @param height Height of entity
	* @param scale Scale of sprite
	*/
	TransformComponent(Vector2D position, int width, int height, int scale) : position{ position } ,width { width }, height{ height }, scale{ scale } {
	}

	/*
	* Constructor of TransformComponent
	* @param x x coordinate of entity
	* @param y y coordinate of entity
	* @param width Width of entity
	* @param height Height of entity
	* @param scale Scale of sprite
	*/
	TransformComponent(int x, int y, int width, int height, int scale) : width{ width }, height{ height }, scale{ scale } {
		position = Vector2D((float)x, (float)y);

	}

	void init() override {
		if (entity->hasComponent<Stats>()) {
			Stats stats = entity->getComponent<Stats>();
			speed = stats.getSpeed();
		}
	}

	 void update() override {
		 position.x += velocity.x * speed;
		 position.y += velocity.y * speed;
	}

};

#endif
