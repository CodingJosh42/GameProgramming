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
AssetManager* Game::assetManager = new AssetManager(&manager);

bool Game::isRunning = false;
SDL_Rect Game::camera = { 0,0,800,640 };


Entity* player;
Entity* enemy = Game::manager.addEntity();
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
	assetManager->addTexture("enemy", "assets/enemy.png");

	assetManager->addTexture("sky", "assets/sky.png");
	assetManager->addTexture("grass", "assets/grass.png");
	assetManager->addTexture("water", "assets/water.png");
	assetManager->addTexture("metal", "assets/metal.png");
	assetManager->addTexture("dirt", "assets/dirt.png");
	assetManager->addTexture("cloud", "assets/cloud.png");

	assetManager->addTexture("projectile", "assets/projectile.png");

	assetManager->addTexture("heart", "assets/heart.png");

	assetManager->addTexture("pistol", "assets/pistol.png");
	assetManager->addTexture("machineGun", "assets/machineGun.png");


	Map map;

	assetManager->createPlayer();
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	player = players[0];

	enemy->addComponent<TransformComponent>(maxWidth + 128, maxHeight - 128 - 32, 64, 64, 2);
	enemy->addComponent<SpriteComponent>("enemy");
	enemy->addComponent<ColliderComponent>("Enemy");
	enemy->addComponent<EnemyComponent>();
	enemy->addGroup(groupEnemy);
	
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
	if ((direction = Collision::AABB_direction(player->getComponent<ColliderComponent>(), enemy->getComponent<ColliderComponent>())) != 0.0f) {
		// Bounce back
		player->getComponent<TransformComponent>().velocity.x = direction;
	}

	// Tile collision
	bool collision = false;
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTile);
	for (Entity* tile: tiles) {
		if (tile->getComponent<TileComponent>().tag == "terrain") {
			ColliderComponent collider = tile->getComponent<ColliderComponent>();
			if (Collision::TileCollision(playerCollider, collider)) {

				if (!keyboard->ignoreCollision) {
					player->getComponent<TransformComponent>().position.y = position.position.y;
				}

				if ((position.height * position.scale + keyboard->jumpHeight - 32) > collider.collider.y && collider.collider.y >= keyboard->jumpHeight) {
					player->getComponent<TransformComponent>().position.x = position.position.x;
					}		
				collision = true;
			}
		}
	}
	keyboard->collision = collision;

	// Projectile collision
	ColliderComponent enemyCollider = enemy->getComponent<ColliderComponent>();
	
	vector<Entity*> projectiles = Game::manager.getGroup(Game::groupProjectiles);

	for (Entity* projectile: projectiles) {
		if (Collision::AABB(enemyCollider, projectile->getComponent<ColliderComponent>()) ) {
			projectile->destroy();
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
	vector<Entity*> projectiles = Game::manager.getGroup(Game::groupProjectiles);

	for (size_t i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}
	for (size_t i = 0; i < enemys.size(); i++) {
		enemys[i]->draw();
	}
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
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


