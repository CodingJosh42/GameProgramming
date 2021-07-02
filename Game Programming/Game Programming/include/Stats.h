#ifndef STATS
#define STATS

#include "ESC.h"
#include "Weapons.h"
#include "Vector2D.h"
#include <SDL.h>
#include "Game.h"
#include "TextureManager.h"
#include "UILabel.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Stats : public Component {
private:
	// Health display
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Texture* healthbar;
	int counter = 0;

	// Ammo display
	SDL_Rect ammoSrc;
	SDL_Rect ammoDest;
	UILabel displayAmmo;
	SDL_Texture* ammoSymbol;

	int maxHealth;
	int currentHealth;
	Weapon* equippedWeapon;
	Weapon primaryWeapon;
	Weapon secondaryWeapon;
	int speed;
	int crouchSpeed;
	bool drawTex;
	Uint32 lastDamageFrame = 0;
	int regenTime = 10000;

public:
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
			src.x = 0;
			src.y = 0;
			src.w = 3 * 32;
			src.h = 32;

			dest.x = 800 - 4 * 32;
			dest.y = 0;
			dest.w = 3 * 32;
			dest.h = 32;

			ammoSrc.x = 0;
			ammoSrc.y = 0;
			ammoSrc.w = 32;
			ammoSrc.h = 32;

			ammoDest.x = 32;
			ammoDest.y = 0;
			ammoDest.w = 32;
			ammoDest.h = 32;


			healthbar = Game::assetManager->getTexture("heart");
			ammoSymbol = Game::assetManager->getTexture("ammo");

			SDL_Color red = { 255,0,0,255 };
			displayAmmo = UILabel(32*2 + 4, 0, "","arial", red);
		}
	}

	void update() override {
		if (drawTex ) {

			stringstream ammo;
			int currentAmmo = equippedWeapon->currentAmmo;
			if (currentAmmo < 10) {
				ammo << "0";
			}
			ammo << currentAmmo;
			displayAmmo.setLabelText(ammo.str());

			if (currentHealth == 0) {
				//Game::gameOver = true;
			}

			if (currentHealth < maxHealth) {
				Uint32 currentFrame = SDL_GetTicks();
				if (currentFrame - lastDamageFrame >= regenTime) {
					currentHealth++;
					lastDamageFrame = currentFrame;
				}
			}
		}
	}

	void draw() override {
		if (drawTex) {
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
	*/
	void reduceHealth(int damage) {
		currentHealth -= damage;
		if (currentHealth < 0) {
			currentHealth = 0;
		}
		lastDamageFrame = SDL_GetTicks();
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