#ifndef ENEMYMOVEMENT
#define ENEMYMOVEMENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Vector2D.h"
#include "Game.h"

class EnemyComponent : public Component {
private:
	Vector2D initialPosition;
public:
	TransformComponent* position;
	SpriteComponent* sprite;
	
	EnemyComponent() = default;

	void init() override {
		position = &entity->getComponent<TransformComponent>();
		initialPosition.x = position->position.x;
		initialPosition.y = position->position.y;

		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		position->position.x = initialPosition.x - Game::camera.x;
		position->position.y = initialPosition.y - Game::camera.y;
	}
};


#endif