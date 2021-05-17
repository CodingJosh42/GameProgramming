#include "../include/Collision.h"

bool Collision::AABB(const SDL_Rect& A, const SDL_Rect& B) {
	// Check x axis and y axis
	if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
		A.y + A.h >= B.y && B.y + B.h >= A.y) {
		return true;
	}
	return false;
}


bool Collision::AABB(const ColliderComponent& c1, const ColliderComponent& c2) {
	return AABB(c1.collider, c2.collider);
}


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