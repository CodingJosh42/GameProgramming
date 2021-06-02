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
	int xOffset = 0;
	int yOffset = 0;
	int width;
	int height;

	ColliderComponent(string tag) : tag {tag} {}

	ColliderComponent(string tag, int Xoffset, int Yoffset, int width, int height) : tag{ tag }, xOffset{ Xoffset }, yOffset{ Yoffset }, width{ width }, height{ height } {}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}

		position = &(entity->getComponent<TransformComponent>());
		Game::colliders.push_back(this);
	}

	void update() override {
		collider.x = (int)position->position.x + (xOffset * position->scale);
		collider.y = (int)position->position.y + (yOffset * position->scale);
		if (xOffset == 0) {
			width = position->width;
		}
		if (yOffset == 0) {
			height = position->height;
		}
		
		collider.w = width * position->scale;
		collider.h = height * position->scale;
	}
};

#endif

