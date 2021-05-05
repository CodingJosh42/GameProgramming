#include "Game.h"
#include "TextureManager.h"

using namespace std;

SDL_Texture* playerTex;
SDL_Rect srcR, destR;
int move = 0;
int maxWidth = 0;

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

	playerTex = TextureManager::LoadTexture("assets/player.png", renderer);

}

void Game::update() {
	move++;
	destR.h = 128;
	destR.w = 128;
	
	destR.x = move;
	if (move >= maxWidth) {
		move = -128;
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	// #TODO stuff to render
	SDL_RenderCopy(renderer, playerTex, NULL, &destR);
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