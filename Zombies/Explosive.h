#pragma once

#include "Projectile.h"

class Explosive : public Projectile
{
public:
	Explosive(Map* map, Vector2 pos, float rotation, bool upgraded = false, GameEntity* user = nullptr);
	~Explosive();

private:
	bool mUpgraded;

	void Hit(PhysEntity* other) override;
};