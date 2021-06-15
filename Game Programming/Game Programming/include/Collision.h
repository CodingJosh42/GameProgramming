#ifndef COLLISION
#define COLLISION

#include <SDL.h>
#include "ColliderComponent.h"

class Collision {
public:
	typedef enum CollisionType {
		NONE,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	} CollisionType;
	static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
	static bool AABB(const ColliderComponent& c1, const ColliderComponent& c2);
	static float AABB_direction(const ColliderComponent& c1, const ColliderComponent& c2);
	/*
	* Equal to AABB at the moment. Goal: Only return true if player is above tile
	*/
	static CollisionType yCollision(const ColliderComponent& player, const ColliderComponent& tile) {

		SDL_Rect A = player.collider;
		SDL_Rect B = tile.collider;
		// Check x axis and y axis
		CollisionType retVal = NONE;
		if (A.x + A.w  -1>= B.x && B.x + B.w >= A.x &&
			A.y + A.h - 1 >= B.y && B.y + B.h >= A.y) {

			if (A.y < B.y) {
				retVal = TOP;
			}
			else {
				retVal = BOTTOM;
			}
		}
		return retVal;
	}

	static CollisionType xCollision(const ColliderComponent& player, const ColliderComponent& tile) {
		SDL_Rect A = player.collider;
		SDL_Rect B = tile.collider;
		// Check x axis and y axis
		CollisionType retVal = NONE;
		if (A.x + A.w - 1 >= B.x && B.x + B.w >= A.x &&
			A.y + A.h - 1 >= B.y && B.y + B.h  >= A.y) {

			if (A.x < B.x) {
				retVal = LEFT;
			}
			else {
				retVal = RIGHT;
			}
		}
		return retVal;
	}
};


	/*
	// now determine one most likely side... caution heuristics 
			int ox1 = B.x + B.w/2 - A.x + A.w;
			ox1 = abs(ox1);
			int ox2 = B.x + B.w/2 - A.x;
			ox2 = abs(ox2);

			if (ox1 < ox2) {
				retVal = RIGHT;
			}
			else {
				retVal = LEFT;
				ox1 = ox2;
			}

			int oy1 = B.y + B.h / 2 - A.y;
			oy1 = abs(oy1);
			int oy2 = B.y + B.h / 2 - A.y + A.h;
			oy2 = abs(oy2);

			if (oy1 > oy2) {
				if (oy1 > ox1) {
					retVal = TOP;
				}
			}
			else {
				if (oy2 < ox1) {
					retVal = BOTTOM;
				}
			}
		}
		return retVal;*/

#endif

