#ifndef KEYBOARD
#define KEYBOARD

#include "Game.h"
#include "ESC.h"
#include "TransformComponent.h"
#include <SDL.h>

class KeyboardController : public Component {
private:
	SDL_Texture* player;
	SDL_Texture* playerCrouching;
	SDL_Texture* playerFlying;
	

public:
	// For gravity
	bool flying = false;
	bool collision = false;
	bool ignoreCollision = false;
	int jumpHeight;

	TransformComponent* position;
	SpriteComponent* sprite;

	void init() override {
		position = &(entity->getComponent<TransformComponent>());
		sprite = &(entity->getComponent<SpriteComponent>());

		player = TextureManager::LoadTexture("assets/animation_player.png");
	}

	~KeyboardController() {
		SDL_DestroyTexture(player);
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
			if (position->velocity.y > 0) {
				if (collision) {
					flying = false;
					position->velocity.y = 0;
				}
			}
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


		catch_KeyDown();

		catch_KeyUp();


		updateTextures();

		
	}

	/**
	* Checks Key Down events
	*
	*/
	void catch_KeyDown()
	{
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				// Only Jump if on the Ground
				if (flying == false && position->speed > 1) {
					position->velocity.y = -3;
					sprite->setAnimation("jumping");
					flying = true;
					ignoreCollision = true;
					jumpHeight = position->position.y;

					if (position->velocity.x == -1) {
						sprite->flip = SDL_FLIP_HORIZONTAL;
					}
					else {
						sprite->flip = SDL_FLIP_NONE;
					}
				}
				break;
			case SDLK_LCTRL:
				if (collision) {
					if (position->velocity.x == -1) {
						sprite->flip = SDL_FLIP_HORIZONTAL;
					}
					else {
						sprite->flip = SDL_FLIP_NONE;
					}
					position->speed = 1;
				}
				break;
			default:
				break;
			}
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
				position->speed = 4;
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
			if (position->speed <= 1) {
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

			// RIGHT
			if (position->velocity.x == 1) {
				sprite->flip = SDL_FLIP_NONE;
			}

			// LEFT
			if (position->velocity.x == -1) {
				sprite->flip = SDL_FLIP_HORIZONTAL;

			}
		}
	}
};


#endif
