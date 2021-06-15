#ifndef WEAPONCOMPONENT
#define WEAPONCOMPONENT

#include "ESC.h"
#include "TransformComponent.h"
#include "Stats.h"
#include "TextureManager.h"
#include "Game.h"

class WeaponComponent : public Component {
public:
	TransformComponent* position;
	Stats* stats;
	SDL_Rect dest;
	int xoffset = 21;
	int yoffset = 14;
	SDL_Texture* texture;

	WeaponComponent() = default;

	WeaponComponent(int xoffset, int yoffset): xoffset {xoffset}, yoffset{yoffset} {}

	void init() override {
		position = &entity->getComponent<TransformComponent>();
		stats = &entity->getComponent<Stats>();

		Weapon weapon = stats->getWeapon();
		dest.x = position->position.x + xoffset * weapon.scale;
		dest.y = position->position.y + yoffset * weapon.scale;
		dest.w = weapon.width * weapon.scale;
		dest.h = weapon.height * weapon.scale;

		texture = Game::assetManager->getTexture(weapon.texture);
	}

	void update() override {
		Weapon weapon = stats->getWeapon();
		dest.x = position->position.x + xoffset * weapon.scale;
		dest.y = position->position.y + yoffset * weapon.scale;
		dest.w = weapon.width * weapon.scale;
		dest.h = weapon.height * weapon.scale;

		texture = Game::assetManager->getTexture(weapon.texture);
	}

	void draw() override {
		Weapon weapon = stats->getWeapon();
		SDL_RendererFlip flip;
		if (position->velocity.x == 1) {
			flip = SDL_FLIP_NONE;
		}
		else if (position->velocity.x == -1) {
			flip = SDL_FLIP_HORIZONTAL;
		}
		else {
			if (position->lastDirection == 1) {
				flip = SDL_FLIP_NONE;
			}
			else {
				flip = SDL_FLIP_HORIZONTAL;
			}
			if ((int)((SDL_GetTicks() / 2) % 200) == 1) {
				dest.y = dest.y + 2 * weapon.scale;
			}
		}
		TextureManager::DrawTexture(texture, weapon.src, dest, flip);
	}
};


#endif