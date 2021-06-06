#include "../include/Collision.h"

/*
* Checks if Rect A and Rect B overlap
* @param A Rect A
* @param B Rect B
*/
bool Collision::AABB(const SDL_Rect& A, const SDL_Rect& B) {
	// Check x axis and y axis
	if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
		A.y + A.h >= B.y && B.y + B.h >= A.y) {
		return true;
	}
	return false;
}

/*
* Checks if two colliders overlap. Calls AABB with rects.
* @param c1 Collider A
* @param c2 Collider B
*/
bool Collision::AABB(const ColliderComponent& c1, const ColliderComponent& c2) {
	return AABB(c1.collider, c2.collider);
}

/*
* Checks if two colliders overlap and returns direction in which c1 should bounce back
* @param c1 Collider1 that should bounce back
* @param c2 Collider 2
* @return Returns -1.1 if A is less than B else returns 1.1
*/
float Collision::AABB_direction(const ColliderComponent& c1, const ColliderComponent& c2) {
	if (AABB(c1, c2)) {
		SDL_Rect A = c1.collider;
		SDL_Rect B = c2.collider;
		if (A.x < B.x) {
			return -1.1;
		}
		else {
			return 1.1;
		}
	}
	return 0.0f;
}

/*
* Equal to AABB at the moment. Goal: Only return true if player is above tile
*/
bool Collision::TileCollision(const ColliderComponent& player, const ColliderComponent& tile) {
	SDL_Rect A = player.collider;
	SDL_Rect B = tile.collider;
	// Check x axis and y axis
	if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
		A.y + A.h >=B.y && B.y + B.h >= A.y) {
		return true;
	}
	return false;
}