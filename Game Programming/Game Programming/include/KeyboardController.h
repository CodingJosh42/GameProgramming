#ifndef KEYBOARD
#define KEYBOARD

#include "Game.h"
#include "ESC.h"
#include "TransformComponent.h"
#include <SDL.h>

class KeyboardController : public Component {
private:
	SDL_Texture* playerRight;
	SDL_Texture* playerLeft;
	SDL_Texture* playerRightCrouching;
	SDL_Texture* playerLeftCrouching;
public:
	TransformComponent* position;
	SpriteComponent* sprite;
	int maxHeight = 0;

	
	KeyboardController(int maxHeight) : maxHeight{ maxHeight } {
	}

	void init() override {
		position = &(entity->getComponent<TransformComponent>());
		sprite = &(entity->getComponent<SpriteComponent>());

		playerRight = TextureManager::LoadTexture("assets/player.png");
		playerRightCrouching = TextureManager::LoadTexture("assets/crouching.png");
		playerLeft = TextureManager::LoadTexture("assets/playerLeft.png");
		playerLeftCrouching = TextureManager::LoadTexture("assets/crouchingLeft.png");
	}

	~KeyboardController() {
		SDL_DestroyTexture(playerRight);
		SDL_DestroyTexture(playerLeft);
		SDL_DestroyTexture(playerRightCrouching);
		SDL_DestroyTexture(playerLeftCrouching);
	}

	void update() override {

		// Add Gravity
		if (position->position.y <= maxHeight - 300) {
			position->velocity.y = 3;
		}
		else if(position->position.y >= maxHeight - 128){
			position->velocity.y = 0;
		}


		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		//continuous-response keys
		if (keystate[SDL_SCANCODE_D])
		{
			position->velocity.x = 1;
		}
		if (keystate[SDL_SCANCODE_A])
		{
			position->velocity.x = -1;
		}
		


		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				// Only Jump if on the Ground
				if (position->position.y >= maxHeight -128 && position->speed > 1) {
					position->velocity.y = -3;
				}
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


		if (position->speed > 1 && position->velocity.x == 1) {
			sprite->setTexture(playerRight);
		}
		else if (position->speed <= 1 && position->velocity.x == 1){
			sprite->setTexture(playerRightCrouching);
		}

		if (position->speed > 1 && position->velocity.x == -1) {
			sprite->setTexture(playerLeft);
		}
		else if (position->speed <= 1 && position->velocity.x == -1) {
			sprite->setTexture(playerLeftCrouching);
		}

		

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				break;
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
