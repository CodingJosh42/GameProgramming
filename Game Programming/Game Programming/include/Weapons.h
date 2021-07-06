#ifndef WEAPONS
#define WEAPONS

#include <iostream>
using namespace std;
class Weapon {
public:
	int range;
	int speed;
	int delay;
	int maxAmmo;
	int currentAmmo;
	int reloadTime;

	Weapon() = default;

	/*
	* Constructor of Weapon Class
	* @param range Range of weapon
	* @param speed Speed of Projectiles
	* @param delay Shooting delay between projectiles
	* @param ammo Amount of bullets in weapon
	* @param reloadTime Time in ms the player needs to reload the weapon
	*/
	Weapon(int range, int speed, int delay, int ammo, int reloadTime) : range{ range }, speed{ speed }, delay{ delay }, maxAmmo{ ammo }, reloadTime{ reloadTime }{
		currentAmmo = maxAmmo;
	}

	/*
	* Reduce ammo for one
	*/
	void reduceAmmo() {
		currentAmmo--;
	}
	/*
	* Reload weapon
	*/
	void reload() {
		currentAmmo = maxAmmo;
	}
};

class Weapons {
public:
	static Weapon pistol;
	static Weapon machineGun;
	static Weapon easyEnemyGun;
	static Weapon sniperEnemyGun;
};

#endif