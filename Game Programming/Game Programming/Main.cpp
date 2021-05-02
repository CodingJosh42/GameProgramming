#include <stdio.h>
#include <SDL.h>
#include "Game.h"

int main(int argc, char* args[]) {
	
    Game* game = new Game();

    game->init("TENET", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, false);
    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();

    }

    game->clean();

	return 0;
}