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
	static CollisionType yCollision(const ColliderComponent& player, const ColliderComponent& tile);
	static CollisionType xCollision(const ColliderComponent& player, const ColliderComponent& tile);
};

#endif

