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
	int width = 0;
	int height = 0;

	/*
	* Constructor of Collider.
	* @param tag Tag of collider
	*/
	ColliderComponent(string tag) : tag {tag} {}

	/*
	* Constructor of Collider.
	* @param tag Tag of collider
	* @param Xoffset Offset of hitbox in x-direction
	* @param Yoffset Offset of hitbox in y-direction
	* @param widht Width of hitbox. If Xoffset is 0 then width is automatically set to width of transformComponent
	* @param widht Height of hitbox. If Yoffset is 0 then height is automatically set to height of transformComponent
	*/
	ColliderComponent(string tag, int Xoffset, int Yoffset, int width, int height) : tag{ tag }, xOffset{ Xoffset }, yOffset{ Yoffset }, width{ width }, height{ height } {}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}

		position = &(entity->getComponent<TransformComponent>());

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

