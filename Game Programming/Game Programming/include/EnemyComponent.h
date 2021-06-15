#ifndef ENEMYMOVEMENT
#define ENEMYMOVEMENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Stats.h"
#include "Vector2D.h"
#include "Game.h"
#include <iostream>

using namespace std;

class EnemyComponent : public Component {
private:
	Vector2D initialPosition;
public:
	TransformComponent* position;
	SpriteComponent* sprite;
	TransformComponent* playerPos;
	Stats* stats;
	Vector2D direction;

	Uint32 lastShot = 0;
	int lastDirection = 1;
	bool reloading = false;
	Uint32 reloadFrame;

	EnemyComponent() = default;

	void init() override {
		position = &entity->getComponent<TransformComponent>();
		initialPosition.x = position->position.x;
		initialPosition.y = position->position.y;

		sprite = &entity->getComponent<SpriteComponent>();

		playerPos = &Game::player->getComponent<TransformComponent>();

		stats = &entity->getComponent<Stats>();
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
				//direction.y = distance.y / abs(distance.y);

			}
			if (abs(distance.x) > 400) {
				position->velocity.x = direction.x;
			}
			if (abs(distance.x) < 350) {
				position->velocity.x = 0;
			}

			if (abs(distance.x) < 400) {
				shoot(stats->getWeapon());
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
			Game::assetManager->createProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction.x,0),Game::groupEnemyProjectiles);
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
			cout << "start reloading" << endl;
		}
		else {
			Uint32 current = SDL_GetTicks();
			if (current - reloadFrame >= stats->getWeapon().reloadTime) {
				stats->getWeapon().reload();
				reloading = false;
				cout << "reloading done" << endl;
			}
		}
	}
};


#endif