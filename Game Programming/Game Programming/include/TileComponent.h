#ifndef TILECOMPONENT
#define TILECOMPONENT

#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class TileComponent : public Component {
public:
	TransformComponent* position;
	SpriteComponent* sprite;

	SDL_Rect dest;
	int tileId;
	const char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) : tileId{ id } {
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;

		switch (tileId) {
		case 0:
			path = "assets/sky";
			break;
		case 1:
			path = "assets/grass";
			break;
		case 2:
			path = "assets/water";
			break;
		case 3:
			path = "assets/metal";
			break;
		case 4:
			path = "assets/dirt";
			break;
		case 5:
			path = "assets/cloud";
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
};

#endif
