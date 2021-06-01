#ifndef COLLISION
#define COLLISION

#include <SDL.h>
#include "ColliderComponent.h"

class Collision {
public:
	static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
	static bool AABB(const ColliderComponent& c1, const ColliderComponent& c2);
	static float AABB_direction(const ColliderComponent& c1, const ColliderComponent& c2);
	static bool TileCollision(const ColliderComponent& c1, const ColliderComponent& c2);
};

#endif

