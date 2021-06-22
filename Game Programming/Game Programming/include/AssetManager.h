#ifndef ASSETMANAGER
#define ASSETMANAGER

#include <string>
#include <map>
#include <SDL.h>
#include "ESC.h"
#include "Vector2D.h"

using namespace std;

class AssetManager {
public:
	AssetManager(Manager* manager);
	~AssetManager();

	void createProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group);
	void createSniperProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group);
	void createPlayer();
	void createEasyEnemy();
	void createSniperEnemy();
	void addTexture(string id, const char* path);
	SDL_Texture* getTexture(string id);

private:
	Manager* manager;
	map<string, SDL_Texture*> textureList;
};

#endif
