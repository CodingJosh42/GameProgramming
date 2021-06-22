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
#include "../include/EasyEnemy.h"

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
	projectile->addComponent<TransformComponent>(position, 8, 8, 2);
	projectile->addComponent<SpriteComponent>("projectile", false);
	projectile->addComponent<ColliderComponent>("projectile");
	projectile->addComponent<ProjectileComponent>(range, speed, velocity);
	projectile->addGroup(group);
}

/*
* Creates a sniper projectile with given values
* @param position Start of projectile
* @param range Range of projectile
* @param speed Speed of projectile
* @param velocity Direction of projectile
*/
void AssetManager::createSniperProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group) {
	Entity* projectile = manager->addEntity();
	projectile->addComponent<TransformComponent>(position, 16, 16, 1);
	projectile->addComponent<SpriteComponent>("sniperProjectile", false);
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
	player->addComponent<ColliderComponent>("Player", 8, 0, 12, 0);
	player->addComponent<KeyboardController>();
	player->addGroup(Game::groupPlayer);
}

void AssetManager::createEasyEnemy() {
	Entity* enemy = manager->addEntity();
	enemy->addComponent<Stats>(1, Weapons::easyEnemyGun, 3, 1, false);
	enemy->addComponent<TransformComponent>(1000, 500, 32, 32, 4);
	Animation standing = Animation(0, 2, 200);
	Animation walking = Animation(1, 7, 150);
	Animation jumping = Animation(2, 1, 100);

	map<const char*, Animation> animations;
	animations.emplace("standing", standing);
	animations.emplace("walking", walking);
	animations.emplace("jumping", jumping);

	enemy->addComponent<SpriteComponent>("enemy", true, animations);
	enemy->addComponent<ColliderComponent>("Enemy", 2, 0, 16, 0);
	enemy->addComponent<EnemyComponent>(EnemyComponent::EASY);
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