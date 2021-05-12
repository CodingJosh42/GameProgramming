#ifndef KEYBOARD
#define KEYBOARD

#include "Game.h"
#include "ESC.h"
#include "TransformComponent.h"

class KeyboardController : public Component {
public:
	TransformComponent* position;
	SpriteComponent* sprite;
	int maxHeight = 0;
	KeyboardController(int maxHeight) : maxHeight{ maxHeight } {
	}

	void init() override {
		position = &(entity->getComponent<TransformComponent>());
		sprite = &(entity->getComponent<SpriteComponent>());
	}

	void update() override {
		// Add Gravity
		if (position->position.y <= maxHeight - 280) {
			position->velocity.y = 3;
		}
		else if(position->position.y >= maxHeight - 128){
			position->velocity.y = 0;
		}


		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				// Only Jump if on the Ground
				if (position->position.y >= maxHeight -128 && position->speed > 1) {
					position->velocity.y = -3;
				}
				break;
			case SDLK_d:
				if (position->speed > 1) {
					sprite->setTexture("assets/player.png");
				}
				else {
					sprite->setTexture("assets/crouching.png");
				}
				position->velocity.x = 1;
				break;
			case SDLK_a:
				if (position->speed > 1) {
					sprite->setTexture("assets/playerLeft.png");
				}
				else {
					sprite->setTexture("assets/crouchingLeft.png");
				}
				position->velocity.x = -1;
				break;
			case SDLK_LCTRL:
				if (position->position.y >= maxHeight - 128) {
					if (position->velocity.x == -1) {
						sprite->setTexture("assets/crouchingLeft.png");
					}
					else {
						sprite->setTexture("assets/crouching.png");
					}
					position->speed = 1;
				}
				break;
			default:
				break;
			}
		}




		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_d:
				// Make movement smooth
				if (position->velocity.x != -1) {
					position->velocity.x = 0;
				}
				break;
			case SDLK_a:
				// Make movement smooth
				if (position->velocity.x != 1) {
					position->velocity.x = 0;
				}
				break;
			case SDLK_LCTRL:
				if (position->velocity.x == -1) {
					sprite->setTexture("assets/playerLeft.png");
				}
				else {
					sprite->setTexture("assets/player.png");

				}
					position->speed = 4;
				break;
			default:
				break;
			}
		}
		
	}
};


#endif
