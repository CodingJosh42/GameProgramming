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
#include "../include/UILabel.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../include/Numbers.h"

using namespace std;

// Init vars
Manager Game::manager;
Entity* Game::player;
AssetManager* Game::assetManager = new AssetManager(&manager);
bool Game::isRunning = false;
bool Game::gameOver = false;
bool Game::gameWon = false;
bool Game::easyMode = false;
bool Game::exploreMap = false;
SDL_Surface* Game::screen = NULL;

SDL_Rect Game::camera = { 0,0,SCREENWIDTH,(50 * 32 - SCREENHEIGHT) / 2};

SDL_Event Game::event;

vector <tuple<int, int, EnemyComponent::EnemyType>> spawnPoints;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {

}

Game::~Game() {

}


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

		renderer = SDL_CreateRenderer(window, -1, 0);

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
		
	}
	// Textures
	addAssets();
}

/*
* Clean the game
*/
void Game::cleanGame() {
	SDL_RenderClear(renderer);
	Game::manager.clear();
	spawnPoints.clear();
}

/*
* Let the background music start playing
*/
void Game::playMusic() {
	Mix_PlayMusic(assetManager->getMusic("background_music"), -1);
}

void Game::startGame() {
	// Background music
	playMusic();

	// Map and Spawn Points
	Map::loadMap("assets/map/map100x50.map", 100, 50);
	vector<tuple<int, int, EnemyComponent::EnemyType>> sp = Map::loadSpawnPoints("assets/map/spawn_points.map", 100, 50);
	spawnPoints.insert(spawnPoints.begin(), sp.begin(), sp.end());

	// Player
	assetManager->createPlayer();
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	player = players[0];
	// Set Camera
	camera.y = player->getComponent<TransformComponent>().position.y;
	camera.x = 0;
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
	// Let Enemys spawn
	for (tuple<int, int, EnemyComponent::EnemyType>& spawnPoint : spawnPoints) {
		int x; int y;
		EnemyComponent::EnemyType type;
		tie(x, y, type) = spawnPoint;
		switch (type) {
		case EnemyComponent::EASY:
			assetManager->createEasyEnemy(x, y);
			break;
		case EnemyComponent::SNIPER:
			assetManager->createSniperEnemy(x, y);
			break;
		default:
			break;
		}
	}

	assetManager->createHUD(player);

}
/**
* Add all necassary assets to the assetManager
*/
void Game::addAssets() {
	// Textures
	// Player and enemys
	assetManager->addTexture("playerPistol", "assets/textures/animation_player.png");
	assetManager->addTexture("playerMachineGun", "assets/textures/player_machinegun.png"); 
	assetManager->addTexture("shielding_pistol", "assets/textures/shielding_pistol.png");
	assetManager->addTexture("shielding_machinegun", "assets/textures/shielding_machinegun.png");
	assetManager->addTexture("easyEnemy", "assets/textures/easyEnemy.png");
	assetManager->addTexture("sniper", "assets/textures/sniper.png");
	// Tiles
	assetManager->addTexture("tiles", "assets/map/tiles.png");
	// Projectiles
	assetManager->addTexture("projectile", "assets/textures/projectile.png");
	assetManager->addTexture("sniperProjectile", "assets/textures/sniperProjectile.png");
	assetManager->addTexture("bullet_hit", "assets/textures/bullet_hit.png");
	// HUD
	assetManager->addTexture("heart", "assets/hud/heart.png");
	assetManager->addTexture("ammo", "assets/hud/ammo.png");

	// Fonts
	assetManager->addFont("arial", "assets/fonts/arial.ttf", 32, TTF_STYLE_NORMAL);
	assetManager->addFont("arial32bold", "assets/fonts/arial.ttf", 32, TTF_STYLE_BOLD);
	assetManager->addFont("arial48", "assets/fonts/arial.ttf", 48, TTF_STYLE_NORMAL);
	assetManager->addFont("menu48", "assets/fonts/COLONNA.TTF", 48, TTF_STYLE_NORMAL);
	assetManager->addFont("menu32", "assets/fonts/COLONNA.TTF", 32, TTF_STYLE_NORMAL);

	// Sound
	// Gun Sounds
	assetManager->addSound("reloading", "assets/audio/reloading.wav");
	assetManager->addSound("gunshot", "assets/audio/gunshot.wav");
	assetManager->addSound("easyEnemyShot", "assets/audio/easy_enemy_shot.wav");
	assetManager->addSound("sniperShot", "assets/audio/sniper_shot.wav");
	assetManager->addSound("changeGun", "assets/audio/change_gun.wav");
	assetManager->addSound("shielding", "assets/audio/shielding.wav");
	// Death sounds
	assetManager->addSound("death", "assets/audio/death_scream.wav");
	// Effect sounds
	assetManager->addSound("bullet_hit", "assets/audio/bullet_hit.wav");
	assetManager->addSound("player_hit", "assets/audio/player_hit.wav");
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
	// Music
	assetManager->addMusic("background_music", "assets/audio/jamesbond.wav");
}

/*
* Check if player won the game 
* @return Returns true when all enemys were defeated
*/
bool Game::checkIfWon() {
	vector<Entity*> enemys = Game::manager.getGroup(Game::groupEnemy);
	if (enemys.size() <= 0) {
		return true;
	}
	return false;
}

/*
* Entities in manager are getting updated. Check for collisions after. Update camera
*/
void Game::update() {
	if (!exploreMap) {
		gameWon = checkIfWon();
	}

	ColliderComponent playerCollider = player->getComponent<ColliderComponent>();
	TransformComponent position = player->getComponent<TransformComponent>();
	KeyboardController* keyboard = &player->getComponent<KeyboardController>();

	// Check camera width and height
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	manager.refresh();
	manager.update();

	//  Player-Enemy collision
	float direction = 0.0f;
	vector<Entity*> enemys = Game::manager.getGroup(Game::groupEnemy);
	for (Entity* enemy : enemys) {
		if ((direction = Collision::AABB_direction(player->getComponent<ColliderComponent>(), enemy->getComponent<ColliderComponent>())) != 0.0f) {
			// Bounce back
			player->getComponent<TransformComponent>().velocity.x = direction;
		}
	}

	// Tile collision vars
	bool keyBoardCollision = false;
	bool dontChange = false;
	bool wasCrouching = false;
	string newTag = "";
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTileColliders);
	int playerSpeed = player->getComponent<Stats>().getSpeed();

	// tile collision
	for (Entity* tile: tiles) {
		ColliderComponent collider = tile->getComponent<ColliderComponent>();

		if (Collision::AABB(playerCollider, collider) ) {
			Collision::CollisionType collision = Collision::yCollision(playerCollider, collider);
			if (collision == Collision::TOP) {
				if (keyboard->flying && collider.collider.y < position.position.y + position.height * position.scale - (3 * playerSpeed)) {
						
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
			// Doesnt work yet
			if (collision == Collision::BOTTOM && keyboard->wasCrouching) {
				keyboard->crouch();
				wasCrouching = true;
			}
				
			// x collision when player is not jumping
			if (keyboard->jumpHeight != -1 && !keyboard->falling) {
				if ((position.height * position.scale + position.position.y - (3 * playerSpeed)) > collider.collider.y && !keyboard->flying) {
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
	keyboard->wasCrouching = wasCrouching;
	


	// Projectile collision
	
	// Player projectiles hitting enemys
	vector<Entity*> projectiles = Game::manager.getGroup(Game::groupPlayerProjectiles);

	for (Entity* projectile: projectiles) {
		for (Entity* enemy : enemys) {
			ColliderComponent* enemyCollider = &enemy->getComponent<ColliderComponent>();
			if (enemyCollider && projectile) {
				if (Collision::AABB(*enemyCollider, projectile->getComponent<ColliderComponent>())) {
					TransformComponent projectilePos = projectile->getComponent<TransformComponent>();
					assetManager->createHitAnimation(projectilePos.position, projectilePos.width* projectilePos.scale * projectilePos.velocity.x);
					Mix_PlayChannel(-1, assetManager->getSound("bullet_hit"), 0);
					projectile->destroy();
					Stats* stats = &enemy->getComponent<Stats>();
					stats->reduceHealth(1);
				}
			}
		}
	}

	// Enemy projectiles hitting player
	vector<Entity*> enemyProjectiles = Game::manager.getGroup(Game::groupEnemyProjectiles);

	for (Entity* projectile : enemyProjectiles) {
			if (Collision::AABB(playerCollider, projectile->getComponent<ColliderComponent>())) {
				TransformComponent projectilePos = projectile->getComponent<TransformComponent>();
				assetManager->createHitAnimation(projectilePos.position, projectilePos.width * projectilePos.scale* projectilePos.velocity.x);
				projectile->destroy();
				Stats* stats = &player->getComponent<Stats>();
				stats->reduceHealth(1);
			}
		}
	// Update x camera
	camera.x = player->getComponent<TransformComponent>().position.x - SCREENWIDTH / 2;

	// Update y camera and player y position based on the difference
	// Camera moving up
	if (player->getComponent<TransformComponent>().position.y < camera.y) {
		int diff = abs(position.position.y - camera.y) + 64;
		camera.y -= diff;
		player->getComponent<TransformComponent>().position.y += diff;
		keyboard->jumpHeight += diff;

	}
	// Camera moving down
	if (player->getComponent<TransformComponent>().position.y + (position.height * position.scale) > camera.y + SCREENHEIGHT) {
		int diff = abs(position.position.y + (position.height * position.scale) - camera.y - SCREENHEIGHT - 200);
		camera.y += diff;
		player->getComponent<TransformComponent>().position.y -= diff;

	}
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
	vector<Entity*> effects = Game::manager.getGroup(Game::groupEffects);
	vector<Entity*> hud = Game::manager.getGroup(Game::groupHUD);

	for (size_t i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
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
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}
	for (size_t i = 0; i < effects.size(); i++) {
		effects[i]->draw();
	}
	for (size_t i = 0; i < hud.size(); i++) {
		hud[i]->draw();
	}

	SDL_RenderPresent(renderer);
}

/*
* Free resources
*/
void Game::clean() {
	cleanGame();
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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
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


