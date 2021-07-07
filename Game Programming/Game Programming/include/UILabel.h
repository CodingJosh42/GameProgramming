#pragma once

#include "components/ESC.h"
#include "TextureManager.h"
#include <SDL.h>
#include <string>
#include "Game.h"

using namespace std;

class UILabel {
private:
	SDL_Rect position;
	string labelText;
	string fontId;
	SDL_Color textColor;
	SDL_Texture* texture = nullptr;

public:

	UILabel() = default;

	/**
	* Constructor of class. Sets text of label to given param
	* @param x x-Position of label
	* @param y y-Position of label
	* @param text Text of label
	* @param font FontId of the text
	* @param color Textcolor
	*/
	UILabel(int x, int y, string text, string font, SDL_Color& color) : labelText( text ), fontId( font ), textColor{ color } {
		position.x = x;
		position.y = y;

		setLabelText(text);
	}

	/**
	* Set text of Label
	* @param labelText Text that should be displayed by the label
	*/
	void setLabelText(string labelText) {
		this->labelText = labelText;
		SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontId), labelText.c_str(), textColor);
		if (texture) {
			SDL_DestroyTexture(texture);
		}
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
	}

	void destroyTex() {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	/*
	* Sets the text color of the label
	* @param color New color
	*/
	void setColor(SDL_Color color) {
		textColor = color;
		SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontId), labelText.c_str(), textColor);
		if (texture) {
			SDL_DestroyTexture(texture);
		}
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
	}

	/*
	* Returns the position of the UILabel
	*/
	SDL_Rect& getPosition() {
		return position;
	}

	/*
	* Set x and y of position
	*/
	void setPosition(int x, int y) {
		position.x = x;
		position.y = y;
	}

	/**
	* Draws the texture of the label to the screen
	*/
	void draw() {
		SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
	}

};
