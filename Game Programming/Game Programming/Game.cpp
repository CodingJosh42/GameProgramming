#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <vector>

using namespace std;


vector<GameObject*> objects;

int maxWidth = 0;
int maxHeight = 0;

void movePlayer(int* xpos, int* ypos) {
	(*xpos)++;
	if (*xpos >= maxWidth) {
		*xpos = -128;
	}
}

void moveEnemy(int* xpos, int* ypos) {
	(*xpos)--;
	if (*xpos <= -128) {
		*xpos = maxWidth + 64;
	}
}


Game::Game() {

}

Game::~Game() {

}

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

	// Player
	objects.push_back(new GameObject("assets/player.png", renderer, 0, maxHeight - 128, movePlayer));
	// Enemy
	objects.push_back(new GameObject("assets/enemy.png", renderer, maxWidth - 128, 0, moveEnemy));
}

void Game::update() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	// #TODO stuff to render
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->render();
	}
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


