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


using namespace std;



Manager Game::manager;
vector<ColliderComponent*> Game::colliders;
bool Game::isRunning = false;
SDL_Rect Game::camera = { 0,0,800,640 };


Entity* player = Game::manager.addEntity();;
Entity* enemy = Game::manager.addEntity();;
SDL_Event Game::event;


int maxWidth = 0;
int maxHeight = 0;

Game::Game() {

}

Game::~Game() {

}

SDL_Renderer* Game::renderer = nullptr;

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

	Map map;
	
	player->addComponent<TransformComponent>(400, 0);
	player->addComponent<SpriteComponent>("assets/animation_player.png", true);
	player->addComponent<KeyboardController>(maxHeight);
	player->addComponent<ColliderComponent>("Player");
	player->addGroup(groupPlayer);

	enemy->addComponent<TransformComponent>(maxWidth - 128, maxHeight - 128, 64, 64, 2);
	enemy->addComponent<SpriteComponent>("assets/enemy.png");
	enemy->addComponent<ColliderComponent>("Enemy");
	enemy->addGroup(groupEnemy);
	
}

void Game::update() {
	ColliderComponent playerCollider = player->getComponent<ColliderComponent>();
	TransformComponent position = player->getComponent<TransformComponent>();

	manager.refresh();
	manager.update();
	float direction = 0.0f;
	if ((direction = Collision::AABB_direction(player->getComponent<ColliderComponent>(), enemy->getComponent<ColliderComponent>())) != 0.0f) {
		// Bounce back
		player->getComponent<TransformComponent>().velocity.x = direction;
	}
	bool collision = false;
	for (ColliderComponent* collider: colliders) {
		if (collider->tag == "terrain") {
			if (Collision::TileCollision(playerCollider, *collider)) {
				player->getComponent<TransformComponent>().position.y = position.position.y;
				collision = true;
			}
		}
	}

	player->getComponent<KeyboardController>().collision = collision;


	camera.x = player->getComponent<TransformComponent>().position.x - 400;
	camera.y = player->getComponent<TransformComponent>().position.y - (maxHeight - 128);
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.x;
	if (camera.y > camera.h)
		camera.y = camera.h;
}



void Game::render() {
	SDL_RenderClear(renderer);
	vector<Entity*> tiles = Game::manager.getGroup(Game::groupTile);
	vector<Entity*> players = Game::manager.getGroup(Game::groupPlayer);
	vector<Entity*> enemys = Game::manager.getGroup(Game::groupEnemy);

	for (size_t i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}
	for (size_t i = 0; i < enemys.size(); i++) {
		enemys[i]->draw();
	}
	// map->drawMap();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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

bool Game::running() {
	return isRunning;
}


