#include <stdio.h>
#include <SDL.h>
#include "Game.h"

int main(int argc, char* args[]) {
	
    Game* game = new Game();

    const int fps = 60;
    const int frameDelay = 1000 / fps;

    Uint32 framestart;
    int frametime;

    game->init("TENET", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
    while (game->running())
    {
        framestart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frametime = SDL_GetTicks() - framestart;

        if (frameDelay > frametime) {
            SDL_Delay(frameDelay - frametime);
        }
    }

    game->clean();

	return 0;
}