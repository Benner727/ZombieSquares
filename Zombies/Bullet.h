#pragma once

#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet(Map* map, Vector2 pos, float rotation, GameEntity* user = nullptr, bool shell = false);
	~Bullet();

	void Damage(int damage);
	void Penetrates(bool penetrates);
};