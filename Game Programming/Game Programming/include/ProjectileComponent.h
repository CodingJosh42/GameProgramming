#ifndef PROJECTILE
#define PROJECTILE

#include "ESC.h"
#include "TransformComponent.h"
#include "Game.h"


class ProjectileComponent : public Component {

private:
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;

	TransformComponent* position;
public:
	/*
	* Constructor of Component
	* @param range Range of projectile
	* @param speed Speed of projectile
	* @param velocity Direction of projectile
	*/
	ProjectileComponent(int range, int speed, Vector2D velocity) : range{ range }, speed{ speed }, velocity{ velocity } {

	}


	void init() override {
		position = &entity->getComponent<TransformComponent>();
		position->velocity = velocity;
		position->speed = speed;
	}

	void update() override {
		distance += speed;

		if (distance > range) {
			entity->destroy();
		} else if (position->position.x > Game::camera.x + Game::camera.w ||
				 position->position.x < Game::camera.x ||
				 position->position.y > Game::camera.y + Game::camera.h ||
				 position->position.y < Game::camera.y) {
			entity->destroy();
		}
	}
};

#endif