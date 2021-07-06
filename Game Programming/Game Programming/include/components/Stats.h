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
	bool drawTex = false;
	// Health display
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Texture* healthbar;
	Uint32 lastCombatFrame = 0;
	int regenTime = 5000;

	// Ammo display
	SDL_Rect ammoSrc;
	SDL_Rect ammoDest;
	UILabel displayAmmo;
	SDL_Texture* ammoSymbol;

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
	Stats(int maxHealth, Weapon weapon, int speed, int crouchSpeed, bool draw) : maxHealth{ maxHealth }, primaryWeapon{ weapon }, speed{ speed }, crouchSpeed{ crouchSpeed }, drawTex{ draw } {
		currentHealth = maxHealth;
		equippedWeapon = &primaryWeapon;
	}

	void init() override {
		if (drawTex) {
			// Set up HUD
			src.x = 0;
			src.y = 0;
			src.w = 3 * TILESIZE;
			src.h = TILESIZE;

			dest.x = SCREENWIDTH - 4 * TILESIZE;
			dest.y = 0;
			dest.w = 3 * TILESIZE;
			dest.h = TILESIZE;

			ammoSrc.x = 0;
			ammoSrc.y = 0;
			ammoSrc.w = TILESIZE;
			ammoSrc.h = TILESIZE;

			ammoDest.x = TILESIZE;
			ammoDest.y = 0;
			ammoDest.w = TILESIZE;
			ammoDest.h = TILESIZE;


			healthbar = Game::assetManager->getTexture("heart");
			ammoSymbol = Game::assetManager->getTexture("ammo");

			SDL_Color red = { 255,0,0,255 };
			displayAmmo = UILabel(TILESIZE*2 + 4, 0, "","arial32bold", red);
		}
	}

	void update() override {
		if (drawTex ) {
			// Update Ammo UILabel
			stringstream ammo;
			int currentAmmo = equippedWeapon->currentAmmo;
			if (currentAmmo < 10) {
				ammo << "0";
			}
			ammo << currentAmmo;
			displayAmmo.setLabelText(ammo.str());
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

	void draw() override {
		if (drawTex) {
			// Draw HUD symbols
			int index = maxHealth - currentHealth;
			src.y = index * 32;
			TextureManager::DrawTexture(healthbar, src, dest, SDL_FLIP_HORIZONTAL);
			TextureManager::DrawTexture(ammoSymbol, ammoSrc, ammoDest);
			displayAmmo.draw();
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
		if (drawTex && !shielding) {
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