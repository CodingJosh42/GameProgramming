#include "../include/Game.h"
#include "../include/TextureManager.h"
#include <vector>
#include "../include/Map.h"
#include "../include/ESC.h"
#include "../include/TransformComponent.h"
#include "../include/SpriteComponent.h"
#include "../include/KeyboardController.h"
#include "../include/Collision.h"
#include "../include/ColliderComponent.h"
#include "../include/TileComponent.h"
#include "../include/EnemyComponent.h"


using namespace std;



Manager Game::manager;
Entity* Game::player;
AssetManager* Game::assetManager = new AssetManager(&manager);

bool Game::isRunning = false;
SDL_Rect Game::camera = { 0,0,800,640 };


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
		if (renderer) {
			// SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
	assetManager->addTexture("player", "assets/animation_player.png");
	assetManager->addTexture("easyEnemy", "assets/easyEnemy.png");
	assetManager->addTexture("sniper", "assets/sniper.png");

	assetManager->addTexture("sky", "assets/sky.png");
	assetManager->addTexture("grass", "assets/grass.png");
	assetManager->addTexture("water", "assets/water.png");
	assetManager->addTexture("metal", "assets/metal.png");
	assetManager->addTexture("dirt", "assets/dirt.png");
	assetManager->addTexture("cloud", "assets/cloud.png");

	assetManager->addTexture("projectile", "assets/projectile.png");
	assetManager->addTexture("sniperProjectile", "assets/sniperProjectile.png");

	assetManager->addTexture("heart", "assets/heart.png");
	


	Map map;

	assetManager->createPlayer();
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	player = players[0];

	assetManager->createEasyEnemy();
	assetManager->createSniperEnemy();
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
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTile);
	for (Entity* tile: tiles) {
		if (tile->getComponent<TileComponent>().tag == "terrain") {
			ColliderComponent collider = tile->getComponent<ColliderComponent>();

			// keyboard->jumpHeight + position.height * position.scale - 32
			if (Collision::AABB(playerCollider, collider) ) {
				Collision::CollisionType collision = Collision::yCollision(playerCollider, collider);
				if (collision == Collision::TOP) {
					if (keyboard->flying && collider.collider.y < position.position.y + position.height * position.scale -32) {
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
						}
					}
					else {
						player->getComponent<TransformComponent>().position.y = position.position.y;
						keyBoardCollision = true;
					}
				}
				if (collision == Collision::BOTTOM) {
					player->getComponent<TransformComponent>().velocity.y = 1;
				}
				
				if ((position.height * position.scale + keyboard->jumpHeight - 16) > collider.collider.y  && !keyboard->flying) {
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

	camera.x = player->getComponent<TransformComponent>().position.x - 400;
	camera.y = player->getComponent<TransformComponent>().position.y - (maxHeight - 128);
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w - 400)
		camera.x = camera.w - 400;
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


