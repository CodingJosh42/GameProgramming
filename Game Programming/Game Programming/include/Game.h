#ifndef GAME
#define GAME

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "AssetManager.h"

class ColliderComponent;
class Manager;
class Entity;

using namespace std;

class Game {
private:
	void addAssets();

public:
	enum groupLabels : size_t {
		groupTile,
		groupPlayer,
		groupEnemy,
		groupColliders,
		groupPlayerProjectiles,
		groupEnemyProjectiles
	};
	static Entity* player;

	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width,  int height, bool fullscreen);

	void update();
	void render();
	void clean();
	void handleEvents();

	bool running();

	void startGame();
	void cleanGame();

	static SDL_Surface* screen;

	static Manager manager;
	static SDL_Event event;
	static SDL_Renderer* renderer;
	static vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
	static bool isRunning;
	static bool gameOver;
	static AssetManager* assetManager;

private:
	SDL_Window* window;
	
};

#endif
