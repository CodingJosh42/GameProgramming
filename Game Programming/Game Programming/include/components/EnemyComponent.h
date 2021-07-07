#ifndef ENEMYMOVEMENT
#define ENEMYMOVEMENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Stats.h"
#include "../Vector2D.h"
#include "../Game.h"
#include <iostream>
#include <vector>
#include "ESC.h"
#include "ColliderComponent.h"
#include "../Collision.h"
#include "TileComponent.h"
#include <SDL_mixer.h>

using namespace std;

class EnemyComponent : public Component {
private:
	int id;
	// For shooting
	Uint32 lastShot = 0;
	int range;
	int lastDirection = 1;
	// For reloading
	bool reloading = false;
	Uint32 reloadFrame;

	Stats* stats;
	TransformComponent* position;
	SpriteComponent* sprite;
	TransformComponent* playerPos;

	// sounds
	Mix_Chunk* easyEnemyShot;
	Mix_Chunk* sniperShot;
	Mix_Chunk* deathScream;
	
	bool firstUpdate = true;
public:
	// Vars

	Vector2D direction;
	// camera vars
	int lastX;
	Vector2D initialPosition;
	int diff = 0;

	// Enemy type
	typedef enum EnemyType {
		EASY,
		SNIPER
	} EnemyType;
	EnemyType type;

	bool flying = false;

	// Methods

	EnemyComponent() = default;

	EnemyComponent(EnemyType type, int id) : type{ type }, id{ id }{
	};


	void init() override {
		position = &entity->getComponent<TransformComponent>();

		sprite = &entity->getComponent<SpriteComponent>();

		playerPos = &Game::player->getComponent<TransformComponent>();

		stats = &entity->getComponent<Stats>();
		range = stats->getWeapon().range;
		// set camera vars
		initialPosition = position->position;
		lastX = Game::camera.x;
		// Sounds
		easyEnemyShot = Game::assetManager->getSound("easyEnemyShot");
		sniperShot = Game::assetManager->getSound("sniperShot");
		deathScream = Game::assetManager->getSound("death");
	}

	void update() override {
		// set camera vars
		if (firstUpdate) {
			lastX = Game::camera.x;
			firstUpdate = false;
		}
		Vector2D pos = playerPos->position;
		Vector2D distance = pos - position->position;
		// Destroy entity if no health left
		if (stats->getCurrentHealth() <= 0) {
			Mix_PlayChannel(-1, deathScream, 0);
			entity->destroy();
		}
		else {
			// Calc direction
			if (distance.x != 0) {
				direction.x = distance.x / abs(distance.x);
				direction.y = distance.y / abs(distance.x);
			}
			// Start running if in range
			int realDist = sqrt(pow(distance.x, 2) + pow(distance.y, 2));
			if (abs(distance.x) > range && type == EASY ) {
				if (realDist < 1.5 * range) {
					position->velocity.x = direction.x;
				}
				else {
					if (position->velocity.x != 0) {
						position->velocity.x = 0;
						// Update camera vars
						lastX = Game::camera.x;
						initialPosition = position->position;	
					}
				}
				
			}
			// Stop running if in range
			if (abs(realDist) < range - 50 ) {
				if (position->velocity.x != 0) {
					position->velocity.x = 0;
					// Update camera vars
					lastX = Game::camera.x;
					initialPosition = position->position;
				}
			}
			// Position updated when standing still based on camera
			if (position->velocity.x == 0) {
				if (lastX != Game::camera.x) {
					diff = lastX - Game::camera.x;
					position->position.x = initialPosition.x + 0.5 * diff;
				}
			}
			// shoot if in range
			if (realDist < range) {
				shoot(stats->getWeapon());
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
	void shoot(Weapon& weapon)
	{
		Uint32 currentTick = SDL_GetTicks();
		if (stats->getWeapon().currentAmmo <= 0) {
			reload();
		}
		else if (currentTick - lastShot > weapon.delay && !reloading) {
			// Shoot when angle is not too large
			if (abs(atan(direction.y) * 180 / M_PI) < 30) {
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
					Mix_PlayChannel(-1, easyEnemyShot, 0);
					Game::assetManager->createProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x, direction.y), Game::groupEnemyProjectiles);
					break;
				case SNIPER:
					Mix_PlayChannel(-1, sniperShot, 0);
					Game::assetManager->createSniperProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x, direction.y), Game::groupEnemyProjectiles);
					break;
				default:
					break;
				}

				stats->getWeapon().reduceAmmo();
				lastShot = currentTick;
			}
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