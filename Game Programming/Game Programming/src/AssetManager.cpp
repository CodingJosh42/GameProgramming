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

/*
* Constructor
*/
AssetManager::AssetManager(Manager* manager) : manager{ manager } {}

/*
* Destructor. Destroys every texture and font in list
*/
AssetManager::~AssetManager() {
	// Clear texture list
	map<string, SDL_Texture*>::iterator it;
	for (it = textureList.begin(); it != textureList.end(); it++)
	{
		SDL_DestroyTexture(it->second);
	}
	textureList.clear();

	// Clear font list
	map<string, TTF_Font*>::iterator fit;
	for (fit = fontList.begin(); fit != fontList.end(); fit++)
	{
		TTF_CloseFont(fit->second);
	}
	fontList.clear();

	// Clear sound list
	map<string, Mix_Chunk*>::iterator sit;
	for (sit = soundList.begin(); sit != soundList.end(); sit++)
	{
		Mix_FreeChunk(sit->second);
	}
	soundList.clear();
}

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
	projectile->addComponent<TransformComponent>(position, 16, 8, 2);
	SpriteComponent* sprite = &projectile->addComponent<SpriteComponent>("sniperProjectile", false);
	double flip = 1.0;
	if (velocity.x == -1) {
		sprite->flip = SDL_FLIP_HORIZONTAL;
		flip = -1.0;
	}
	if (velocity.y != 0) {
		sprite->angle = flip * atan(velocity.y) * 180 / M_PI;;
	}
	projectile->addComponent<ColliderComponent>("projectile");
	projectile->addComponent<ProjectileComponent>(range, speed, velocity);
	projectile->addGroup(group);
}

/*
* Creates the player entity
*/
void AssetManager::createPlayer() {
	Entity* player = manager->addEntity();
	Stats* stats = &player->addComponent<Stats>(3, Weapons::pistol, 4, 1, true);
	stats->addSecondaryWeapon(Weapons::machineGun);
	player->addComponent<TransformComponent>(400, 0);
	player->addComponent<SpriteComponent>("playerPistol", true);
	player->addComponent<ColliderComponent>("Player", 8, 0, 12, 0);
	player->addComponent<KeyboardController>();
	player->addGroup(Game::groupPlayer);
}

/*
* Creates an easy-enemy-entity
*/
void AssetManager::createEasyEnemy() {
	Entity* enemy = manager->addEntity();
	enemy->addComponent<Stats>(1, Weapons::easyEnemyGun, 3, 1, false);
	enemy->addComponent<TransformComponent>(1000, 500, 32, 32, 4);
	// Animations
	Animation standing = Animation(0, 2, 200);
	Animation walking = Animation(1, 7, 150);
	Animation jumping = Animation(2, 1, 100);
	// Animation map
	map<const char*, Animation> animations;
	animations.emplace("standing", standing);
	animations.emplace("walking", walking);
	animations.emplace("jumping", jumping);

	enemy->addComponent<SpriteComponent>("easyEnemy", true, animations);
	enemy->addComponent<ColliderComponent>("Enemy", 2, 0, 16, 0);
	enemy->addComponent<EnemyComponent>(EnemyComponent::EASY);
	enemy->addComponent<GravityComponent>();
	enemy->addGroup(Game::groupEnemy);
}

/*
* Creates a sniper-enemy-entity
*/
void AssetManager::createSniperEnemy() {
	Entity* enemy = manager->addEntity();
	enemy->addComponent<Stats>(4, Weapons::sniperEnemyGun, 2, 1, false);
	enemy->addComponent<TransformComponent>(1000, 500, 32, 32, 4);
	// Animations
	Animation standing = Animation(0, 2, 200);
	Animation walking = Animation(1, 7, 150);
	Animation jumping = Animation(2, 1, 100);
	// Animation map
	map<const char*, Animation> animations;
	animations.emplace("standing", standing);
	animations.emplace("walking", walking);
	animations.emplace("jumping", jumping);

	enemy->addComponent<SpriteComponent>("sniper", true, animations);
	enemy->addComponent<ColliderComponent>("Enemy", 2, 0, 16, 0);
	enemy->addComponent<EnemyComponent>(EnemyComponent::SNIPER);
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

/*
* Loads a font and adds it to the list of fonts
* @param id Id of font
* @param path Path of asset
* @param fontSize Size of font
*/
void AssetManager::addFont(string id, const char* path, int fontSize) {
	TTF_Font* font = TTF_OpenFont(path, fontSize);
	fontList.emplace(id, font);
}


/*
* Returns a specific font from the list
* @param id Id of font that should be returned
* @return Returns the requested font
*/
TTF_Font* AssetManager::getFont(string id) {
	return fontList[id];
}


/*
* Loads a sound and adds it to the list of sounds
* @param id Id of sound
* @param path Path of .wav file
*/
void AssetManager::addSound(string id, const char* path){
	Mix_Chunk* sound = Mix_LoadWAV(path);
	soundList.emplace(id, sound);
}

/*
* Returns a specific sound from the list
* @param id Id of sound that should be returned
* @return Returns the requested sound
*/
Mix_Chunk* AssetManager::getSound(string id) {
	return soundList[id];
}