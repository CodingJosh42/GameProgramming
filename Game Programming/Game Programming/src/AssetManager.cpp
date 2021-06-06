#include "../include/AssetManager.h"
#include "../include/TextureManager.h"
#include "../include/TransformComponent.h"
#include "../include/SpriteComponent.h"
#include "../include/ColliderComponent.h"
#include "../include/ProjectileComponent.h"
#include "../include/Game.h"

#include <iostream>
AssetManager::AssetManager(Manager* manager) : manager{ manager } {}

AssetManager::~AssetManager() {}

void AssetManager::createProjectile(int x, int y, int range, int speed, Vector2D velocity) {
	Entity* projectile = manager->addEntity();
	projectile->addComponent<TransformComponent>(x, y, 32, 32, 1);
	projectile->addComponent<SpriteComponent>("projectile", false);
	projectile->addComponent<ColliderComponent>("projectile");
	projectile->addComponent<ProjectileComponent>(range, speed, velocity);
	projectile->addGroup(Game::groupProjectiles);
}

void AssetManager::addTexture(string id, const char* path) {
	SDL_Texture* texture = TextureManager::LoadTexture(path);
	textureList.emplace(id, texture);
}

SDL_Texture* AssetManager::getTexture(string id) {
	return textureList[id];
}