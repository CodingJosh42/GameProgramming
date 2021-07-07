#pragma once
#include "ESC.h"
#include "Stats.h"
#include <vector>
#include "../Game.h"
#include "../Numbers.h"
#include "../UILabel.h"
#include <SDL.h>
#include "../TextureManager.h"

class HUDComponent : public Component {
private:
	Entity* player;
	Stats* stats;

	// Health display
	SDL_Rect healthSrc;
	SDL_Rect healthDest;
	SDL_Texture* healthSymbol;

	// Ammo display
	SDL_Rect ammoSrc;
	SDL_Rect ammoDest;
	UILabel ammoDisplay;
	SDL_Texture* ammoSymbol;

	// Enemy display
	vector<Entity*> enemys;
	UILabel enemyDisplay;
public:
	HUDComponent() = default;

	HUDComponent(Entity* player) : player{ player } {}

	void init() {
		// Set up HUD
		healthSrc.x = 0;
		healthSrc.y = 0;
		healthSrc.w = 3 * TILESIZE;
		healthSrc.h = TILESIZE;

		healthDest.x = SCREENWIDTH - 4 * TILESIZE;
		healthDest.y = 0;
		healthDest.w = 3 * TILESIZE;
		healthDest.h = TILESIZE;

		ammoSrc.x = 0;
		ammoSrc.y = 0;
		ammoSrc.w = TILESIZE;
		ammoSrc.h = TILESIZE;

		ammoDest.x = TILESIZE;
		ammoDest.y = 0;
		ammoDest.w = TILESIZE;
		ammoDest.h = TILESIZE;

		stats = &player->getComponent<Stats>();
		// Textures
		healthSymbol = Game::assetManager->getTexture("heart");
		ammoSymbol = Game::assetManager->getTexture("ammo");
		// Ammo label
		SDL_Color red = { 255,0,0,255 };
		ammoDisplay = UILabel(TILESIZE * 2 + 4, 0, "", "arial32bold", red);
		// Enemy label
		enemyDisplay = UILabel(SCREENWIDTH / 2, 0, "Gegner: XX", "arial32bold", red);
		SDL_Rect pos = enemyDisplay.getPosition();
		enemyDisplay.setPosition(pos.x - pos.w / 2, pos.y);
	}

	void update() {
		// Update Ammo UILabel
		stringstream ammo;
		int currentAmmo = stats->getWeapon().currentAmmo;
		if (currentAmmo < 10) {
			ammo << "0";
		}
		ammo << currentAmmo;
		ammoDisplay.setLabelText(ammo.str());

		// Update enemy UILabel
		enemys = Game::manager.getGroup(Game::groupEnemy);
		stringstream currentEnemys;
		int size = enemys.size();
		currentEnemys << "Gegner: ";
		if (size < 10) {
			currentEnemys << "0";
		}
		currentEnemys << size;
		enemyDisplay.setLabelText(currentEnemys.str());
	}

	void draw() override {
			// Draw HUD symbols
			int index = stats->getMaxHealth() - stats->getCurrentHealth();
			healthSrc.y = index * 32;
			TextureManager::DrawTexture(healthSymbol, healthSrc, healthDest, SDL_FLIP_HORIZONTAL);
			TextureManager::DrawTexture(ammoSymbol, ammoSrc, ammoDest);
			// Draw labels
			ammoDisplay.draw();
			enemyDisplay.draw();
	}
};