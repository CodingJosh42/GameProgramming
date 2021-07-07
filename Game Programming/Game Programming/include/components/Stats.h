#ifndef STATS
#define STATS

#include "ESC.h"
#include "../Weapons.h"
#include "../Vector2D.h"
#include <SDL.h>
#include "../Game.h"
#include "../TextureManager.h"
#include "../UILabel.h"
#include <string>
#include <sstream>
#include <iostream>
#include "../Numbers.h"
#include <SDL_mixer.h>

using namespace std;

class Stats : public Component {
private:
	// True if palyer stats
	bool isPlayer = false;
	// Health regeneration
	Uint32 lastCombatFrame = 0;
	int regenTime = 5000;
	// Stats
	int maxHealth;
	int currentHealth;
	Weapon* equippedWeapon;
	Weapon primaryWeapon;
	Weapon secondaryWeapon;
	int speed;
	int crouchSpeed;

public:
	bool shielding = false;

	Stats() = default;

	/*
	* Constructor of Component
	* @param maxHealth Max healt of entity
	* @param weapon Weapon of entity
	* @param speed Speed of entity
	* @param crouchSpeed Speed of entity when crouching
	* @param draw Should the stats be drawn to screen, True for player
	*/
	Stats(int maxHealth, Weapon weapon, int speed, int crouchSpeed, bool isPlayer) : maxHealth{ maxHealth }, primaryWeapon{ weapon }, speed{ speed }, crouchSpeed{ crouchSpeed }, isPlayer{ isPlayer } {
		currentHealth = maxHealth;
		equippedWeapon = &primaryWeapon;
	}

	void update() override {
		if (isPlayer) {
			// Check if player lost the game
			if (currentHealth == 0) {
				if (!Game::easyMode) {
					Game::gameOver = true;
				}
			}
			// Regenerate health if 5 sec past since last combat frame
			if (currentHealth < maxHealth) {
				Uint32 currentFrame = SDL_GetTicks();
				if (shielding) {
					lastCombatFrame = currentFrame;
				}
				if (currentFrame - lastCombatFrame >= regenTime) {
					currentHealth++;
					lastCombatFrame = currentFrame;
				}
			}
		}
	}

	

	/*
	* Add a secondary weapon
	*/
	void addSecondaryWeapon(Weapon weapon) {
		secondaryWeapon = weapon;
	}

	/*
	* Change Equipped Weapom
	* @param weapon If weapon is 1, primary Weapon is equipped, if weapon is 2, secondary is equipped
	*/
	void changeWeapon(int weapon) {
		if (weapon == 1) {
			equippedWeapon = &primaryWeapon;
		}
		else if (weapon == 2) {
			equippedWeapon = &secondaryWeapon;
		}
	}

	/*
	* Reduce Health of player
	* @param damage Damage dealt
	*/
	void reduceHealth(int damage) {
		if (isPlayer && !shielding) {
			Mix_PlayChannel(-1, Game::assetManager->getSound("player_hit"), 0);
		}
		if (!shielding) {
			currentHealth -= damage;
			if (currentHealth < 0) {
				currentHealth = 0;
			}
			lastCombatFrame = SDL_GetTicks();
		}
	}

	int getMaxHealth() {
		return maxHealth;
	}

	int getCurrentHealth() {
		return currentHealth;
	}

	Weapon& getWeapon() {
		return *equippedWeapon;
	}

	int getSpeed() {
		return speed;
	}

	int getCrouchSpeed() {
		return crouchSpeed;
	}

};


#endif