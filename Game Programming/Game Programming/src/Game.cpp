#include "../include/Game.h"
#include "../include/TextureManager.h"
#include <vector>
#include "../include/Map.h"
#include "../include/components/ESC.h"
#include "../include/components/TransformComponent.h"
#include "../include/components/SpriteComponent.h"
#include "../include/components/KeyboardController.h"
#include "../include/Collision.h"
#include "../include/components/ColliderComponent.h"
#include "../include/components/TileComponent.h"
#include "../include/components/EnemyComponent.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../include/Numbers.h"

using namespace std;



Manager Game::manager;
Entity* Game::player;
AssetManager* Game::assetManager = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::gameOver = false;
bool Game::gameWon = false;
bool Game::easyMode = false;
SDL_Surface* Game::screen = NULL;
SDL_Rect Game::camera = { 0,0,MAPWIDTH * TILESIZE / 2, MAPHEIGHT * TILESIZE - 35 * TILESIZE };


SDL_Event Game::event;

int maxWidth = 0;
int maxHeight = 0;

Game::Game() {

}

Game::~Game() {

}

SDL_Renderer* Game::renderer = nullptr;

/*
* Initializes SDL, creates window, loads textures and creates Player and Enemy
*/
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		maxWidth = width;
		maxHeight = height;
		renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		screen = SDL_GetWindowSurface(window);

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		cout << "Error => TTF not initialized" << endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

	}
	else {
		Mix_Volume(-1, MIX_MAX_VOLUME / 2);
		Mix_VolumeMusic(10);
		Mix_Music* music = Mix_LoadMUS("assets/audio/jamesbond.wav");
		Mix_PlayMusic(music, -1);
	}

	// Textures
	addAssets();

	
	startGame();
}

/*
* Clean the game
*/
void Game::cleanGame() {
	SDL_RenderClear(renderer);
	Game::manager.clear();
}

void Game::startGame() {
	// Map
	Map::loadMap("assets/map/map100x50.map", 100, 50);

	// Player
	assetManager->createPlayer();
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	player = players[0];

	// Enemys
	assetManager->createEasyEnemy();
	assetManager->createSniperEnemy();

	camera.y = 15 * 32;
}
/**
* Add all necassary assets to the assetManager
*/
void Game::addAssets() {

	// Player and enemys
	assetManager->addTexture("playerPistol", "assets/animation_player.png");
	assetManager->addTexture("playerMachineGun", "assets/player_machinegun.png");
	assetManager->addTexture("easyEnemy", "assets/easyEnemy.png");
	assetManager->addTexture("sniper", "assets/sniper.png");

	// Tiles
	assetManager->addTexture("tiles", "assets/map/tiles.png");


	// Projectiles
	assetManager->addTexture("projectile", "assets/projectile.png");
	assetManager->addTexture("sniperProjectile", "assets/sniperProjectile.png");

	// HUD
	assetManager->addTexture("heart", "assets/hud/heart.png");
	assetManager->addTexture("ammo", "assets/hud/ammo.png");

	// Fonts
	assetManager->addFont("arial", "assets/arial.ttf", 32);
	assetManager->addFont("arial48", "assets/arial.ttf", 48);

	// Sound
	// Gun Sounds
	assetManager->addSound("reloading", "assets/audio/reloading.wav");
	assetManager->addSound("gunshot", "assets/audio/gunshot.wav");
	assetManager->addSound("easyEnemyShot", "assets/audio/easy_enemy_shot.wav");
	assetManager->addSound("sniperShot", "assets/audio/sniper_shot.wav");
	assetManager->addSound("changeGun", "assets/audio/change_gun.wav");
	// Menu Sounds
	assetManager->addSound("gameover", "assets/audio/game_over.wav");
	assetManager->addSound("gamewon", "assets/audio/game_won.wav");
	assetManager->addSound("optionSelected", "assets/audio/menu_option_selected.wav");
	assetManager->addSound("optionHovering", "assets/audio/menu_option_hovering.wav");
	// Walking sounds
	assetManager->addSound("dirt", "assets/audio/dirt.wav");
	assetManager->addSound("grass", "assets/audio/grass.wav");
	assetManager->addSound("water", "assets/audio/water.wav");
	assetManager->addSound("metal", "assets/audio/metal.wav");
	assetManager->addSound("jump", "assets/audio/jump.wav");
	// Landing on tile sound
	assetManager->addSound("fallen_dirt", "assets/audio/fallen_dirt.wav");
	assetManager->addSound("fallen_grass", "assets/audio/fallen_grass.wav");
	assetManager->addSound("fallen_water", "assets/audio/fallen_water.wav");
	assetManager->addSound("fallen_metal", "assets/audio/fallen_metal.wav");


}

/*
* Entities in manager are getting updated. Check for collisions after. Update camera
*/
void Game::update() {
	ColliderComponent playerCollider = player->getComponent<ColliderComponent>();
	TransformComponent position = player->getComponent<TransformComponent>();
	KeyboardController* keyboard = &player->getComponent<KeyboardController>();

	manager.refresh();
	manager.update();
	

	// Enemy collision
	float direction = 0.0f;
	vector<Entity*> enemys = Game::manager.getGroup(Game::groupEnemy);
	for (Entity* enemy : enemys) {
		if ((direction = Collision::AABB_direction(player->getComponent<ColliderComponent>(), enemy->getComponent<ColliderComponent>())) != 0.0f) {
			// Bounce back
			player->getComponent<TransformComponent>().velocity.x = direction;
		}
	}

	// Tile collision
	bool keyBoardCollision = false;
	bool dontChange = false;
	string newTag = "";
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTileColliders);
	for (Entity* tile: tiles) {
		ColliderComponent collider = tile->getComponent<ColliderComponent>();

		if (Collision::AABB(playerCollider, collider) ) {
			Collision::CollisionType collision = Collision::yCollision(playerCollider, collider);
			if (collision == Collision::TOP) {
				if (keyboard->flying && collider.collider.y < position.position.y + position.height * position.scale - 12) {
						// player should bounce back from tiles while jumping
							
						collision = Collision::xCollision(playerCollider, collider);
						if (collision == Collision::LEFT) {
							player->getComponent<TransformComponent>().velocity.x = -1;
						}
						if (collision == Collision::RIGHT) {
							player->getComponent<TransformComponent>().velocity.x = 1;
						}
						if (collision == Collision::NONE) {
							player->getComponent<TransformComponent>().position.y = position.position.y;
							keyBoardCollision = true;
							keyboard->ignoreCollision = false;
							string tag = tile->getComponent<TileComponent>().tag;
							if (keyboard->tileTag == tag) {
								dontChange = true;
							}
							else {
								newTag = tag;
							}
						}
				}
				else {
					// if player is falling down
					player->getComponent<TransformComponent>().position.y = position.position.y;
					keyboard->ignoreCollision = false;
					keyBoardCollision = true;
					string tag = tile->getComponent<TileComponent>().tag;
					if (keyboard->tileTag == tag) {
						dontChange = true;
					}
					else {
						newTag = tag;
					}
				}
			}
			// player falls down
			if (collision == Collision::BOTTOM && keyboard->flying) {
				player->getComponent<TransformComponent>().velocity.y = 3;
			}
				
			// x collision when player is not jumping
			if (keyboard->jumpHeight != -1) {
				if ((position.height * position.scale + position.position.y - 12) > collider.collider.y && !keyboard->flying) {
					collision = Collision::xCollision(playerCollider, collider);
					if (collision == Collision::LEFT) {
						player->getComponent<TransformComponent>().velocity.x = -1;
					}
					if (collision == Collision::RIGHT) {
						player->getComponent<TransformComponent>().velocity.x = 1;
					}

				}
			}
				
		}
	}
	keyboard->collision = keyBoardCollision;
	if (newTag != "" && !dontChange) {
		keyboard->tileTag = newTag;
		keyboard->terrainChanged = true;
	}
	


	// Projectile collision
	
	vector<Entity*> projectiles = Game::manager.getGroup(Game::groupPlayerProjectiles);

	for (Entity* projectile: projectiles) {
		for (Entity* enemy : enemys) {
			ColliderComponent enemyCollider = enemy->getComponent<ColliderComponent>();
			if (Collision::AABB(enemyCollider, projectile->getComponent<ColliderComponent>())) {
				projectile->destroy();
				Stats* stats = &enemy->getComponent<Stats>();
				stats->reduceHealth(1);
			}
		}
	}

	vector<Entity*> enemyProjectiles = Game::manager.getGroup(Game::groupEnemyProjectiles);

	for (Entity* projectile : enemyProjectiles) {
			if (Collision::AABB(playerCollider, projectile->getComponent<ColliderComponent>())) {
				projectile->destroy();
				Stats* stats = &player->getComponent<Stats>();
				stats->reduceHealth(1);
			}
		}

	camera.x = player->getComponent<TransformComponent>().position.x - SCREENWIDTH / 2;

	if (player->getComponent<TransformComponent>().position.y  < camera.y) {
		int diff = abs(position.position.y - camera.y) + 64;
		camera.y -= diff;
		player->getComponent<TransformComponent>().position.y += diff;
		keyboard->jumpHeight += diff;

	}
	if (player->getComponent<TransformComponent>().position.y + (position.height * position.scale) > camera.y + SCREENHEIGHT) {
		int diff = abs(position.position.y + (position.height * position.scale) - camera.y - SCREENHEIGHT - 200);
		camera.y += diff;
		player->getComponent<TransformComponent>().position.y -= diff;

	}
	
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	
}


/*
* Renders rendergroup in following order: tiles, player, enemys, projectiles
*/
void Game::render() {
	SDL_RenderClear(renderer);
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTile);
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	vector<Entity*> enemys = Game::manager.getGroup(Game::groupEnemy);
	vector<Entity*> playerProjectiles = Game::manager.getGroup(Game::groupPlayerProjectiles);
	vector<Entity*> enemyProjectiles = Game::manager.getGroup(Game::groupEnemyProjectiles);

	for (size_t i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}
	for (size_t i = 0; i < enemys.size(); i++) {
		enemys[i]->draw();
	}
	for (size_t i = 0; i < playerProjectiles.size(); i++) {
		playerProjectiles[i]->draw();
	}
	for (size_t i = 0; i < enemyProjectiles.size(); i++) {
		enemyProjectiles[i]->draw();
	}

	SDL_RenderPresent(renderer);
}

/*
* Free resources
*/
void Game::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

/*
* Handles only quit event at the moment
*/
void Game::handleEvents() {
	// Get the next event
	SDL_PollEvent(&event);
	switch(event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

/*
* Returns if game is still running
*/ 
bool Game::running() {
	return isRunning;
}


