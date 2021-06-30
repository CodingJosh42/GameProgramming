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
	string texId;
	const char* tag;
	Vector2D initialPosition;

	TileComponent() = default;

	/*
	* Constructor of tiles
	* @param x x-coordinate of tile
	* @param y y-coordinate of tile
	* @param w width of tile
	* @param h height of tile
	* @param id Id of tile
	*/
	TileComponent(int x, int y, int w, int h, int id) : tileId{ id } {
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		initialPosition.x = x;
		initialPosition.y = y;

		switch (id) {
		case 0:
			tag = "terrain";
			break;
		case 1:
			tag = "terrain";
			break;
		case 2:
			tag = "terrain";
			break;
		case 3:
			tag = "terrain";
			break;
		case 4:
			tag = "";
			break;
		case 5:
			tag = "";
			break;
		default:
			break;
		}
	}

	void init() override {
		entity->addComponent<TransformComponent>(dest.x, dest.y, dest.w, dest.h, 1);
		position = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>("tiles", tileId * 32);
		sprite = &entity->getComponent<SpriteComponent>();

		if (tag == "terrain") {
			ColliderComponent* collider = &entity->addComponent<ColliderComponent>(tag);
		}
		entity->addGroup(Game::groupColliders);
	}

	void update() override {
		position->position.x = initialPosition.x - 0.5* Game::camera.x;
		position->position.y = initialPosition.y - Game::camera.y;
	}
};

#endif
