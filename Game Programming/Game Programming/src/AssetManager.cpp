#include "../include/AssetManager.h"
#include "../include/TextureManager.h"
#include "../include/TransformComponent.h"
#include "../include/SpriteComponent.h"
#include "../include/ColliderComponent.h"
#include "../include/ProjectileComponent.h"
#include "../include/Game.h"
#include "../include/KeyboardController.h"
#include "../include/Stats.h"
#include "../include/Weapons.h"
#include "../include/EnemyComponent.h"
#include "../include/Gravity.h"

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
void AssetManager::createProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group) {
	Entity* projectile = manager->addEntity();
	projectile->addComponent<TransformComponent>(position, 32, 32, 1);
	projectile->addComponent<SpriteComponent>("projectile", false);
	projectile->addComponent<ColliderComponent>("projectile");
	projectile->addComponent<ProjectileComponent>(range, speed, velocity);
	projectile->addGroup(group);
}

void AssetManager::createPlayer() {
	Entity* player = manager->addEntity();
	Stats* stats = &player->addComponent<Stats>(3, Weapons::pistol, 4, 1, true);
	stats->addSecondaryWeapon(Weapons::machineGun);
	player->addComponent<TransformComponent>(400, 0);
	player->addComponent<SpriteComponent>("player", true);
	player->addComponent<KeyboardController>();
	player->addComponent<ColliderComponent>("Player", 8, 0, 12, 0);
	player->addGroup(Game::groupPlayer);
}

void AssetManager::createEasyEnemy() {
	Entity* enemy = manager->addEntity();
	enemy->addComponent<Stats>(1, Weapons::easyEnemyGun, 3, 1, false);
	enemy->addComponent<TransformComponent>(200, 0, 64, 64, 2);
	enemy->addComponent<SpriteComponent>("enemy");
	enemy->addComponent<ColliderComponent>("Enemy");
	enemy->addComponent<EnemyComponent>();
	enemy->addComponent<GravityComponent>();
	enemy->addGroup(Game::groupEnemy);
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