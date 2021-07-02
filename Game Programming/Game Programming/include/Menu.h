#pragma once
#include "UILabel.h"
#include <vector>
#include <SDL_ttf.h>
#include "Numbers.h"
#include <string>
#include "Game.h"
#include <SDL.h>

using namespace std;

class Menu {
	vector<string> labels;
	vector<UILabel> uiLabels;
	SDL_Color red = { 255,0,0 };
	SDL_Color white = { 255,255,255 };
	bool selected[10];
	SDL_Texture* background;

public:

	Menu(vector<string>& labels) {

		this->labels = labels;
		int halfWidth = SCREENWIDTH / 2;
		int halfHeight = SCREENHEIGHT / 2 - 200;
		for (int i = 0; i < labels.size(); i++) {
			uiLabels.push_back(UILabel(halfWidth, halfHeight + i * 100, labels[i], "arial", red) );
			SDL_Rect pos = uiLabels[i].getPosition();
			uiLabels[i].setPosition(pos.x - pos.w / 2, pos.y);
			selected[i] = false;
		}
	}

	void draw() {

		SDL_RenderClear(Game::renderer);

		
		SDL_RenderCopy(Game::renderer, background, NULL, NULL);

		for (int i = 0; i < uiLabels.size(); i++) {
			uiLabels[i].draw();
		}

		SDL_RenderPresent(Game::renderer);
	};

	int showMenu() {
		const int fps = 60;
		const int frameDelay = 1000 / fps;

		Uint32 framestart;
		int frametime;
		int ret;

		SDL_FillRect(Game::screen, NULL, SDL_MapRGB(Game::screen->format, 0x00, 0x00, 0x00));
		background = SDL_CreateTextureFromSurface(Game::renderer, Game::screen);

		SDL_Event event;
		while (true) {
			framestart = SDL_GetTicks();
			draw();
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					return 2;
					break;
				case SDL_MOUSEMOTION:
					checkHovering(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
					ret = checkSelectedOption(event);
					if (ret != -1) {
						return ret;
					}
					break;
				default:
					break;
				}
			}
			frametime = SDL_GetTicks() - framestart;

			if (frameDelay > frametime) {
				SDL_Delay(frameDelay - frametime);
			}
			
		}
	}

	void checkHovering(SDL_Event event) {
		int x = event.motion.x;
		int y = event.motion.y;
		for (int i = 1; i < uiLabels.size(); i++) {
			SDL_Rect pos = uiLabels[i].getPosition();
			if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
				if (!selected[i]) {
					uiLabels[i].setColor(white);
					selected[i] = true;
				}
			}
			else {
				if (selected[i]) {
					uiLabels[i].setColor(red);
					selected[i] = false;
				}
			}
		}
	}

	int checkSelectedOption(SDL_Event event) {
		int x = event.motion.x;
		int y = event.motion.y;
		for (int i = 1; i < uiLabels.size(); i++) {
			SDL_Rect pos = uiLabels[i].getPosition();
			if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
				return i;
			}
		}
		return -1;
	}
};
