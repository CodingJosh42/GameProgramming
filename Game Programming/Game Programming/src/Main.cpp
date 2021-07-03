#include <stdio.h>
#include <SDL.h>
#include "../include/Game.h"
#include "../include/Menu.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
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

        if (!Game::gameOver && !Game::gameWon) {
            game->handleEvents();
            game->update();
            game->render();
        }
        else if(Game::gameOver) {
            vector<string> labels = { "GAME OVER", "Erneut spielen", "Beenden" };
            Menu gameOverMenu = Menu(labels);
            game->cleanGame();
            gameOverMenu.setSound("gameover");
            int i = gameOverMenu.showMenu();
            if (i == 1) {
                Game::gameOver = false;
                game->startGame();
            }
            else if (i == 2) {
                Game::isRunning = false;
            }
        }
        else if (Game::gameWon) {
            vector<string> labels = { "DU HAST GEWONNEN!", "Erneut spielen", "Beenden" };
            Menu gameOverMenu = Menu(labels);
            game->cleanGame();
            gameOverMenu.setSound("gamewon");
            int i = gameOverMenu.showMenu();
            if (i == 1) {
                Game::gameWon = false;
                game->startGame();
            }
            else if (i == 2) {
                Game::isRunning = false;
            }
        }

        frametime = SDL_GetTicks() - framestart;

        if (frameDelay > frametime) {
            SDL_Delay(frameDelay - frametime);
        }
    }

    game->clean();

	return 0;
}