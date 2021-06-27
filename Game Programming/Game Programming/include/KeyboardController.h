#ifndef KEYBOARD
#define KEYBOARD

#include "Game.h"
#include "ESC.h"
#include "TransformComponent.h"
#include <SDL.h>
#include "Vector2D.h"
#include "Weapons.h"
#include "Stats.h"
#include <iostream>
#include "ColliderComponent.h"
#include <SDL_mixer.h>

using namespace std;
class KeyboardController : public Component {
private:

	Stats* stats;
	Uint32 lastShot = 0;
	int lastDirection = 1;
	bool reloading = false;
	Uint32 reloadFrame;
	ColliderComponent* collider;
	int initHeight;
	// sounds
	Mix_Chunk* reloadingSound;
	Mix_Chunk* gunshot;

	/**
	* Checks Key Down events
	*
	*/
	void catch_KeyDown()
	{
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				jump();
				break;
			case SDLK_s:
				if (maxHeight - 32 - 128 > position->position.y) {
					ignoreCollision = true;
					flying = true;
					position->velocity.y = 3;
				}
				break;
			case SDLK_k:
				shoot(stats->getWeapon());
				break;
			case SDLK_LCTRL:
				crouch();
				break;
			case SDLK_1:
				stats->changeWeapon(1);
				sprite->setTexture("playerPistol");
				reloading = false;
				break;
			case SDLK_2:
				stats->changeWeapon(2);
				reloading = false;
				sprite->setTexture("playerMachineGun");
				break;
			case SDLK_r:
				if (!reloading) {
					reload();
				}
			default:
				break;
			}
		}
	}

	/*
	* Player is reloading his weapon. After reloading time weapon has maxAmmo again
	*/
	void reload() {
		if (!reloading) {
			reloading = true;
			reloadFrame = SDL_GetTicks();
			Mix_PlayChannelTimed(-1, reloadingSound, -1, stats->getWeapon().reloadTime);
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
	* Player is crouching if not in the air. Speed is reduced
	*/
	void crouch()
	{
		if (collision) {
			if (position->velocity.x == -1) {
				sprite->flip = SDL_FLIP_HORIZONTAL;
			}
			else if (position->velocity.x == 1) {
				sprite->flip = SDL_FLIP_NONE;
			}
			else {
				if (lastDirection == 1) {
					sprite->flip = SDL_FLIP_NONE;
				}
				else {
					sprite->flip = SDL_FLIP_HORIZONTAL;
				}
			}
			position->speed = stats->getCrouchSpeed();
			collider->height = 22;
			collider->yOffset = 10;
		}
	}

	/*
	* Player jumps if he is on a tile
	*/
	void jump()
	{
		// Only Jump if on the Ground
		if (flying == false && position->speed > stats->getCrouchSpeed()) {
			position->velocity.y = -3;
			sprite->setAnimation("jumping");
			flying = true;
			ignoreCollision = true;
			jumpHeight = position->position.y;

			if (position->velocity.x == -1) {
				sprite->flip = SDL_FLIP_HORIZONTAL;
			}
			else if (position->velocity.x == 1) {
				sprite->flip = SDL_FLIP_NONE;
			}
			else {
				if (lastDirection == 1) {
					sprite->flip = SDL_FLIP_NONE;
				}
				else {
					sprite->flip = SDL_FLIP_HORIZONTAL;
				}
			}
		}
	}

	/**
	* Player is shooting a projectile
	* @param delay Delay between the single shots
	* @param range Range of the projectile
	* @param speed Speed of the projectile
	*/
	void shoot(Weapon weapon)
	{
		Uint32 currentTick = SDL_GetTicks();
		if (stats->getWeapon().currentAmmo <= 0) {
			reload();
		} else if (currentTick - lastShot > weapon.delay && !reloading) {

			int direction = 0;
			int xStart = 0;
			if (position->velocity.x == -1) {
				direction = -1;
				xStart = position->position.x;
			}
			else if (position->velocity.x == 1) {
				direction = 1;
				xStart = position->position.x + position->width * position->scale;
			}
			else {
				direction = lastDirection;
				if (lastDirection == -1) {
					xStart = position->position.x;
				}
				else {
					xStart = position->position.x + position->width * position->scale;
				}
			}
			Vector2D projetilePos = Vector2D(xStart, position->position.y + position->height / 2 * position->scale);
			Mix_PlayChannel(-1, gunshot, 0);
			Game::assetManager->createProjectile(projetilePos, weapon.range, weapon.speed, Vector2D(direction, 0), Game::groupPlayerProjectiles);
			stats->getWeapon().reduceAmmo();
			lastShot = currentTick;
		}
	}


	/**
	* Checks Key Up events
	*
	*/
	void catch_KeyUp()
	{
		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				// TODO
				break;
			case SDLK_d:
				// Make movement smooth
				if (position->velocity.x != -1) {
					position->velocity.x = 0;
					lastDirection = 1;
				}
				break;
			case SDLK_a:
				// Make movement smooth
				if (position->velocity.x != 1) {
					position->velocity.x = 0;
					lastDirection = -1;
				}
				break;
			case SDLK_s:
				ignoreCollision = false;
				break;
			case SDLK_LCTRL:
				position->speed = stats->getSpeed();
				collider->height = initHeight;
				collider->yOffset = 0;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
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
			// Crouching
			if (position->speed <= stats->getCrouchSpeed()) {
				sprite->setAnimation("crouching");
			}
			else {

				// Standing
				if (position->velocity.x == 0) {
					sprite->setAnimation("standing");
				}
				//  Walking
				else {
					sprite->setAnimation("walking");
				}
			}
		}
		// RIGHT
		if (position->velocity.x == 1) {
			sprite->flip = SDL_FLIP_NONE;
		}

		// LEFT
		if (position->velocity.x == -1) {
			sprite->flip = SDL_FLIP_HORIZONTAL;

		}

	}

public:
	// For gravity
	bool flying = false;
	bool collision = false;
	bool ignoreCollision = false;
	bool xCollision = false;
	int maxHeight = 640;
	int jumpHeight = maxHeight - 128;

	TransformComponent* position;
	SpriteComponent* sprite;

	void init() override {
		position = &(entity->getComponent<TransformComponent>());
		sprite = &(entity->getComponent<SpriteComponent>());
		stats = &entity->getComponent<Stats>();
		collider = &entity->getComponent<ColliderComponent>();
		initHeight = collider->height;
		// init sounds
		reloadingSound = Game::assetManager->getSound("reloading");
		gunshot = Game::assetManager->getSound("gunshot");
	}

	void update() override {
		// Add Gravity
		// Player not jumping
		if (!flying) {
			if (!collision) {
				position->velocity.y = 3;
			}
			else {
				jumpHeight = position->position.y;
				position->velocity.y = 0;
			}
		}
		// Player jumping
		if (flying) {
			// Player can jump 200 pixels
			if (position->position.y <= jumpHeight - 200) {
				if (!collision) {
					position->velocity.y = 3;
					ignoreCollision = false;
				}
			}
			// Player is falling down
			if (position->velocity.y > 0 && ignoreCollision == false) {
				if (collision) {
					flying = false;
					position->velocity.y = 0;
				}
			}
		}

		if (position->position.y > maxHeight - 32 - position->height * position->scale) {
			ignoreCollision = false;
		}

		const Uint8* keystate = SDL_GetKeyboardState(NULL);


			//continuous-response keys
			if (keystate[SDL_SCANCODE_D])
			{
				position->velocity.x = 1;
			}
			else if (keystate[SDL_SCANCODE_A])
			{
				position->velocity.x = -1;
			}
			else {
				position->velocity.x = 0;
			}

		if (keystate[SDL_SCANCODE_K])
		{
			shoot(stats->getWeapon());
		}

		if (reloading) {
			reload();
		}

		catch_KeyDown();

		catch_KeyUp();

		updateTextures();
	}

	
};


#endif
