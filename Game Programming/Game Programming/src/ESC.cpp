#include "../include/ESC.h"
#include "../include/Game.h"


void Entity::addGroup(size_t group) {
	groupBitset[group] = true;
	Game::manager.addToGroup(this, group);
}