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

/*
* Creates a projectile with given values
* @param position Start of projectile
* @param range Range of projectile
* @param speed Speed of projectile
* @param velocity Direction of projectile
*/
void AssetManager::createProjectile(Vector2D position, int range, int speed, Vector2D velocity) {
	Entity* projectile = manager->addEntity();
	projectile->addComponent<TransformComponent>(position, 32, 32, 1);
	projectile->addComponent<SpriteComponent>("projectile", false);
	projectile->addComponent<ColliderComponent>("projectile");
	projectile->addComponent<ProjectileComponent>(range, speed, velocity);
	projectile->addGroup(Game::groupProjectiles);
}

/*
* Loads a texture and adds it to the list of textures
* @param id Id of texture
* @param path Path of asset
*/
void AssetManager::addTexture(string id, const char* path) {
	SDL_Texture* texture = TextureManager::LoadTexture(path);
	textureList.emplace(id, texture);
}

/*
* Returns a specific texture from the list
* @param id Id of texture that should be returned
* @return Returns the requested texture
*/
SDL_Texture* AssetManager::getTexture(string id) {
	return textureList[id];
}