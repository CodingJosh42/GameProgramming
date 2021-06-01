#ifndef TILECOMPONENT
#define TILECOMPONENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"

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
			tag = "";
			break;
		case 1:
			path = "assets/grass.png";
			tag = "terrain";
			break;
		case 2:
			path = "assets/water.png";
			tag = "terrain";
			break;
		case 3:
			path = "assets/metal.png";
			tag = "terrain";
			break;
		case 4:
			path = "assets/dirt.png";
			tag = "terrain";
			break;
		case 5:
			path = "assets/cloud.png";
			tag = "";
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

		if (tag == "terrain") {
			ColliderComponent* collider = &entity->addComponent<ColliderComponent>(tag);
			Game::colliders.push_back(collider);
		}
	}

	void update() override {
		position->position.x = initialPosition.x - Game::camera.x;
		position->position.y = initialPosition.y - Game::camera.y;
	}
};

#endif
