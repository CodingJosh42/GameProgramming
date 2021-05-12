#ifndef GAME
#define GAME

#include <SDL.h>
#include <SDL_image.h>

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width,  int height, bool fullscreen);

	void update();
	void render();
	void clean();
	void handleEvents();

	bool running();

	static SDL_Event event;
	static SDL_Renderer* renderer;

private:

	bool isRunning;
	SDL_Window* window;
	
};

#endif
