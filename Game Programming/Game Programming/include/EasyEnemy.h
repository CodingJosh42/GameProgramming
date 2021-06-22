#ifndef EASYENEMY
#define EASYENEMY

#include "Weapons.h"
#include <SDL.h>
#include "Enemy.h"
#include "TransformComponent.h"
#include "Stats.h"
#include "Vector2D.h"
#include <iostream>

class EasyEnemy : public Enemy {
private:
	Uint32 lastShot = 0;
	int lastDirection = 1;
	int lastX ;
	
public:
	EasyEnemy() {
		cout << "Easy Enemy" << endl;
	}
	bool reloading = false;
	Uint32 reloadFrame;
	Stats* stats;
	TransformComponent* position;
	Vector2D initialPosition;
	Vector2D direction;
	Vector2D distance;

	void init(Stats* stats, TransformComponent* position) {
		this->stats = stats;
		this->position = position;
		initialPosition = position->position;
		cout << "init" << endl;
	}

	void movement(Vector2D& distance, Vector2D& direction) final {
		this->direction = direction;


		cout << "movement" << endl;
		if (abs(distance.x) > 400) {
			position->velocity.x = direction.x;
		}
		if (abs(distance.x) < 350) {
			position->velocity.x = 0;
			lastX = Game::camera.x;
			initialPosition = position->position;
		}

		if (abs(distance.x) < 400) {
			shoot(stats->getWeapon());
		}
		if (position->velocity.x == 0) {
			if (lastX != Game::camera.x) {
				position->position.x = initialPosition.x + (lastX - Game::camera.x);
			}
		}

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
			Game::assetManager->createProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x, direction.y), Game::groupEnemyProjectiles);
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
};

#endif