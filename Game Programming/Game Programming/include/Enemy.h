#ifndef ENEMYCLASS
#define ENEMYCLASS

#include "Stats.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "Vector2D.h"
#include <iostream>

class Enemy {
public:
	
	virtual void init(Stats* stats, TransformComponent* position) = 0;
	virtual void movement(Vector2D& distance, Vector2D& direction) = 0;

};

#endif