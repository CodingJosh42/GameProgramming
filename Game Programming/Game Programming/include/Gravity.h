#ifndef GRAVITY
#define GRAVITY

#include "ESC.h"
#include "ColliderComponent.h"
#include "Game.h"
#include "TileComponent.h"
#include "TransformComponent.h"
#include "Collision.h"
#include <vector>
#include <iostream>

using namespace std;

class GravityComponent : public Component {
public:
	bool collision = false;
	bool flying = false;
	int jumpHeight = 640;
	bool ignoreCollision = false;

	TransformComponent* position;

	void init() override {
		position = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		gravity();
		checkCollision();
	}

	void checkCollision() {
		ColliderComponent enemyCollider = entity->getComponent<ColliderComponent>();
		vector<Entity*> tiles = Game::manager.getGroup(Game::groupTile);

		bool tempCollision = false;
		for (Entity* tile : tiles) {
			if (tile->getComponent<TileComponent>().tag == "terrain") {
				ColliderComponent collider = tile->getComponent<ColliderComponent>();
				if (Collision::AABB(enemyCollider, collider)) {
					Collision::CollisionType collision = Collision::yCollision(enemyCollider, collider);
					if (collision == Collision::TOP) {
						tempCollision = true;
					}
					if (collider.collider.y < position->position.y + position->height * position->scale - 32) {

						collision = Collision::xCollision(enemyCollider, collider);
						if (collision == Collision::LEFT) {
							entity->getComponent<TransformComponent>().position.x = position->position.x;
							jump();
						}
						if (collision == Collision::RIGHT) {
							entity->getComponent<TransformComponent>().position.x = position->position.x;
							jump();
						}
					}

				}
			}
		}
		this->collision = tempCollision;
	}

	/*
	* Player jumps if he is on a tile
	*/
	void jump()
	{
		// Only Jump if on the Ground
		if (!flying) {
			position->velocity.y = -3;
			flying = true;
			jumpHeight = position->position.y;
			ignoreCollision = true;
			// sprite->setAnimation("jumping");

			if (position->velocity.x == -1) {
				// sprite->flip = SDL_FLIP_HORIZONTAL;
			}
			else if (position->velocity.x == 1) {
				// sprite->flip = SDL_FLIP_NONE;
			}
			else {
				/*if (lastDirection == 1) {
					// sprite->flip = SDL_FLIP_NONE;
				}
				else {
					// sprite->flip = SDL_FLIP_HORIZONTAL;
				} */
			}
		}
	}

	void gravity() {
		if (!flying) {
			if (!collision) {
				position->velocity.y = 3;
			}
			else {
				jumpHeight = position->position.y;
				position->velocity.y = 0;
			}
		}
		// Enemy jumping
		if (flying) {
			// Player can jump 200 pixels
			if (position->position.y <= jumpHeight - 200) {
				if (!collision) {
					position->velocity.y = 3;
					ignoreCollision = false;
				}
			}
			// Enemy is falling down
			if (position->velocity.y > 0 && ignoreCollision == false) {
				if (collision) {
					flying = false;
					position->velocity.y = 0;
				}
			}
		}
	}
};


#endif