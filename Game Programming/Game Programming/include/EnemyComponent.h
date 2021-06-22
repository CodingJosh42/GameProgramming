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


using namespace std;

class EnemyComponent : public Component {
private:
	Uint32 lastShot = 0;
	int lastDirection = 1;
	int lastX;
	bool reloading = false;
	Uint32 reloadFrame;


	Stats* stats;
	TransformComponent* position;
	SpriteComponent* sprite;
	TransformComponent* playerPos;

	Vector2D initialPosition;
	
	int range;

public:
	Vector2D direction;

	typedef enum EnemyType {
		EASY,
		SNIPER
	} EnemyType;


	bool flying = false;
	EnemyType type;
	

	EnemyComponent() = default;

	EnemyComponent(EnemyType type) : type{ type } {
	};


	void init() override {
		position = &entity->getComponent<TransformComponent>();

		sprite = &entity->getComponent<SpriteComponent>();

		playerPos = &Game::player->getComponent<TransformComponent>();

		stats = &entity->getComponent<Stats>();
		range = stats->getWeapon().range;
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
			if (abs(distance.x) > range) {
				position->velocity.x = direction.x;
			}
			if (abs(distance.x) < range - 50) {
				position->velocity.x = 0;
				lastX = Game::camera.x;
				initialPosition = position->position;
			}

			if (abs(distance.x) < range) {
				shoot(stats->getWeapon());
			}
			if (position->velocity.x == 0) {
				if (lastX != Game::camera.x) {
					position->position.x = initialPosition.x + (lastX - Game::camera.x);
				}
			}
		}
		
		updateTextures();
		
	}

	/**
	* Enemy is shooting a projectile
	* @param delay Delay between the single shots
	* @param range Range of the projectile
	* @param speed Speed of the projectile
	*/
	void shoot(Weapon weapon)
	{
		Uint32 currentTick = SDL_GetTicks();
		if (stats->getWeapon().currentAmmo <= 0) {
			reload();
		}
		else if (currentTick - lastShot > weapon.delay && !reloading) {

			int xStart = 0;

			if (direction.x == -1) {
				xStart = position->position.x;
			}
			else {
				xStart = position->position.x + position->width * position->scale;
			}
			Vector2D projetilePos = Vector2D(xStart, position->position.y + position->height / 2 * position->scale);
			switch (type) {
			case EASY:
				Game::assetManager->createProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x, direction.y), Game::groupEnemyProjectiles);
				break;
			case SNIPER:
				Game::assetManager->createSniperProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x, direction.y), Game::groupEnemyProjectiles);
				break;
			default:
				break;
			}
			
			stats->getWeapon().reduceAmmo();
			lastShot = currentTick;
		}
	}

	/*
	* Enemy is reloading his weapon. After reloading time weapon has maxAmmo again
	*/
	void reload() {
		if (!reloading) {
			reloading = true;
			reloadFrame = SDL_GetTicks();
		}
		else {
			Uint32 current = SDL_GetTicks();
			if (current - reloadFrame >= stats->getWeapon().reloadTime) {
				stats->getWeapon().reload();
				reloading = false;
			}
		}
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