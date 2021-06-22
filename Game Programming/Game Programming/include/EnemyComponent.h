#ifndef ENEMYMOVEMENT
#define ENEMYMOVEMENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Stats.h"
#include "Vector2D.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include "ESC.h"
#include "ColliderComponent.h"
#include "Collision.h"
#include "TileComponent.h"
#include "Enemy.h"
#include "EasyEnemy.h"

using namespace std;

class EnemyComponent : public Component {

public:
	TransformComponent* position;
	SpriteComponent* sprite;
	TransformComponent* playerPos;
	Stats* stats;
	Vector2D direction;

	typedef enum EnemyType {
		EASY,
		SNIPER
	} EnemyType;

	bool flying = false;
	Enemy* enemyType;
	

	EnemyComponent() = default;

	EnemyComponent(EnemyType type) {
		switch (type) {
		case EASY:
			this->enemyType = new EasyEnemy();
		}
		
	};

	~EnemyComponent() {
		delete enemyType;
	}

	void init() override {
		position = &entity->getComponent<TransformComponent>();

		sprite = &entity->getComponent<SpriteComponent>();

		playerPos = &Game::player->getComponent<TransformComponent>();

		stats = &entity->getComponent<Stats>();

		enemyType->init(stats, position);
	}

	void update() override {
		Vector2D pos = playerPos->position;
		Vector2D distance = pos - position->position;
		if (stats->getCurrentHealth() <= 0) {
			entity->destroy();
		}
		else {
			if (distance.x != 0) {
				direction.x = distance.x / abs(distance.x);
				direction.y = distance.y / abs(distance.x);

			}
			enemyType->movement(distance, direction);
		}
		
		updateTextures();
		
	}

	


	/**
	* Updates Textures depending on what buttons are still pressed
	*
	*/
	void updateTextures()
	{
		// Update Textures if buttons still pressed
		if (!flying) {
			

			// Standing
			if (position->velocity.x == 0) {
				sprite->setAnimation("standing");
			}
			//  Walking
			else {
				sprite->setAnimation("walking");
			}

		}
		// RIGHT
		if (direction.x == 1) {
			sprite->flip = SDL_FLIP_NONE;
		}

		// LEFT
		if (direction.x == -1) {
			sprite->flip = SDL_FLIP_HORIZONTAL;

		}

	}
	
};


#endif