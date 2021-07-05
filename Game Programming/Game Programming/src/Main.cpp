#include <stdio.h>
#include <SDL.h>
#include "../include/Game.h"
#include "../include/Menu.h"
#include <vector>
#include <string>
#include <iostream>
#include "../include/Numbers.h"

using namespace std;
int main(int argc, char* args[]) {
	
    Game* game = new Game();

    const int fps = 60;
    const int frameDelay = 1000 / fps;

    Uint32 framestart;
    int frametime;
    bool startMenu = true;

    game->init("2D Shooting Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, false);
    while (game->running())
    {
        framestart = SDL_GetTicks();

        if (!Game::gameOver && !Game::gameWon && !startMenu) {
            game->handleEvents();
            game->update();
            game->render();
        }
        else if(Game::gameOver) {
            Mix_HaltMusic();
            vector<string> labels = { "GAME OVER", "Erneut spielen", "Easy Mode", "Beenden" };
            Menu gameOverMenu = Menu(labels);
            game->cleanGame();
            gameOverMenu.setSound("gameover");
            int i = gameOverMenu.showMenu();
            if (i == 1) {
                Game::gameOver = false;
                game->startGame();
            }
            else if (i == 2) {
                game->cleanGame();
                Game::gameOver = false;
                Game::easyMode = true;
                game->startGame();
            }
            else if (i == 3) {
                Game::isRunning = false;
            }
        }
        else if (Game::gameWon) {
            Mix_HaltMusic();
            vector<string> labels = { "DU HAST GEWONNEN!", "Erneut spielen", "Karte erkunden", "Beenden" };
            Menu gameOverMenu = Menu(labels);
            Mix_ExpireChannel(-1, 1);
            gameOverMenu.setSound("gamewon");
            int i = gameOverMenu.showMenu();
            if (i == 1) {
                game->cleanGame();
                Game::gameWon = false;
                game->startGame();
            }
            else if (i == 2) {
                Game::gameWon = false;
                Game::exploreMap = true;
            }
            else if (i == 3) {
                Game::isRunning = false;
            }
        }
        else if (startMenu) {
            vector<string> labels = { "2D SHOOTING GAME", "Spiel starten", "Easy Mode", "Beenden" };
            Menu gameOverMenu = Menu(labels);
            int i = gameOverMenu.showMenu();
            if (i == 1) {
                startMenu = false;
                game->startGame();
            }
            else if (i == 2) {
                startMenu = false;
                Game::easyMode = true;
                game->startGame();
            }
            else if (i == 3) {
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