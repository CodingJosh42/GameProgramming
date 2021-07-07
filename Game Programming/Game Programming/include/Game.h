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
	SDL_Window* window;

public:
	enum groupLabels : size_t {
		groupTile,
		groupPlayer,
		groupEnemy,
		groupTileColliders,
		groupPlayerProjectiles,
		groupEnemyProjectiles,
		groupEffects,
		groupHUD
	};

	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width,  int height, bool fullscreen);
	void update();
	void render();
	void clean();
	void handleEvents();
	bool running();
	bool checkIfWon();
	void startGame();
	void cleanGame();
	void playMusic();

	static SDL_Surface* screen;
	static Manager manager;
	static SDL_Event event;
	static SDL_Renderer* renderer;
	static vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
	static bool isRunning;
	static bool gameOver;
	static bool gameWon;
	static bool easyMode;
	static bool exploreMap;
	static AssetManager* assetManager;
	static Entity* player;
};

#endif
