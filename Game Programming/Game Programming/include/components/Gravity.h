#ifndef GRAVITY
#define GRAVITY

#include "ESC.h"
#include "ColliderComponent.h"
#include "../Game.h"
#include "TileComponent.h"
#include "TransformComponent.h"
#include "../Collision.h"
#include "SpriteComponent.h"
#include <vector>
#include <iostream>
#include "../Vector2D.h"
#include "Stats.h"

using namespace std;

class GravityComponent : public Component {
public:
	bool collision = false;
	bool flying = false;
	int jumpHeight = -1;
	bool ignoreCollision = false;
	bool firstupdate = true;
	int lastY;
	int* lastX;
	Vector2D* initialPosition;
	Uint32 lastBottom = 0;

	TransformComponent* position;
	TransformComponent lastPosition;
	ColliderComponent* collider;
	ColliderComponent lastCollider;
	SpriteComponent* sprite;
	EnemyComponent* enemyComponent;
	Stats* stats;

	void init() override {
		position = &entity->getComponent<TransformComponent>();
		lastPosition = *position;
		collider = &entity->getComponent<ColliderComponent>();
		lastCollider = *collider;
		sprite = &entity->getComponent<SpriteComponent>();
		enemyComponent = &entity->getComponent<EnemyComponent>();
		lastX = &enemyComponent->lastX;
		initialPosition = &enemyComponent->initialPosition;
		stats = &entity->getComponent<Stats>();
		lastY = Game::camera.y;
	}

	void update() override {
		if (firstupdate) {
			lastY = Game::camera.y;
			firstupdate = false;
		}
		if (lastY != Game::camera.y) {
			int diff = (lastY - Game::camera.y);
			position->position.y = position->position.y + diff;
			jumpHeight += diff;
			lastPosition.position.y += diff;
			lastY = Game::camera.y;
		}
		if (position->velocity.x == 0) {

			lastPosition.position.x = position->position.x;
		}
		
		gravity();
		checkCollision();
	}

	/*
	* Check for collision. If top collision stop movement in y-direction. If right or left collision try to jump over obstacle
	*/
	void checkCollision() {
		ColliderComponent enemyCollider = lastCollider;
		lastCollider = *collider;
		vector<Entity*> tiles = Game::manager.getGroup(Game::groupTileColliders);

		bool tempCollision = false;
		for (Entity* tile : tiles) {
			ColliderComponent collider = tile->getComponent<ColliderComponent>();
			if (Collision::AABB(enemyCollider, collider)) {
				Collision::CollisionType collision = Collision::yCollision(enemyCollider, collider);

				if (collision == Collision::TOP) {
					// 3 * stats->getSpeed() + 1
					if (flying && collider.collider.y < position->position.y + (position->height * position->scale) - (16)) {
						collision = Collision::xCollision(enemyCollider, collider);
						if (collision == Collision::LEFT) {
							position->velocity.x = -1;
						}
						if (collision == Collision::RIGHT) {
							position->velocity.x = 1;
						}
						if (collision == Collision::NONE) {
							ignoreCollision = false;
							tempCollision = true;
							position->position.y = lastPosition.position.y;
						}
					}
					else {
						tempCollision = true;
						ignoreCollision = false;
						position->position.y = lastPosition.position.y;
					}
				}
				if (collision == Collision::BOTTOM && flying) {
					position->velocity.y = 1;
					lastBottom = SDL_GetTicks();
				}

				if (jumpHeight != -1) {
					if (collider.collider.y < position->position.y + (position->height * position->scale) - (16) && !flying) {

						collision = Collision::xCollision(enemyCollider, collider);
						Uint32 current = SDL_GetTicks();
						if (collision == Collision::LEFT) {
							//entity->getComponent<TransformComponent>().position.x = lastPosition.position.x;
							entity->getComponent<TransformComponent>().velocity.x = -1;
							if (current > lastBottom + 1000) {
								position->position.x = lastPosition.position.x;
								position->velocity.x = 0;
								jump();
							}
						}
						if (collision == Collision::RIGHT) {
							//entity->getComponent<TransformComponent>().position.x = lastPosition.position.x;
							entity->getComponent<TransformComponent>().velocity.x = 1;
							if (current > lastBottom + 1000) {
								position->position.x = lastPosition.position.x;
								position->velocity.x = 0;
								jump();
							}
						}
					}
				}

			}
		}
		this->collision = tempCollision;
		lastPosition = *position;
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
			enemyComponent->flying = true;
			jumpHeight = position->position.y;
			ignoreCollision = true;
			sprite->setAnimation("jumping");

			if (position->velocity.x == -1) {
				sprite->flip = SDL_FLIP_HORIZONTAL;
			}
			else if (position->velocity.x == 1) {
				sprite->flip = SDL_FLIP_NONE;
			}
			else {
				if (enemyComponent->direction.x == 1) {
					sprite->flip = SDL_FLIP_NONE;
				}
				else {
					sprite->flip = SDL_FLIP_HORIZONTAL;
				}
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
					cout << position->velocity.x << endl;
					ignoreCollision = false;
				}
			}
			else if(position->position.y <= jumpHeight - 150) {
				position->velocity.x = enemyComponent->direction.x;
			}
			else {
				position->velocity.x = 0;
			}
			// Enemy is falling down
			if (position->velocity.y > 0 && ignoreCollision == false) {
				if (collision) {
					flying = false;
					enemyComponent->flying = false;
					position->velocity.y = 0;
				}
			}
		}
	}
};


#endif