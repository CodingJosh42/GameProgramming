#pragma once
#include "UILabel.h"
#include <vector>
#include <SDL_ttf.h>
#include "Numbers.h"
#include <string>
#include "Game.h"
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

class Menu {
	// Labels and selection
	vector<string> labels;
	vector<UILabel> uiLabels;
	bool selected[10];
	// Colors
	SDL_Color red = { 255,0,0 };
	SDL_Color white = { 255,255,255 };
	// Background texture (black)
	SDL_Texture* background;
	// Sounds
	Mix_Chunk* sound = nullptr;
	Mix_Chunk* hoveringSound;
	Mix_Chunk* selectedSound;

public:

	/*
	* Constructor
	* @param labels List of options that should be displayed
	*/
	Menu(vector<string>& labels) {

		this->labels = labels;
		int halfWidth = SCREENWIDTH / 2;
		int halfHeight = SCREENHEIGHT / 2 - 200;
		string font = "arial48";
		for (int i = 0; i < labels.size(); i++) {
			if (i != 0) {
				font = "arial";
			}
			int y = halfHeight + i * 150;
			if (i > 1) {
				y = halfHeight + 150 + (i-1) * 100;
			}
			uiLabels.push_back(UILabel(halfWidth, y, labels[i], font, red) );
			// Center options horizontal
			SDL_Rect pos = uiLabels[i].getPosition();
			uiLabels[i].setPosition(pos.x - pos.w / 2, pos.y);
			selected[i] = false;
		}

		hoveringSound = Game::assetManager->getSound("optionHovering");
		selectedSound = Game::assetManager->getSound("optionSelected");
	}

	void setSound(string id) {
		sound = Game::assetManager->getSound(id);
	}

	/*
	* Draw a black screen and the menu options
	*/
	void draw() {

		SDL_RenderClear(Game::renderer);

		
		SDL_RenderCopy(Game::renderer, background, NULL, NULL);

		for (int i = 0; i < uiLabels.size(); i++) {
			uiLabels[i].draw();
		}

		SDL_RenderPresent(Game::renderer);
	};

	/*
	* Shows the menu in an infinite loop and handles for events
	* @return Returns the selected option
	*/
	int showMenu() {
		const int fps = 60;
		const int frameDelay = 1000 / fps;

		Uint32 framestart;
		int frametime;
		// Return value
		int ret;

		// Background texture
		SDL_FillRect(Game::screen, NULL, SDL_MapRGB(Game::screen->format, 0x00, 0x00, 0x00));
		background = SDL_CreateTextureFromSurface(Game::renderer, Game::screen);

		if (sound) {
			Mix_PlayChannel(-1, sound, 0);
		}
		SDL_Event event;
		while (true) {
			framestart = SDL_GetTicks();
			draw();
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					return labels.size() - 1;
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

	/*
	* Checks if user is hovering over a specific Option. If he is hovering over a specific option the text color changes to white
	* @param event Current event
	*/
	void checkHovering(SDL_Event event) {
		int x = event.motion.x;
		int y = event.motion.y;
		for (int i = 1; i < uiLabels.size(); i++) {
			// Change color to white if user hovering over option
			SDL_Rect pos = uiLabels[i].getPosition();
			if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
				if (!selected[i]) {
					uiLabels[i].setColor(white);
					selected[i] = true;
					Mix_PlayChannel(-1, hoveringSound, 0);
				}
			}
			else {
				// Change color back to red if not hovering over option
				if (selected[i]) {
					uiLabels[i].setColor(red);
					selected[i] = false;
				}
			}
		}
	}

	/*
	* Checks if user has selected an option
	* @return Returns the index of the selected option or -1 if no option were selected
	*/
	int checkSelectedOption(SDL_Event event) {
		int x = event.motion.x;
		int y = event.motion.y;
		for (int i = 1; i < uiLabels.size(); i++) {
			// Check if any option is selected. Return the index of option
			SDL_Rect pos = uiLabels[i].getPosition();
			if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
				Mix_PlayChannel(-1, selectedSound, 0);
				return i;
			}
		}
		return -1;
	}
};
