#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <vector>
#include "Map.h"
#include "ESC.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

using namespace std;


Map* map;
Manager manager;

Entity* player = manager.addEntity();
Entity* enemy = manager.addEntity();

int maxWidth = 0;
int maxHeight = 0;

void movePlayer(float* xpos, float* ypos) {
	(*xpos)++;
	if (*xpos >= maxWidth) {
		*xpos = -128;
	}
}

void moveEnemy(float* xpos, float* ypos) {
	(*xpos)--;
	if (*xpos <= -128) {
		*xpos = maxWidth + 64;
	}
}


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


	player->addComponent<TransformComponent>(0, maxHeight - 128, movePlayer);
	player->addComponent<SpriteComponent>("assets/player.png");

	enemy->addComponent<TransformComponent>(maxWidth - 128, maxHeight - 128, moveEnemy);
	enemy->addComponent<SpriteComponent>("assets/enemy.png");

	map = new Map();
}

void Game::update() {
	manager.refresh();
	manager.update();
}

void Game::render() {
	SDL_RenderClear(renderer);

	map->drawMap();
	manager.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::handleEvents() {
	// Get the next event
	SDL_Event event;
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


