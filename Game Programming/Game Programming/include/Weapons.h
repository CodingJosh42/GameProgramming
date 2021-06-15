#ifndef WEAPONS
#define WEAPONS

#include <SDL.h>
#include <string>

using namespace std;
class Weapon {
public:
	int range;
	int speed;
	int delay;
	int maxAmmo;
	int currentAmmo;
	int reloadTime;
	
	SDL_Rect src;
	int width;
	int height;
	int scale;
	string texture;

	Weapon() = default;

	/*
	* Constructor of Weapon Class
	* @param range Range of weapon
	* @param speed Speed of Projectiles
	* @param delay Shooting delay between projectiles
	* @param ammo Amount of bullets in weapon
	* @param reloadTime Time in ms the player needs to reload the weapon
	*/
	Weapon(int range, int speed, int delay, int ammo, int reloadTime, string id, int width, int height, int scale) : range{ range }, speed{ speed }, delay{ delay }, maxAmmo{ ammo }, reloadTime{ reloadTime }, texture{ id }{
		currentAmmo = maxAmmo;
		setRect(width, height, scale);
	}

	void reduceAmmo() {
		currentAmmo--;
	}

	void reload() {
		currentAmmo = maxAmmo;
	}

	void setRect(int width, int height,  int scale) {
		src.x = 0;
		src.y = 0;
		src.w = width;
		src.h = height;
		this->width = width;
		this->height = height;
		this->scale = scale;
	}

};

class Weapons {
public:
	static Weapon pistol;
	static Weapon machineGun;
};

#endif