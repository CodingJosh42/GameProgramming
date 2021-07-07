#ifndef ASSETMANAGER
#define ASSETMANAGER

#include <string>
#include <map>
#include <SDL.h>
#include "components/ESC.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

class AssetManager {
public:
	AssetManager(Manager* manager);
	~AssetManager();

	void createProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group);
	void createSniperProjectile(Vector2D position, int range, int speed, Vector2D velocity, int group);
	void createHitAnimation(Vector2D position, int width);
	void createPlayer();
	void createEasyEnemy(int x, int y);
	void createSniperEnemy(int x, int y);

	static int id;

	void addTexture(string id, const char* path);
	SDL_Texture* getTexture(string id);

	void addFont(string id, const char* path, int fontSize, int style);
	TTF_Font* getFont(string id);

	void addSound(string id, const char* path);
	Mix_Chunk* getSound(string id);

	void addMusic(string id, const char* path);
	Mix_Music* getMusic(string id);
private:
	Manager* manager;
	map<string, SDL_Texture*> textureList;
	map<string, TTF_Font*> fontList;
	map<string, Mix_Chunk*> soundList;
	map<string, Mix_Music*> musicList;
};

#endif
