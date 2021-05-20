#ifndef COLLIDER
#define COLLIDER

#include "Game.h"
#include "ESC.h"
#include <SDL.h>
#include <string>
#include "TransformComponent.h"


using namespace std;

class ColliderComponent : public Component {

public:
	SDL_Rect collider;
	string tag;
	TransformComponent* position;

	ColliderComponent(string tag) : tag {tag} {}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}

		position = &(entity->getComponent<TransformComponent>());
		Game::colliders.push_back(this);
	}

	void update() override {
		collider.x = (int) position->position.x;
		collider.y = (int) position->position.y;
		collider.w = position->width * position->scale;
		collider.h = position->height * position->scale;
	}
};

#endif

