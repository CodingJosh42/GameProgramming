#ifndef TILECOMPONENT
#define TILECOMPONENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <iostream>

class TileComponent : public Component {
public:
	TransformComponent* position;
	SpriteComponent* sprite;

	SDL_Rect dest;
	int tileId;
	const char* path;
	const char* tag;
	Vector2D initialPosition;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) : tileId{ id } {
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		initialPosition.x = x;
		initialPosition.y = y;


		switch (id) {
		case 0:
			path = "assets/sky.png";
			tag = "sky";
			break;
		case 1:
			path = "assets/grass.png";
			tag = "grass";
			break;
		case 2:
			path = "assets/water.png";
			tag = "water";
			break;
		case 3:
			path = "assets/metal.png";
			tag = "metal";
			break;
		case 4:
			path = "assets/dirt.png";
			tag = "dirt";
			break;
		case 5:
			path = "assets/cloud.png";
			tag = "cloud";
			break;
		default:
			break;
		}
	}

	void init() override {
		entity->addComponent<TransformComponent>(dest.x, dest.y, dest.w, dest.h, 1);
		position = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		position->position.x = initialPosition.x - Game::camera.x;
		position->position.y = initialPosition.y - Game::camera.y;
	}
};

#endif
